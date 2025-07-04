/*
 * Copyright © 2010  Behdad Esfahbod
 * Copyright © 2011,2012  Google, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Google Author(s): Garret Rieger, Rod Sheeter
 */

#include "batch.hh"
#include "options.hh"

#ifdef HAVE_GLIB_H

#include "face-options.hh"
#include "glib.h"
#include "main-font-text.hh"
#include "output-options.hh"
#include "helper-subset.hh"

#include <hb-subset.h>


static hb_face_t* preprocess_face(hb_face_t* face)
{
  return hb_subset_preprocess (face);
}

/*
 * Command line interface to the harfbuzz font subsetter.
 */

struct subset_main_t : option_parser_t, face_options_t, output_options_t<false>
{
  subset_main_t ()
  : input (hb_subset_input_create_or_fail ())
  {}
  ~subset_main_t ()
  {
    hb_subset_input_destroy (input);
  }

  void parse_face (int argc, const char * const *argv)
  {
    subset_main_t main2;
    main2.add_options ();

    g_option_context_set_ignore_unknown_options (main2.context, true);
    g_option_context_set_help_enabled (main2.context, false);

    char **args = (char **)
#if GLIB_CHECK_VERSION (2, 68, 0)
      g_memdup2
#else
      g_memdup
#endif
      (argv, argc * sizeof (*argv));
    main2.option_parser_t::parse (&argc, &args);
    g_free (args);

    set_face (main2.face);
  }

  void parse (int argc, char **argv)
  {
    bool help = false;
    for (auto i = 1; i < argc; i++)
      if (!strncmp ("--help", argv[i], 6))
      {
	help = true;
	break;
      }

    if (likely (!help))
    {
      /* Do a preliminary parse to load font-face, such that we can use it
       * during main option parsing. */
      parse_face (argc, argv);
    }

    add_options ();
    option_parser_t::parse (&argc, &argv);
  }

  int operator () (int argc, char **argv)
  {
    parse (argc, argv);

    hb_face_t* orig_face = face;
    if (orig_face != cache.face)
    {
      hb_face_destroy (cache.face);
      cache.face = hb_face_reference (orig_face);
      if (cache.face_preprocessed)
      {
        hb_face_destroy (cache.face_preprocessed);
	cache.face_preprocessed = nullptr;
      }
    }

    if (preprocess)
    {
      if (!cache.face_preprocessed)
        cache.face_preprocessed = preprocess_face (cache.face);
      orig_face = cache.face_preprocessed;
    }

    hb_face_t *new_face = nullptr;
    for (unsigned i = 0; i < num_iterations; i++)
    {
      hb_face_destroy (new_face);
      new_face = hb_subset_or_fail (orig_face, input);
    }

    bool success = new_face;
    if (success)
    {
      hb_blob_t *result = hb_face_reference_blob (new_face);
      write_file (output_file, result);
      hb_blob_destroy (result);
    }
    else if (hb_face_get_glyph_count (orig_face) == 0)
      fail (false, "Invalid font file.");

    hb_face_destroy (new_face);

    return success ? 0 : 1;
  }

  bool
  write_file (const char *output_file, hb_blob_t *blob)
  {
    assert (out_fp);

    unsigned int size;
    const char* data = hb_blob_get_data (blob, &size);

    while (size)
    {
      size_t ret = fwrite (data, 1, size, out_fp);
      size -= ret;
      data += ret;
      if (size && ferror (out_fp))
        fail (false, "Failed to write output: %s", strerror (errno));
    }

    return true;
  }

  void add_options ();

  protected:
  static gboolean
  collect_face (const char *name,
		const char *arg,
		gpointer    data,
		GError    **error);
  static gboolean
  collect_rest (const char *name,
		const char *arg,
		gpointer    data,
		GError    **error);

  public:

  unsigned num_iterations = 1;
  gboolean preprocess = false;
  hb_subset_input_t *input = nullptr;

  static struct cache_t
  {
    ~cache_t ()
    {
      hb_face_destroy (face);
      hb_face_destroy (face_preprocessed);
      hb_font_destroy (font);
    }

    hb_face_t *face = nullptr;
    hb_face_t *face_preprocessed = nullptr;
    hb_font_t *font = nullptr;
  } cache;
};

subset_main_t::cache_t subset_main_t::cache {};

static gboolean
parse_gids (const char *name G_GNUC_UNUSED,
	    const char *arg,
	    gpointer    data,
	    GError    **error)
{
  subset_main_t *subset_main = (subset_main_t *) data;
  hb_bool_t is_remove = (name[strlen (name) - 1] == '-');
  hb_bool_t is_add = (name[strlen (name) - 1] == '+');
  hb_set_t *gids = hb_subset_input_glyph_set (subset_main->input);

  if (!is_remove && !is_add) hb_set_clear (gids);

  if (0 == strcmp (arg, "*"))
  {
    hb_set_clear (gids);
    if (!is_remove)
      hb_set_invert (gids);
    return true;
  }

  char *s = (char *) arg;
  char *p;

  while (s && *s)
  {
    while (*s && strchr (", ", *s))
      s++;
    if (!*s)
      break;

    errno = 0;
    hb_codepoint_t start_code = strtoul (s, &p, 10);
    if (s[0] == '-' || errno || s == p)
    {
      g_set_error (error, G_OPTION_ERROR, G_OPTION_ERROR_BAD_VALUE,
		   "Failed parsing glyph-index at: '%s'", s);
      return false;
    }

    if (p && p[0] == '-') // ranges
    {
      s = ++p;
      hb_codepoint_t end_code = strtoul (s, &p, 10);
      if (s[0] == '-' || errno || s == p)
      {
	g_set_error (error, G_OPTION_ERROR, G_OPTION_ERROR_BAD_VALUE,
		     "Failed parsing glyph-index at: '%s'", s);
	return false;
      }

      if (end_code < start_code)
      {
	g_set_error (error, G_OPTION_ERROR, G_OPTION_ERROR_BAD_VALUE,
		     "Invalid glyph-index range %u-%u", start_code, end_code);
	return false;
      }
      if (!is_remove)
        hb_set_add_range (gids, start_code, end_code);
      else
        hb_set_del_range (gids, start_code, end_code);
    }
    else
    {
      if (!is_remove)
        hb_set_add (gids, start_code);
      else
        hb_set_del (gids, start_code);
    }

    s = p;
  }

  return true;
}

static gboolean
parse_glyphs (const char *name,
	      const char *arg,
	      gpointer    data,
	      GError    **error G_GNUC_UNUSED)
{
  subset_main_t *subset_main = (subset_main_t *) data;
  if (!subset_main->face)
  {
    // We are in pre-parsing.
    return true;
  }

  hb_bool_t is_remove = (name[strlen (name) - 1] == '-');
  hb_bool_t is_add = (name[strlen (name) - 1] == '+');
  hb_set_t *gids = hb_subset_input_glyph_set (subset_main->input);

  if (!is_remove && !is_add) hb_set_clear (gids);

  if (0 == strcmp (arg, "*"))
  {
    hb_set_clear (gids);
    if (!is_remove)
      hb_set_invert (gids);
    return true;
  }

  const char *p = arg;
  const char *p_end = arg + strlen (arg);

  if (!subset_main->cache.font)
    subset_main->cache.font = hb_font_create (subset_main->face);
  hb_font_t *font = subset_main->cache.font;

  while (p < p_end)
  {
    while (p < p_end && (*p == ' ' || *p == ','))
      p++;

    const char *end = p;
    while (end < p_end && *end != ' ' && *end != ',')
      end++;

    if (p < end)
    {
      hb_codepoint_t gid;
      if (!hb_font_glyph_from_string (font, p, end - p, &gid))
      {
	g_set_error (error, G_OPTION_ERROR, G_OPTION_ERROR_BAD_VALUE,
		     "Failed parsing glyph name: '%s'", p);
	return false;
      }

      if (!is_remove)
        hb_set_add (gids, gid);
      else
        hb_set_del (gids, gid);
    }

    p = end + 1;
  }

  return true;
}

static gboolean
parse_text (const char *name,
	    const char *arg,
	    gpointer    data,
	    GError    **error G_GNUC_UNUSED)
{
  subset_main_t *subset_main = (subset_main_t *) data;
  hb_bool_t is_remove = (name[strlen (name) - 1] == '-');
  hb_bool_t is_add = (name[strlen (name) - 1] == '+');
  hb_set_t *unicodes = hb_subset_input_unicode_set (subset_main->input);

  if (!is_remove && !is_add) hb_set_clear (unicodes);

  if (0 == strcmp (arg, "*"))
  {
    hb_set_clear (unicodes);
    if (!is_remove)
      hb_set_invert (unicodes);
    return true;
  }

  for (gchar *c = (gchar *) arg;
       *c;
       c = g_utf8_find_next_char(c, nullptr))
  {
    gunichar cp = g_utf8_get_char(c);
    if (!is_remove)
      hb_set_add (unicodes, cp);
    else
      hb_set_del (unicodes, cp);
  }
  return true;
}

static gboolean
parse_unicodes (const char *name,
		const char *arg,
		gpointer    data,
		GError    **error)
{
  subset_main_t *subset_main = (subset_main_t *) data;
  hb_bool_t is_remove = (name[strlen (name) - 1] == '-');
  hb_bool_t is_add = (name[strlen (name) - 1] == '+');
  hb_set_t *unicodes = hb_subset_input_unicode_set (subset_main->input);

  if (!is_remove && !is_add) hb_set_clear (unicodes);

  if (0 == strcmp (arg, "*"))
  {
    hb_set_clear (unicodes);
    if (!is_remove)
      hb_set_invert (unicodes);
    return true;
  }

  // XXX TODO Ranges
#define DELIMITERS "<+->{},;&#\\xXuUnNiI\n\t\v\f\r "

  char *s = (char *) arg;
  char *p;

  while (s && *s)
  {
    while (*s && strchr (DELIMITERS, *s))
      s++;
    if (!*s)
      break;

    errno = 0;
    hb_codepoint_t start_code = strtoul (s, &p, 16);
    if (errno || s == p)
    {
      g_set_error (error, G_OPTION_ERROR, G_OPTION_ERROR_BAD_VALUE,
		   "Failed parsing Unicode at: '%s'", s);
      return false;
    }

    if (p && p[0] == '-') // ranges
    {
      s = ++p;
      hb_codepoint_t end_code = strtoul (s, &p, 16);
      if (s[0] == '-' || errno || s == p)
      {
	g_set_error (error, G_OPTION_ERROR, G_OPTION_ERROR_BAD_VALUE,
		     "Failed parsing Unicode at: '%s'", s);
	return false;
      }

      if (end_code < start_code)
      {
	g_set_error (error, G_OPTION_ERROR, G_OPTION_ERROR_BAD_VALUE,
		     "Invalid Unicode range %u-%u", start_code, end_code);
	return false;
      }
      if (!is_remove)
        hb_set_add_range (unicodes, start_code, end_code);
      else
        hb_set_del_range (unicodes, start_code, end_code);
    }
    else
    {
      if (!is_remove)
        hb_set_add (unicodes, start_code);
      else
        hb_set_del (unicodes, start_code);
    }

    s = p;
  }

  return true;
}

static gboolean
parse_nameids (const char *name,
	       const char *arg,
	       gpointer    data,
	       GError    **error)
{
  subset_main_t *subset_main = (subset_main_t *) data;
  hb_bool_t is_remove = (name[strlen (name) - 1] == '-');
  hb_bool_t is_add = (name[strlen (name) - 1] == '+');
  hb_set_t *name_ids = hb_subset_input_set (subset_main->input, HB_SUBSET_SETS_NAME_ID);


  if (!is_remove && !is_add) hb_set_clear (name_ids);

  if (0 == strcmp (arg, "*"))
  {
    hb_set_clear (name_ids);
    if (!is_remove)
      hb_set_invert (name_ids);
    return true;
  }

  char *s = (char *) arg;
  char *p;

  while (s && *s)
  {
    while (*s && strchr (", ", *s))
      s++;
    if (!*s)
      break;

    errno = 0;
    hb_codepoint_t u = strtoul (s, &p, 10);
    if (errno || s == p)
    {
      g_set_error (error, G_OPTION_ERROR, G_OPTION_ERROR_BAD_VALUE,
		   "Failed parsing nameID at: '%s'", s);
      return false;
    }

    if (!is_remove)
    {
      hb_set_add (name_ids, u);
    } else {
      hb_set_del (name_ids, u);
    }

    s = p;
  }

  return true;
}

static gboolean
parse_name_languages (const char *name,
		      const char *arg,
		      gpointer    data,
		      GError    **error)
{
  subset_main_t *subset_main = (subset_main_t *) data;
  hb_bool_t is_remove = (name[strlen (name) - 1] == '-');
  hb_bool_t is_add = (name[strlen (name) - 1] == '+');
  hb_set_t *name_languages = hb_subset_input_set (subset_main->input, HB_SUBSET_SETS_NAME_LANG_ID);

  if (!is_remove && !is_add) hb_set_clear (name_languages);

  if (0 == strcmp (arg, "*"))
  {
    hb_set_clear (name_languages);
    if (!is_remove)
      hb_set_invert (name_languages);
    return true;
  }

  char *s = (char *) arg;
  char *p;

  while (s && *s)
  {
    while (*s && strchr (", ", *s))
      s++;
    if (!*s)
      break;

    errno = 0;
    hb_codepoint_t u = strtoul (s, &p, 10);
    if (errno || s == p)
    {
      g_set_error (error, G_OPTION_ERROR, G_OPTION_ERROR_BAD_VALUE,
		   "Failed parsing name-language code at: '%s'", s);
      return false;
    }

    if (!is_remove)
    {
      hb_set_add (name_languages, u);
    } else {
      hb_set_del (name_languages, u);
    }

    s = p;
  }

  return true;
}

static gboolean
_keep_everything (const char *name,
		  const char *arg,
		  gpointer    data,
		  GError    **error G_GNUC_UNUSED)
{
  subset_main_t *subset_main = (subset_main_t *) data;

  hb_subset_input_keep_everything (subset_main->input);

  return true;
}

template <hb_subset_flags_t flag>
static gboolean
set_flag (const char *name,
	  const char *arg,
	  gpointer    data,
	  GError    **error G_GNUC_UNUSED)
{
  subset_main_t *subset_main = (subset_main_t *) data;

  hb_subset_input_set_flags (subset_main->input,
			     hb_subset_input_get_flags (subset_main->input) | flag);

  return true;
}

static gboolean
parse_layout_tag_list (hb_subset_sets_t set_type,
                       const char *name,
                       const char *arg,
                       gpointer    data,
                       GError    **error G_GNUC_UNUSED)
{
  subset_main_t *subset_main = (subset_main_t *) data;
  hb_bool_t is_remove = (name[strlen (name) - 1] == '-');
  hb_bool_t is_add = (name[strlen (name) - 1] == '+');
  hb_set_t *layout_tags = hb_subset_input_set (subset_main->input, set_type);

  if (!is_remove && !is_add) hb_set_clear (layout_tags);

  if (0 == strcmp (arg, "*"))
  {
    hb_set_clear (layout_tags);
    if (!is_remove)
      hb_set_invert (layout_tags);
    return true;
  }

  char *s = strtok((char *) arg, ", ");
  while (s)
  {
    if (strlen (s) > 4) // tags are at most 4 bytes
    {
      g_set_error (error, G_OPTION_ERROR, G_OPTION_ERROR_BAD_VALUE,
                   "Failed parsing table tag at: '%s'", s);
      return false;
    }

    hb_tag_t tag = hb_tag_from_string (s, strlen (s));

    if (!is_remove)
      hb_set_add (layout_tags, tag);
    else
      hb_set_del (layout_tags, tag);

    s = strtok(nullptr, ", ");
  }

  return true;
}

static gboolean
parse_layout_features (const char *name,
		       const char *arg,
		       gpointer    data,
		       GError    **error)

{
  return parse_layout_tag_list (HB_SUBSET_SETS_LAYOUT_FEATURE_TAG,
                                name,
                                arg,
                                data,
                                error);
}

static gboolean
parse_layout_scripts (const char *name,
		       const char *arg,
		       gpointer    data,
		       GError    **error)

{
  return parse_layout_tag_list (HB_SUBSET_SETS_LAYOUT_SCRIPT_TAG,
                                name,
                                arg,
                                data,
                                error);
}

static gboolean
parse_drop_tables (const char *name,
		   const char *arg,
		   gpointer    data,
		   GError    **error)
{
  subset_main_t *subset_main = (subset_main_t *) data;
  hb_bool_t is_remove = (name[strlen (name) - 1] == '-');
  hb_bool_t is_add = (name[strlen (name) - 1] == '+');
  hb_set_t *drop_tables = hb_subset_input_set (subset_main->input, HB_SUBSET_SETS_DROP_TABLE_TAG);

  if (!is_remove && !is_add) hb_set_clear (drop_tables);

  if (0 == strcmp (arg, "*"))
  {
    hb_set_clear (drop_tables);
    if (!is_remove)
      hb_set_invert (drop_tables);
    return true;
  }

  char *s = strtok((char *) arg, ", ");
  while (s)
  {
    if (strlen (s) > 4) // Table tags are at most 4 bytes.
    {
      g_set_error (error, G_OPTION_ERROR, G_OPTION_ERROR_BAD_VALUE,
		   "Failed parsing table tag at: '%s'", s);
      return false;
    }

    hb_tag_t tag = hb_tag_from_string (s, strlen (s));

    if (!is_remove)
      hb_set_add (drop_tables, tag);
    else
      hb_set_del (drop_tables, tag);

    s = strtok(nullptr, ", ");
  }

  return true;
}

#ifndef HB_NO_VAR

static gboolean
parse_instance (const char *name,
		const char *arg,
		gpointer    data,
		GError    **error)
{
  subset_main_t *subset_main = (subset_main_t *) data;
  if (!subset_main->face)
  {
    // We are in pre-parsing.
    return true;
  }

  return parse_instancing_spec(arg, subset_main->face, subset_main->input, error);
}
#endif

static gboolean
parse_glyph_map (const char *name,
		 const char *arg,
		 gpointer    data,
		 GError    **error)
{
  // Glyph map has the following format:
  // <entry 1>,<entry 2>,...,<entry n>
  // <entry> = <old gid>:<new gid>
  subset_main_t *subset_main = (subset_main_t *) data;
  hb_subset_input_t* input = subset_main->input;
  hb_set_t *glyphs = hb_subset_input_glyph_set(input);

  char *s = (char *) arg;
  char *p;

  while (s && *s)
  {
    while (*s && strchr (", ", *s))
      s++;
    if (!*s)
      break;

    errno = 0;
    hb_codepoint_t start_code = strtoul (s, &p, 10);
    if (s[0] == '-' || errno || s == p)
    {
      g_set_error (error, G_OPTION_ERROR, G_OPTION_ERROR_BAD_VALUE,
		   "Failed parsing glyph map at: '%s'", s);
      return false;
    }

    if (!p || p[0] != ':') // ranges
    {
      g_set_error (error, G_OPTION_ERROR, G_OPTION_ERROR_BAD_VALUE,
		   "Failed parsing glyph map at: '%s'", s);
      return false;
    }

    s = ++p;
    hb_codepoint_t end_code = strtoul (s, &p, 10);
    if (s[0] == '-' || errno || s == p)
    {
      g_set_error (error, G_OPTION_ERROR, G_OPTION_ERROR_BAD_VALUE,
		"Failed parsing glyph map at: '%s'", s);
      return false;
    }

    hb_set_add(glyphs, start_code);
    hb_map_set (hb_subset_input_old_to_new_glyph_mapping (input), start_code, end_code);

    s = p;
  }

  return true;
}

template <GOptionArgFunc line_parser, bool allow_comments=true>
static gboolean
parse_file_for (const char *name,
		const char *arg,
		gpointer    data,
		GError    **error)
{
  FILE *fp = nullptr;
  if (0 != strcmp (arg, "-"))
    fp = fopen (arg, "r");
  else
    fp = stdin;

  if (!fp)
  {
    g_set_error (error, G_OPTION_ERROR, G_OPTION_ERROR_FAILED,
		 "Failed opening file `%s': %s",
		 arg, strerror (errno));
    return false;
  }

  GString *gs = g_string_new (nullptr);
  do
  {
    g_string_set_size (gs, 0);
    char buf[BUFSIZ];
    while (fgets (buf, sizeof (buf), fp))
    {
      unsigned bytes = strlen (buf);
      if (bytes && buf[bytes - 1] == '\n')
      {
	bytes--;
	g_string_append_len (gs, buf, bytes);
	break;
      }
      g_string_append_len (gs, buf, bytes);
    }
    if (ferror (fp))
    {
      g_set_error (error, G_OPTION_ERROR, G_OPTION_ERROR_FAILED,
		   "Failed reading file `%s': %s",
		   arg, strerror (errno));
      fclose (fp);
      return false;
    }
    g_string_append_c (gs, '\0');

    if (allow_comments)
    {
      char *comment = strchr (gs->str, '#');
      if (comment)
        *comment = '\0';
    }

    line_parser ("+", gs->str, data, error);

    if (*error)
      break;
  }
  while (!feof (fp));

  g_string_free (gs, true);

  fclose (fp);

  return true;
}

gboolean
subset_main_t::collect_face (const char *name,
			     const char *arg,
			     gpointer    data,
			     GError    **error)
{
  face_options_t *thiz = (face_options_t *) data;

  if (!thiz->font_file)
  {
    thiz->font_file = g_strdup (arg);
    return true;
  }

  return true;
}

gboolean
subset_main_t::collect_rest (const char *name,
			     const char *arg,
			     gpointer    data,
			     GError    **error)
{
  subset_main_t *thiz = (subset_main_t *) data;

  if (!thiz->font_file)
  {
    thiz->font_file = g_strdup (arg);
    return true;
  }

  parse_text (name, arg, data, error);
  return true;
}

void
subset_main_t::add_options ()
{
  set_summary ("Subset font to specification.");
  set_description ("Subsets font file to a specified set of glyphs, Unicode codepoints, or text, design-space limiting, and other reductions.");

  face_options_t::add_options (this);

  GOptionEntry glyphset_entries[] =
  {
    {"gids",		'g', 0, G_OPTION_ARG_CALLBACK, (gpointer) &parse_gids,
     "Specify glyph IDs or ranges to include in the subset.\n"
     "                                                        "
     "Use --gids-=... to subtract codepoints from the current set.", "list of glyph indices/ranges or *"},
    {"gids-",		0, G_OPTION_FLAG_HIDDEN, G_OPTION_ARG_CALLBACK, (gpointer) &parse_gids,			"Specify glyph IDs or ranges to remove from the subset", "list of glyph indices/ranges or *"},
    {"gids+",		0, G_OPTION_FLAG_HIDDEN, G_OPTION_ARG_CALLBACK, (gpointer) &parse_gids,			"Specify glyph IDs or ranges to include in the subset", "list of glyph indices/ranges or *"},
    {"gids-file",	0, 0, G_OPTION_ARG_CALLBACK, (gpointer) &parse_file_for<parse_gids>,	"Specify file to read glyph IDs or ranges from", "filename"},
    {"glyphs",		0, 0, G_OPTION_ARG_CALLBACK, (gpointer) &parse_glyphs,			"Specify glyph names to include in the subset. Use --glyphs-=... to subtract glyphs from the current set.", "list of glyph names or *"},
    {"glyphs+",		0, G_OPTION_FLAG_HIDDEN, G_OPTION_ARG_CALLBACK, (gpointer) &parse_glyphs,			"Specify glyph names to include in the subset", "list of glyph names"},
    {"glyphs-",		0, G_OPTION_FLAG_HIDDEN, G_OPTION_ARG_CALLBACK, (gpointer) &parse_glyphs,			"Specify glyph names to remove from the subset", "list of glyph names"},


    {"glyphs-file",	0, 0, G_OPTION_ARG_CALLBACK, (gpointer) &parse_file_for<parse_glyphs>,	"Specify file to read glyph names from", "filename"},

    {"text",		't', 0, G_OPTION_ARG_CALLBACK, (gpointer) &parse_text,			"Specify text to include in the subset. Use --text-=... to subtract codepoints from the current set.", "string"},
    {"text-",		0, G_OPTION_FLAG_HIDDEN, G_OPTION_ARG_CALLBACK, (gpointer) &parse_text,			"Specify text to remove from the subset", "string"},
    {"text+",		0, G_OPTION_FLAG_HIDDEN, G_OPTION_ARG_CALLBACK, (gpointer) &parse_text,			"Specify text to include in the subset", "string"},


    {"text-file",	0, 0, G_OPTION_ARG_CALLBACK, (gpointer) &parse_file_for<parse_text, false>,"Specify file to read text from", "filename"},
    {"unicodes",	'u', 0, G_OPTION_ARG_CALLBACK, (gpointer) &parse_unicodes,
     "Specify Unicode codepoints or ranges to include in the subset. Use * to include all codepoints.\n"
     "                                                        "
     "--unicodes-=... can be used to subtract codepoints from the current set.\n"
     "                                                        "
     "For example: --unicodes=* --unicodes-=41,42,43 would create a subset with all codepoints\n"
     "                                                        "
     "except for 41, 42, 43.",
     "list of hex numbers/ranges or *"},
    {"unicodes-",	0, G_OPTION_FLAG_HIDDEN, G_OPTION_ARG_CALLBACK, (gpointer) &parse_unicodes, "Specify Unicode codepoints or ranges to remove from the subset", "list of hex numbers/ranges or *"},
    {"unicodes+",	0, G_OPTION_FLAG_HIDDEN, G_OPTION_ARG_CALLBACK, (gpointer) &parse_unicodes, "Specify Unicode codepoints or ranges to include in the subset", "list of hex numbers/ranges or *"},

    {"unicodes-file",	0, 0, G_OPTION_ARG_CALLBACK, (gpointer) &parse_file_for<parse_unicodes>,"Specify file to read Unicode codepoints or ranges from", "filename"},
    {nullptr}
  };
  add_group (glyphset_entries,
	     "subset-glyphset",
	     "Subset glyph-set option:",
	     "Subsetting glyph-set options",
	     this);

  GOptionEntry other_entries[] =
  {
    {"gid-map", 0, 0, G_OPTION_ARG_CALLBACK, (gpointer) &parse_glyph_map, "Specify a glyph mapping to use, any unmapped gids will be automatically assigned.", "List of pairs old_gid1:new_gid1,old_gid2:new_gid2,..."},
    {"name-IDs",	0, 0, G_OPTION_ARG_CALLBACK, (gpointer) &parse_nameids,		"Subset specified nameids. Use --name-IDs-=... to subtract from the current set.", "list of int numbers or *"},
    {"name-IDs-",	0, G_OPTION_FLAG_HIDDEN, G_OPTION_ARG_CALLBACK, (gpointer) &parse_nameids,		"Subset specified nameids", "list of int numbers or *"},
    {"name-IDs+",	0, G_OPTION_FLAG_HIDDEN, G_OPTION_ARG_CALLBACK, (gpointer) &parse_nameids,		"Subset specified nameids", "list of int numbers or *"},
    {"name-languages",	0, 0, G_OPTION_ARG_CALLBACK, (gpointer) &parse_name_languages,	"Subset nameRecords with specified language IDs. Use --name-languages-=... to subtract from the current set.", "list of int numbers or *"},
    {"name-languages-",	0, G_OPTION_FLAG_HIDDEN, G_OPTION_ARG_CALLBACK, (gpointer) &parse_name_languages,	"Subset nameRecords with specified language IDs", "list of int numbers or *"},
    {"name-languages+",	0, G_OPTION_FLAG_HIDDEN, G_OPTION_ARG_CALLBACK, (gpointer) &parse_name_languages,	"Subset nameRecords with specified language IDs", "list of int numbers or *"},

    {"layout-features",	0, 0, G_OPTION_ARG_CALLBACK, (gpointer) &parse_layout_features,	"Specify set of layout feature tags that will be preserved. Use --layout-features-=... to subtract from the current set.", "list of string table tags or *"},
    {"layout-features+",0, G_OPTION_FLAG_HIDDEN, G_OPTION_ARG_CALLBACK, (gpointer) &parse_layout_features,	"Specify set of layout feature tags that will be preserved", "list of string tags or *"},
    {"layout-features-",0, G_OPTION_FLAG_HIDDEN, G_OPTION_ARG_CALLBACK, (gpointer) &parse_layout_features,	"Specify set of layout feature tags that will be preserved", "list of string tags or *"},

    {"layout-scripts",	0, 0, G_OPTION_ARG_CALLBACK, (gpointer) &parse_layout_scripts,	"Specify set of layout script tags that will be preserved. Use --layout-scripts-=... to subtract from the current set.", "list of string table tags or *"},
    {"layout-scripts+",0, G_OPTION_FLAG_HIDDEN, G_OPTION_ARG_CALLBACK, (gpointer) &parse_layout_scripts,	"Specify set of layout script tags that will be preserved", "list of string tags or *"},
    {"layout-scripts-",0, G_OPTION_FLAG_HIDDEN, G_OPTION_ARG_CALLBACK, (gpointer) &parse_layout_scripts,	"Specify set of layout script tags that will be preserved", "list of string tags or *"},

    {"drop-tables",	0, 0, G_OPTION_ARG_CALLBACK, (gpointer) &parse_drop_tables,	"Drop the specified tables. Use --drop-tables-=... to subtract from the current set.", "list of string table tags or *"},
    {"drop-tables+",	0, G_OPTION_FLAG_HIDDEN, G_OPTION_ARG_CALLBACK, (gpointer) &parse_drop_tables,	"Drop the specified tables.", "list of string table tags or *"},
    {"drop-tables-",	0, G_OPTION_FLAG_HIDDEN, G_OPTION_ARG_CALLBACK, (gpointer) &parse_drop_tables,	"Drop the specified tables.", "list of string table tags or *"},
#ifndef HB_NO_VAR
     {"variations",	0, 0, G_OPTION_ARG_CALLBACK, (gpointer) &parse_instance,
     "(Partially|Fully) Instantiate a variable font. A location consists of the tag "
     "of a variation axis, followed by '=', followed by a number or the literal "
     "string 'drop'. For example: --variations=\"wdth=100 wght=200\" or --variations=\"wdth=drop\""
     ,
     "list of comma separated axis-locations."
     },
     {"instance",	0, G_OPTION_FLAG_HIDDEN, G_OPTION_ARG_CALLBACK, (gpointer) &parse_instance,
     "Alias for --variations.", "list of comma separated axis-locations"},
#endif
    {nullptr}
  };
  add_group (other_entries,
	     "subset-other",
	     "Subset other option:",
	     "Subsetting other options",
	     this);

  GOptionEntry flag_entries[] =
  {
    {"keep-everything",		0, G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK, (gpointer) &_keep_everything,
     "Keep everything by default. Options after this can override the operation.", nullptr},
    {"no-hinting",		0, G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK, (gpointer) &set_flag<HB_SUBSET_FLAGS_NO_HINTING>,		"Whether to drop hints", nullptr},
    {"retain-gids",		0, G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK, (gpointer) &set_flag<HB_SUBSET_FLAGS_RETAIN_GIDS>,		"If set don't renumber glyph ids in the subset.", nullptr},
    {"desubroutinize",		0, G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK, (gpointer) &set_flag<HB_SUBSET_FLAGS_DESUBROUTINIZE>,		"Remove CFF/CFF2 use of subroutines", nullptr},
    {"name-legacy",		0, G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK, (gpointer) &set_flag<HB_SUBSET_FLAGS_NAME_LEGACY>,		"Keep legacy (non-Unicode) 'name' table entries", nullptr},
    {"set-overlaps-flag",	0, G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK, (gpointer) &set_flag<HB_SUBSET_FLAGS_SET_OVERLAPS_FLAG>,	"Set the overlaps flag on each glyph.", nullptr},
    {"notdef-outline",		0, G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK, (gpointer) &set_flag<HB_SUBSET_FLAGS_NOTDEF_OUTLINE>,		"Keep the outline of \'.notdef\' glyph", nullptr},
    {"no-prune-unicode-ranges",	0, G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK, (gpointer) &set_flag<HB_SUBSET_FLAGS_NO_PRUNE_UNICODE_RANGES>,	"Don't change the 'OS/2 ulUnicodeRange*' bits.", nullptr},
    {"no-layout-closure",	0, G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK, (gpointer) &set_flag<HB_SUBSET_FLAGS_NO_LAYOUT_CLOSURE>,	"Don't perform glyph closure for layout substitution (GSUB).", nullptr},
    {"no-bidi-closure",	        0, G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK, (gpointer) &set_flag<HB_SUBSET_FLAGS_NO_BIDI_CLOSURE>,	"Don't perform bidi closure (adding mirrored variants) for input codepoints.", nullptr},
    {"glyph-names",		0, G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK, (gpointer) &set_flag<HB_SUBSET_FLAGS_GLYPH_NAMES>,		"Keep PS glyph names in TT-flavored fonts. ", nullptr},
    {"passthrough-tables",	0, G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK, (gpointer) &set_flag<HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED>,	"Do not drop tables that the tool does not know how to subset.", nullptr},
    {"preprocess-face",		0, G_OPTION_FLAG_HIDDEN, G_OPTION_ARG_NONE, &this->preprocess,
     "Alternative name for --preprocess.", nullptr},
    {"preprocess",		0, 0, G_OPTION_ARG_NONE, &this->preprocess,
     "If set preprocesses the face with the add accelerator option before actually subsetting.", nullptr},
#ifdef HB_EXPERIMENTAL_API
    {"iftb-requirements",	0, G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK, (gpointer) &set_flag<HB_SUBSET_FLAGS_IFTB_REQUIREMENTS>,	"Enforce requirements needed to use the subset with incremental font transfer IFTB patches.", nullptr},
#endif
    {"optimize",		0, G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK, (gpointer) &set_flag<HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS>,	"Perform IUP delta optimization on the resulting gvar table's deltas", nullptr},
    {nullptr}
  };
  add_group (flag_entries,
	     "subset-flags",
	     "Subset boolean option:",
	     "Subsetting boolean options",
	     this);

  GOptionEntry app_entries[] =
  {
    {"num-iterations",	'n', G_OPTION_FLAG_IN_MAIN, G_OPTION_ARG_INT,
     &this->num_iterations,
     "Run subsetter N times (default: 1)", "N"},
    {nullptr}
  };
  add_group (app_entries,
	     "subset-app",
	     "Subset app option:",
	     "Subsetting application options",
	     this);

  output_options_t::add_options (this);

  GOptionEntry entries[] =
  {
    {G_OPTION_REMAINING,	0, G_OPTION_FLAG_IN_MAIN,
			      G_OPTION_ARG_CALLBACK,	(gpointer) &collect_rest,	nullptr,	"[FONT-FILE] [TEXT]"},
    {nullptr}
  };
  add_main_group (entries, this);
  option_parser_t::add_options ();
}


#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      hb_subset_main(cnt, arr)
#endif

int main(int argc, const char** argv)
{
  return batch_main<subset_main_t, true> (argc, argv);
}

#else


#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      hb_subset_main(cnt, arr)
#endif

int main(int argc, const char** argv)
{
  fprintf(stderr, "hb-subset utility is not supported in this non-GNU-Glib build.\n");
  return EXIT_FAILURE;
}

#endif
