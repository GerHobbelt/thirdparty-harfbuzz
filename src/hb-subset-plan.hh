/*
 * Copyright © 2018  Google, Inc.
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
 * Google Author(s): Garret Rieger, Roderick Sheeter
 */

#ifndef HB_SUBSET_PLAN_HH
#define HB_SUBSET_PLAN_HH

#include "hb.hh"

#include "hb-subset.h"
#include "hb-subset-input.hh"
#include "hb-subset-accelerator.hh"

#include "hb-map.hh"
#include "hb-bimap.hh"
#include "hb-set.hh"

namespace OT {
struct Feature;
}

struct hb_subset_plan_t
{
  hb_subset_plan_t ()
  {}

  ~hb_subset_plan_t()
  {
    hb_set_destroy (unicodes);
    hb_set_destroy (name_ids);
    hb_set_destroy (name_languages);
    hb_set_destroy (layout_features);
    hb_set_destroy (layout_scripts);
    hb_set_destroy (glyphs_requested);
    hb_set_destroy (drop_tables);
    hb_set_destroy (no_subset_tables);
    hb_face_destroy (source);
    hb_face_destroy (dest);
    hb_map_destroy (codepoint_to_glyph);
    hb_map_destroy (glyph_map);
    hb_map_destroy (reverse_glyph_map);
    hb_map_destroy (glyph_map_gsub);
    hb_set_destroy (_glyphset);
    hb_set_destroy (_glyphset_gsub);
    hb_set_destroy (_glyphset_mathed);
    hb_set_destroy (_glyphset_colred);
    hb_map_destroy (gsub_lookups);
    hb_map_destroy (gpos_lookups);
    hb_map_destroy (gsub_features);
    hb_map_destroy (gpos_features);
    hb_map_destroy (colrv1_layers);
    hb_map_destroy (colr_palettes);
    hb_map_destroy (axes_index_map);
    hb_map_destroy (axes_old_index_tag_map);

    hb_hashmap_destroy (gsub_langsys);
    hb_hashmap_destroy (gpos_langsys);
    hb_hashmap_destroy (gsub_feature_record_cond_idx_map);
    hb_hashmap_destroy (gpos_feature_record_cond_idx_map);
    hb_hashmap_destroy (gsub_feature_substitutes_map);
    hb_hashmap_destroy (gpos_feature_substitutes_map);
    hb_hashmap_destroy (axes_location);
    hb_hashmap_destroy (sanitized_table_cache);
    hb_hashmap_destroy (hmtx_map);
    hb_hashmap_destroy (vmtx_map);
    hb_hashmap_destroy (layout_variation_idx_delta_map);

#ifdef HB_EXPERIMENTAL_API
    if (name_table_overrides)
    {
      for (auto _ : name_table_overrides->values ())
        _.fini ();
    }
    hb_hashmap_destroy (name_table_overrides);
#endif

    if (user_axes_location)
    {
      hb_object_destroy (user_axes_location);
      hb_free (user_axes_location);
    }
  }

  hb_object_header_t header;

  bool successful;
  unsigned flags;
  bool attach_accelerator_data = false;

  // For each cp that we'd like to retain maps to the corresponding gid.
  hb_set_t *unicodes;
  hb_vector_t<hb_pair_t<hb_codepoint_t, hb_codepoint_t>> unicode_to_new_gid_list;

  // name_ids we would like to retain
  hb_set_t *name_ids;

  // name_languages we would like to retain
  hb_set_t *name_languages;

  //layout features which will be preserved
  hb_set_t *layout_features;

  // layout scripts which will be preserved.
  hb_set_t *layout_scripts;

  //glyph ids requested to retain
  hb_set_t *glyphs_requested;

  // Tables which should not be processed, just pass them through.
  hb_set_t *no_subset_tables;

  // Tables which should be dropped.
  hb_set_t *drop_tables;

  // The glyph subset
  hb_map_t *codepoint_to_glyph;

  // Old -> New glyph id mapping
  hb_map_t *glyph_map;
  hb_map_t *reverse_glyph_map;
  hb_map_t *glyph_map_gsub;

  // Plan is only good for a specific source/dest so keep them with it
  hb_face_t *source;
  hb_face_t *dest;

  unsigned int _num_output_glyphs;
  hb_set_t *_glyphset;
  hb_set_t *_glyphset_gsub;
  hb_set_t *_glyphset_mathed;
  hb_set_t *_glyphset_colred;

  //active lookups we'd like to retain
  hb_map_t *gsub_lookups;
  hb_map_t *gpos_lookups;

  //active langsys we'd like to retain
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *gsub_langsys;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *gpos_langsys;

  //active features after removing redundant langsys and prune_features
  hb_map_t *gsub_features;
  hb_map_t *gpos_features;

  //active feature variation records/condition index with variations
  hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *gsub_feature_record_cond_idx_map;
  hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *gpos_feature_record_cond_idx_map;

  //feature index-> address of substituation feature table mapping with
  //variations
  hb_hashmap_t<unsigned, const OT::Feature*> *gsub_feature_substitutes_map;
  hb_hashmap_t<unsigned, const OT::Feature*> *gpos_feature_substitutes_map;

  //active layers/palettes we'd like to retain
  hb_map_t *colrv1_layers;
  hb_map_t *colr_palettes;

  //Old layout item variation index -> (New varidx, delta) mapping
  hb_hashmap_t<unsigned, hb_pair_t<unsigned, int>> *layout_variation_idx_delta_map;

  //gdef varstore retained varidx mapping
  hb_vector_t<hb_inc_bimap_t> gdef_varstore_inner_maps;

  hb_hashmap_t<hb_tag_t, hb::unique_ptr<hb_blob_t>>* sanitized_table_cache;
  //normalized axes location map
  hb_hashmap_t<hb_tag_t, int> *axes_location;
  //user specified axes location map
  hb_hashmap_t<hb_tag_t, float> *user_axes_location;
  //retained old axis index -> new axis index mapping in fvar axis array
  hb_map_t *axes_index_map;
  //axis_index->axis_tag mapping in fvar axis array
  hb_map_t *axes_old_index_tag_map;
  bool all_axes_pinned;
  bool pinned_at_default;
  bool has_seac;

  //hmtx metrics map: new gid->(advance, lsb)
  hb_hashmap_t<unsigned, hb_pair_t<unsigned, int>> *hmtx_map;
  //vmtx metrics map: new gid->(advance, lsb)
  hb_hashmap_t<unsigned, hb_pair_t<unsigned, int>> *vmtx_map;

#ifdef HB_EXPERIMENTAL_API
  // name table overrides map: name_id->name string new value or None
  // to indicate should remove
  hb_hashmap_t<unsigned, hb_bytes_t> *name_table_overrides;
#endif

  const hb_subset_accelerator_t* accelerator;

 public:

  template<typename T>
  hb_blob_ptr_t<T> source_table()
  {
    if (sanitized_table_cache
        && !sanitized_table_cache->in_error ()
        && sanitized_table_cache->has (T::tableTag)) {
      return hb_blob_reference (sanitized_table_cache->get (T::tableTag).get ());
    }

    hb::unique_ptr<hb_blob_t> table_blob {hb_sanitize_context_t ().reference_table<T> (source)};
    hb_blob_t* ret = hb_blob_reference (table_blob.get ());

    if (likely (sanitized_table_cache))
      sanitized_table_cache->set (T::tableTag,
                                  std::move (table_blob));

    return ret;
  }

  bool in_error () const
  {
    return !successful
        || unicodes->in_error ()
        || name_ids->in_error ()
        || name_languages->in_error ()
        || glyphs_requested->in_error ()
        || drop_tables->in_error ()
        || codepoint_to_glyph->in_error ()
        || glyph_map->in_error ()
        || reverse_glyph_map->in_error ()
        || _glyphset->in_error ()
        || _glyphset_gsub->in_error ()
        || gsub_lookups->in_error ()
        || gpos_lookups->in_error ()
        || gsub_features->in_error ()
        || gpos_features->in_error ()
        || layout_variation_idx_delta_map->in_error ()
        || gdef_varstore_inner_maps.in_error ()
        || sanitized_table_cache->in_error ()
        || axes_location->in_error ()
        || user_axes_location->in_error ()
        || hmtx_map->in_error ()
        || vmtx_map->in_error ()
        ;
  }

  bool check_success(bool success)
  {
    successful = (successful && success);
    return successful;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   * Does NOT include ids kept due to retain_gids. You probably want to use
   * glyph_map/reverse_glyph_map.
   */
  inline const hb_set_t *
  glyphset () const
  {
    return _glyphset;
  }

  /*
   * The set of input glyph ids which will be retained in the subset.
   */
  inline const hb_set_t *
  glyphset_gsub () const
  {
    return _glyphset_gsub;
  }

  /*
   * The total number of output glyphs in the final subset.
   */
  inline unsigned int
  num_output_glyphs () const
  {
    return _num_output_glyphs;
  }

  /*
   * Given an output gid , returns true if that glyph id is an empty
   * glyph (ie. it's a gid that we are dropping all data for).
   */
  inline bool is_empty_glyph (hb_codepoint_t gid) const
  {
    return !_glyphset->has (gid);
  }

  inline bool new_gid_for_codepoint (hb_codepoint_t codepoint,
                                     hb_codepoint_t *new_gid) const
  {
    hb_codepoint_t old_gid = codepoint_to_glyph->get (codepoint);
    if (old_gid == HB_MAP_VALUE_INVALID)
      return false;

    return new_gid_for_old_gid (old_gid, new_gid);
  }

  inline bool new_gid_for_old_gid (hb_codepoint_t old_gid,
                                   hb_codepoint_t *new_gid) const
  {
    hb_codepoint_t gid = glyph_map->get (old_gid);
    if (gid == HB_MAP_VALUE_INVALID)
      return false;

    *new_gid = gid;
    return true;
  }

  inline bool old_gid_for_new_gid (hb_codepoint_t  new_gid,
                                   hb_codepoint_t *old_gid) const
  {
    hb_codepoint_t gid = reverse_glyph_map->get (new_gid);
    if (gid == HB_MAP_VALUE_INVALID)
      return false;

    *old_gid = gid;
    return true;
  }

  inline bool
  add_table (hb_tag_t tag,
             hb_blob_t *contents)
  {
    if (HB_DEBUG_SUBSET)
    {
      hb_blob_t *source_blob = source->reference_table (tag);
      DEBUG_MSG(SUBSET, nullptr, "add table %c%c%c%c, dest %d bytes, source %d bytes",
                HB_UNTAG(tag),
                hb_blob_get_length (contents),
                hb_blob_get_length (source_blob));
      hb_blob_destroy (source_blob);
    }
    return hb_face_builder_add_table (dest, tag, contents);
  }
};

#endif /* HB_SUBSET_PLAN_HH */
