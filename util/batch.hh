/*
 * Copyright © 2021  Behdad Esfahbod
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
 */

#ifndef BATCH_HH
#define BATCH_HH

#include "options.hh"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#ifndef MAX
#define MAX(a, b)  ((a) > (b) ? (a) : (b))
#endif

typedef int (*main_func_t) (int argc, const char **argv);

template <typename main_t, bool report_status=false>
int
batch_main (int argc, const char **argv)
{
  if (argc == 2 && !strcmp (argv[1], "--batch"))
  {
    int ret = 0;
    std::string line;

    // Read line by line from stdin until EOF
    while (std::getline(std::cin, line))
    {
      // Windows CRLF line-endings MAY produce lines with a lingering CR at the end, so we better discard that one before continuing...
      if (!line.empty() && line.back() == '\r')
      {
	line.pop_back();
      }

      std::vector<std::string> tokens;
      std::string token;
      std::istringstream tokenStream(line);

      // set argv[0] for this command line:
      tokens.push_back(argv[0]);

      // chop the line into arguments a.k.a. 'tokens':
      while (std::getline(tokenStream, token, ';'))
      {
	tokens.push_back(token);
      }

      // Handle trailing semicolon (e.g., "a;b;" -> {"a", "b", ""})
      if (!line.empty() && line.back() == ';')
      {
	tokens.push_back("");
      }

      // now that the arg set has been collected, produce it in argv[] compatible form:

      std::vector<const char *> c_strings;
      c_strings.reserve(tokens.size() + 1);  // reckon with the NULL sentinel!

      for (const auto &str : tokens)
      {
	c_strings.push_back(str.c_str());
      }
      c_strings.push_back(nullptr);

      int result = main_t() (tokens.size(), c_strings);

      if (report_status)
	fprintf(stdout, result == 0 ? "success\n" : "failure\n");
      fflush(stdout);

      ret = MAX(ret, result);
    }
    return ret;
  }

  int ret = main_t() (argc, argv);
  if (report_status && ret != 0)
    fprintf (stdout, "error: Operation failed. Probably a bug. File github issue.\n");
  return ret;
}

#endif
