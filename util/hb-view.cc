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
 * Google Author(s): Behdad Esfahbod
 */

#include "batch.hh"
#include "options.hh"

#ifdef HAVE_GLIB_H

#include "font-options.hh"
#include "main-font-text.hh"
#include "shape-consumer.hh"
#include "text-options.hh"
#include "view-cairo.hh"

static const unsigned DEFAULT_FONT_SIZE = 256;
static const unsigned SUBPIXEL_BITS = 6;


#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      hb_view_main(cnt, arr)
#endif

int
main (int argc, char **argv)
{
  using main_t = main_font_text_t<shape_consumer_t<view_cairo_t>, font_options_t, shape_text_options_t>;
  return batch_main<main_t, true> (argc, argv);
}

#else


#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      hb_view_main(cnt, arr)
#endif

int main(int argc, const char** argv)
{
  fprintf(stderr, "hb_view utility is not supported in this non-GNU-Glib build.\n");
  return EXIT_FAILURE;
}

#endif
