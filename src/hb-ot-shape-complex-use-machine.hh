
#line 1 "hb-ot-shape-complex-use-machine.rl"
/*
 * Copyright © 2015  Mozilla Foundation.
 * Copyright © 2015  Google, Inc.
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
 * Mozilla Author(s): Jonathan Kew
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_OT_SHAPE_COMPLEX_USE_MACHINE_HH
#define HB_OT_SHAPE_COMPLEX_USE_MACHINE_HH

#include "hb.hh"


#line 38 "hb-ot-shape-complex-use-machine.hh"
static const unsigned char _use_syllable_machine_trans_keys[] = {
	12u, 48u, 1u, 15u, 1u, 1u, 12u, 48u, 1u, 1u, 0u, 51u, 21u, 21u, 11u, 48u, 
	11u, 48u, 1u, 15u, 1u, 1u, 11u, 48u, 22u, 48u, 23u, 48u, 24u, 47u, 25u, 47u, 
	26u, 47u, 45u, 46u, 46u, 46u, 24u, 48u, 24u, 48u, 24u, 48u, 1u, 1u, 24u, 48u, 
	23u, 48u, 23u, 48u, 23u, 48u, 22u, 48u, 22u, 48u, 22u, 48u, 11u, 48u, 1u, 48u, 
	11u, 48u, 13u, 21u, 4u, 4u, 13u, 13u, 11u, 48u, 11u, 48u, 41u, 42u, 42u, 42u, 
	11u, 48u, 11u, 48u, 22u, 48u, 23u, 48u, 24u, 47u, 25u, 47u, 26u, 47u, 45u, 46u, 
	46u, 46u, 24u, 48u, 24u, 48u, 24u, 48u, 24u, 48u, 23u, 48u, 23u, 48u, 23u, 48u, 
	22u, 48u, 22u, 48u, 22u, 48u, 11u, 48u, 1u, 48u, 1u, 15u, 4u, 4u, 13u, 21u, 
	13u, 13u, 12u, 48u, 1u, 48u, 11u, 48u, 41u, 42u, 42u, 42u, 21u, 42u, 1u, 5u, 
	50u, 52u, 49u, 52u, 49u, 51u, 0
};

static const char _use_syllable_machine_key_spans[] = {
	37, 15, 1, 37, 1, 52, 1, 38, 
	38, 15, 1, 38, 27, 26, 24, 23, 
	22, 2, 1, 25, 25, 25, 1, 25, 
	26, 26, 26, 27, 27, 27, 38, 48, 
	38, 9, 1, 1, 38, 38, 2, 1, 
	38, 38, 27, 26, 24, 23, 22, 2, 
	1, 25, 25, 25, 25, 26, 26, 26, 
	27, 27, 27, 38, 48, 15, 1, 9, 
	1, 37, 48, 38, 2, 1, 22, 5, 
	3, 4, 3
};

static const short _use_syllable_machine_index_offsets[] = {
	0, 38, 54, 56, 94, 96, 149, 151, 
	190, 229, 245, 247, 286, 314, 341, 366, 
	390, 413, 416, 418, 444, 470, 496, 498, 
	524, 551, 578, 605, 633, 661, 689, 728, 
	777, 816, 826, 828, 830, 869, 908, 911, 
	913, 952, 991, 1019, 1046, 1071, 1095, 1118, 
	1121, 1123, 1149, 1175, 1201, 1227, 1254, 1281, 
	1308, 1336, 1364, 1392, 1431, 1480, 1496, 1498, 
	1508, 1510, 1548, 1597, 1636, 1639, 1641, 1664, 
	1670, 1674, 1679
};

static const char _use_syllable_machine_indicies[] = {
	1, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	1, 0, 0, 0, 1, 0, 3, 2, 
	2, 2, 2, 2, 2, 2, 2, 2, 
	2, 2, 2, 2, 4, 2, 3, 2, 
	6, 5, 5, 5, 5, 5, 5, 5, 
	5, 5, 5, 5, 5, 5, 5, 5, 
	5, 5, 5, 5, 5, 5, 5, 5, 
	5, 5, 5, 5, 5, 5, 5, 5, 
	6, 5, 5, 5, 6, 5, 7, 5, 
	8, 9, 10, 8, 11, 12, 10, 10, 
	10, 10, 10, 3, 13, 14, 10, 15, 
	8, 8, 16, 17, 10, 10, 18, 19, 
	20, 21, 22, 23, 24, 18, 25, 26, 
	27, 28, 29, 30, 10, 31, 32, 33, 
	10, 34, 35, 36, 37, 38, 39, 40, 
	13, 41, 10, 42, 10, 44, 43, 46, 
	1, 45, 45, 47, 45, 45, 45, 45, 
	45, 48, 49, 50, 51, 52, 53, 54, 
	55, 49, 56, 48, 57, 58, 59, 60, 
	45, 61, 62, 63, 45, 45, 45, 45, 
	64, 65, 66, 67, 1, 45, 46, 1, 
	45, 45, 47, 45, 45, 45, 45, 45, 
	68, 49, 50, 51, 52, 53, 54, 55, 
	49, 56, 57, 57, 58, 59, 60, 45, 
	61, 62, 63, 45, 45, 45, 45, 64, 
	65, 66, 67, 1, 45, 46, 69, 69, 
	69, 69, 69, 69, 69, 69, 69, 69, 
	69, 69, 69, 70, 69, 46, 69, 46, 
	1, 45, 45, 47, 45, 45, 45, 45, 
	45, 45, 49, 50, 51, 52, 53, 54, 
	55, 49, 56, 57, 57, 58, 59, 60, 
	45, 61, 62, 63, 45, 45, 45, 45, 
	64, 65, 66, 67, 1, 45, 49, 50, 
	51, 52, 53, 45, 45, 45, 45, 45, 
	45, 58, 59, 60, 45, 61, 62, 63, 
	45, 45, 45, 45, 50, 65, 66, 67, 
	71, 45, 50, 51, 52, 53, 45, 45, 
	45, 45, 45, 45, 45, 45, 45, 45, 
	61, 62, 63, 45, 45, 45, 45, 45, 
	65, 66, 67, 71, 45, 51, 52, 53, 
	45, 45, 45, 45, 45, 45, 45, 45, 
	45, 45, 45, 45, 45, 45, 45, 45, 
	45, 45, 65, 66, 67, 45, 52, 53, 
	45, 45, 45, 45, 45, 45, 45, 45, 
	45, 45, 45, 45, 45, 45, 45, 45, 
	45, 45, 65, 66, 67, 45, 53, 45, 
	45, 45, 45, 45, 45, 45, 45, 45, 
	45, 45, 45, 45, 45, 45, 45, 45, 
	45, 65, 66, 67, 45, 65, 66, 45, 
	66, 45, 51, 52, 53, 45, 45, 45, 
	45, 45, 45, 45, 45, 45, 45, 61, 
	62, 63, 45, 45, 45, 45, 45, 65, 
	66, 67, 71, 45, 51, 52, 53, 45, 
	45, 45, 45, 45, 45, 45, 45, 45, 
	45, 45, 62, 63, 45, 45, 45, 45, 
	45, 65, 66, 67, 71, 45, 51, 52, 
	53, 45, 45, 45, 45, 45, 45, 45, 
	45, 45, 45, 45, 45, 63, 45, 45, 
	45, 45, 45, 65, 66, 67, 71, 45, 
	73, 72, 51, 52, 53, 45, 45, 45, 
	45, 45, 45, 45, 45, 45, 45, 45, 
	45, 45, 45, 45, 45, 45, 45, 65, 
	66, 67, 71, 45, 50, 51, 52, 53, 
	45, 45, 45, 45, 45, 45, 58, 59, 
	60, 45, 61, 62, 63, 45, 45, 45, 
	45, 50, 65, 66, 67, 71, 45, 50, 
	51, 52, 53, 45, 45, 45, 45, 45, 
	45, 45, 59, 60, 45, 61, 62, 63, 
	45, 45, 45, 45, 50, 65, 66, 67, 
	71, 45, 50, 51, 52, 53, 45, 45, 
	45, 45, 45, 45, 45, 45, 60, 45, 
	61, 62, 63, 45, 45, 45, 45, 50, 
	65, 66, 67, 71, 45, 49, 50, 51, 
	52, 53, 45, 55, 49, 45, 45, 45, 
	58, 59, 60, 45, 61, 62, 63, 45, 
	45, 45, 45, 50, 65, 66, 67, 71, 
	45, 49, 50, 51, 52, 53, 45, 45, 
	49, 45, 45, 45, 58, 59, 60, 45, 
	61, 62, 63, 45, 45, 45, 45, 50, 
	65, 66, 67, 71, 45, 49, 50, 51, 
	52, 53, 54, 55, 49, 45, 45, 45, 
	58, 59, 60, 45, 61, 62, 63, 45, 
	45, 45, 45, 50, 65, 66, 67, 71, 
	45, 46, 1, 45, 45, 47, 45, 45, 
	45, 45, 45, 45, 49, 50, 51, 52, 
	53, 54, 55, 49, 56, 45, 57, 58, 
	59, 60, 45, 61, 62, 63, 45, 45, 
	45, 45, 64, 65, 66, 67, 1, 45, 
	46, 69, 69, 69, 69, 69, 69, 69, 
	69, 69, 69, 69, 69, 69, 70, 69, 
	69, 69, 69, 69, 69, 69, 50, 51, 
	52, 53, 69, 69, 69, 69, 69, 69, 
	69, 69, 69, 69, 61, 62, 63, 69, 
	69, 69, 69, 69, 65, 66, 67, 71, 
	69, 46, 1, 45, 45, 47, 45, 45, 
	45, 45, 45, 45, 49, 50, 51, 52, 
	53, 54, 55, 49, 56, 48, 57, 58, 
	59, 60, 45, 61, 62, 63, 45, 45, 
	45, 45, 64, 65, 66, 67, 1, 45, 
	75, 74, 74, 74, 74, 74, 74, 74, 
	76, 74, 11, 77, 75, 74, 46, 1, 
	45, 45, 47, 45, 45, 45, 45, 45, 
	78, 49, 50, 51, 52, 53, 54, 55, 
	49, 56, 48, 57, 58, 59, 60, 45, 
	61, 62, 63, 45, 79, 80, 45, 64, 
	65, 66, 67, 1, 45, 46, 1, 45, 
	45, 47, 45, 45, 45, 45, 45, 45, 
	49, 50, 51, 52, 53, 54, 55, 49, 
	56, 48, 57, 58, 59, 60, 45, 61, 
	62, 63, 45, 79, 80, 45, 64, 65, 
	66, 67, 1, 45, 79, 80, 81, 80, 
	81, 3, 6, 82, 82, 83, 82, 82, 
	82, 82, 82, 84, 18, 19, 20, 21, 
	22, 23, 24, 18, 25, 27, 27, 28, 
	29, 30, 82, 31, 32, 33, 82, 82, 
	82, 82, 37, 38, 39, 40, 6, 82, 
	3, 6, 82, 82, 83, 82, 82, 82, 
	82, 82, 82, 18, 19, 20, 21, 22, 
	23, 24, 18, 25, 27, 27, 28, 29, 
	30, 82, 31, 32, 33, 82, 82, 82, 
	82, 37, 38, 39, 40, 6, 82, 18, 
	19, 20, 21, 22, 82, 82, 82, 82, 
	82, 82, 28, 29, 30, 82, 31, 32, 
	33, 82, 82, 82, 82, 19, 38, 39, 
	40, 85, 82, 19, 20, 21, 22, 82, 
	82, 82, 82, 82, 82, 82, 82, 82, 
	82, 31, 32, 33, 82, 82, 82, 82, 
	82, 38, 39, 40, 85, 82, 20, 21, 
	22, 82, 82, 82, 82, 82, 82, 82, 
	82, 82, 82, 82, 82, 82, 82, 82, 
	82, 82, 82, 38, 39, 40, 82, 21, 
	22, 82, 82, 82, 82, 82, 82, 82, 
	82, 82, 82, 82, 82, 82, 82, 82, 
	82, 82, 82, 38, 39, 40, 82, 22, 
	82, 82, 82, 82, 82, 82, 82, 82, 
	82, 82, 82, 82, 82, 82, 82, 82, 
	82, 82, 38, 39, 40, 82, 38, 39, 
	82, 39, 82, 20, 21, 22, 82, 82, 
	82, 82, 82, 82, 82, 82, 82, 82, 
	31, 32, 33, 82, 82, 82, 82, 82, 
	38, 39, 40, 85, 82, 20, 21, 22, 
	82, 82, 82, 82, 82, 82, 82, 82, 
	82, 82, 82, 32, 33, 82, 82, 82, 
	82, 82, 38, 39, 40, 85, 82, 20, 
	21, 22, 82, 82, 82, 82, 82, 82, 
	82, 82, 82, 82, 82, 82, 33, 82, 
	82, 82, 82, 82, 38, 39, 40, 85, 
	82, 20, 21, 22, 82, 82, 82, 82, 
	82, 82, 82, 82, 82, 82, 82, 82, 
	82, 82, 82, 82, 82, 82, 38, 39, 
	40, 85, 82, 19, 20, 21, 22, 82, 
	82, 82, 82, 82, 82, 28, 29, 30, 
	82, 31, 32, 33, 82, 82, 82, 82, 
	19, 38, 39, 40, 85, 82, 19, 20, 
	21, 22, 82, 82, 82, 82, 82, 82, 
	82, 29, 30, 82, 31, 32, 33, 82, 
	82, 82, 82, 19, 38, 39, 40, 85, 
	82, 19, 20, 21, 22, 82, 82, 82, 
	82, 82, 82, 82, 82, 30, 82, 31, 
	32, 33, 82, 82, 82, 82, 19, 38, 
	39, 40, 85, 82, 18, 19, 20, 21, 
	22, 82, 24, 18, 82, 82, 82, 28, 
	29, 30, 82, 31, 32, 33, 82, 82, 
	82, 82, 19, 38, 39, 40, 85, 82, 
	18, 19, 20, 21, 22, 82, 82, 18, 
	82, 82, 82, 28, 29, 30, 82, 31, 
	32, 33, 82, 82, 82, 82, 19, 38, 
	39, 40, 85, 82, 18, 19, 20, 21, 
	22, 23, 24, 18, 82, 82, 82, 28, 
	29, 30, 82, 31, 32, 33, 82, 82, 
	82, 82, 19, 38, 39, 40, 85, 82, 
	3, 6, 82, 82, 83, 82, 82, 82, 
	82, 82, 82, 18, 19, 20, 21, 22, 
	23, 24, 18, 25, 82, 27, 28, 29, 
	30, 82, 31, 32, 33, 82, 82, 82, 
	82, 37, 38, 39, 40, 6, 82, 3, 
	82, 82, 82, 82, 82, 82, 82, 82, 
	82, 82, 82, 82, 82, 4, 82, 82, 
	82, 82, 82, 82, 82, 19, 20, 21, 
	22, 82, 82, 82, 82, 82, 82, 82, 
	82, 82, 82, 31, 32, 33, 82, 82, 
	82, 82, 82, 38, 39, 40, 85, 82, 
	3, 86, 86, 86, 86, 86, 86, 86, 
	86, 86, 86, 86, 86, 86, 4, 86, 
	87, 82, 14, 82, 82, 82, 82, 82, 
	82, 82, 88, 82, 14, 82, 6, 86, 
	86, 86, 86, 86, 86, 86, 86, 86, 
	86, 86, 86, 86, 86, 86, 86, 86, 
	86, 86, 86, 86, 86, 86, 86, 86, 
	86, 86, 86, 86, 86, 86, 6, 86, 
	86, 86, 6, 86, 9, 82, 82, 82, 
	9, 82, 82, 82, 82, 82, 3, 6, 
	14, 82, 83, 82, 82, 82, 82, 82, 
	82, 18, 19, 20, 21, 22, 23, 24, 
	18, 25, 26, 27, 28, 29, 30, 82, 
	31, 32, 33, 82, 34, 35, 82, 37, 
	38, 39, 40, 6, 82, 3, 6, 82, 
	82, 83, 82, 82, 82, 82, 82, 82, 
	18, 19, 20, 21, 22, 23, 24, 18, 
	25, 26, 27, 28, 29, 30, 82, 31, 
	32, 33, 82, 82, 82, 82, 37, 38, 
	39, 40, 6, 82, 34, 35, 82, 35, 
	82, 79, 81, 81, 81, 81, 81, 81, 
	81, 81, 81, 81, 81, 81, 81, 81, 
	81, 81, 81, 81, 81, 79, 80, 81, 
	9, 86, 86, 86, 9, 86, 90, 89, 
	41, 89, 41, 90, 89, 90, 89, 41, 
	89, 42, 89, 0
};

static const char _use_syllable_machine_trans_targs[] = {
	5, 9, 5, 40, 2, 5, 1, 52, 
	6, 7, 5, 33, 36, 61, 62, 65, 
	66, 70, 42, 43, 44, 45, 46, 56, 
	57, 58, 67, 59, 53, 54, 55, 49, 
	50, 51, 68, 69, 71, 60, 47, 48, 
	5, 72, 74, 5, 5, 5, 8, 0, 
	32, 12, 13, 14, 15, 16, 27, 28, 
	29, 30, 24, 25, 26, 19, 20, 21, 
	31, 17, 18, 5, 11, 5, 10, 22, 
	5, 23, 5, 34, 35, 5, 37, 38, 
	39, 5, 5, 3, 41, 4, 5, 63, 
	64, 5, 73
};

static const char _use_syllable_machine_trans_actions[] = {
	1, 0, 2, 3, 0, 4, 0, 5, 
	0, 5, 8, 0, 5, 9, 0, 9, 
	3, 0, 5, 5, 0, 0, 0, 5, 
	5, 5, 3, 3, 5, 5, 5, 5, 
	5, 5, 0, 0, 0, 3, 0, 0, 
	10, 0, 0, 11, 12, 13, 5, 0, 
	5, 0, 0, 0, 0, 0, 0, 0, 
	0, 5, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 14, 5, 15, 0, 0, 
	16, 0, 17, 0, 0, 18, 5, 0, 
	0, 19, 20, 0, 3, 0, 21, 0, 
	0, 22, 0
};

static const char _use_syllable_machine_to_state_actions[] = {
	0, 0, 0, 0, 0, 6, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0
};

static const char _use_syllable_machine_from_state_actions[] = {
	0, 0, 0, 0, 0, 7, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0
};

static const short _use_syllable_machine_eof_trans[] = {
	1, 3, 3, 6, 6, 0, 44, 46, 
	46, 70, 70, 46, 46, 46, 46, 46, 
	46, 46, 46, 46, 46, 46, 73, 46, 
	46, 46, 46, 46, 46, 46, 46, 70, 
	46, 75, 78, 75, 46, 46, 82, 82, 
	83, 83, 83, 83, 83, 83, 83, 83, 
	83, 83, 83, 83, 83, 83, 83, 83, 
	83, 83, 83, 83, 83, 87, 83, 83, 
	83, 87, 83, 83, 83, 83, 82, 87, 
	90, 90, 90
};

static const int use_syllable_machine_start = 5;
static const int use_syllable_machine_first_final = 5;
static const int use_syllable_machine_error = -1;

static const int use_syllable_machine_en_main = 5;


#line 38 "hb-ot-shape-complex-use-machine.rl"



#line 167 "hb-ot-shape-complex-use-machine.rl"


#define found_syllable(syllable_type) \
  HB_STMT_START { \
    if (0) fprintf (stderr, "syllable %d..%d %s\n", ts, te, #syllable_type); \
    for (unsigned int i = ts; i < te; i++) \
      info[i].syllable() = (syllable_serial << 4) | use_##syllable_type; \
    syllable_serial++; \
    if (unlikely (syllable_serial == 16)) syllable_serial = 1; \
  } HB_STMT_END

static void
find_syllables_use (hb_buffer_t *buffer)
{
  unsigned int p, pe, eof, ts, te, act;
  int cs;
  hb_glyph_info_t *info = buffer->info;
  
#line 391 "hb-ot-shape-complex-use-machine.hh"
	{
	cs = use_syllable_machine_start;
	ts = 0;
	te = 0;
	act = 0;
	}

#line 187 "hb-ot-shape-complex-use-machine.rl"


  p = 0;
  pe = eof = buffer->len;

  unsigned int syllable_serial = 1;
  
#line 407 "hb-ot-shape-complex-use-machine.hh"
	{
	int _slen;
	int _trans;
	const unsigned char *_keys;
	const char *_inds;
	if ( p == pe )
		goto _test_eof;
_resume:
	switch ( _use_syllable_machine_from_state_actions[cs] ) {
	case 7:
#line 1 "NONE"
	{ts = p;}
	break;
#line 421 "hb-ot-shape-complex-use-machine.hh"
	}

	_keys = _use_syllable_machine_trans_keys + (cs<<1);
	_inds = _use_syllable_machine_indicies + _use_syllable_machine_index_offsets[cs];

	_slen = _use_syllable_machine_key_spans[cs];
	_trans = _inds[ _slen > 0 && _keys[0] <=( info[p].use_category()) &&
		( info[p].use_category()) <= _keys[1] ?
		( info[p].use_category()) - _keys[0] : _slen ];

_eof_trans:
	cs = _use_syllable_machine_trans_targs[_trans];

	if ( _use_syllable_machine_trans_actions[_trans] == 0 )
		goto _again;

	switch ( _use_syllable_machine_trans_actions[_trans] ) {
	case 5:
#line 1 "NONE"
	{te = p+1;}
	break;
	case 12:
#line 154 "hb-ot-shape-complex-use-machine.rl"
	{te = p+1;{ found_syllable (independent_cluster); }}
	break;
	case 14:
#line 157 "hb-ot-shape-complex-use-machine.rl"
	{te = p+1;{ found_syllable (standard_cluster); }}
	break;
	case 10:
#line 162 "hb-ot-shape-complex-use-machine.rl"
	{te = p+1;{ found_syllable (broken_cluster); }}
	break;
	case 8:
#line 163 "hb-ot-shape-complex-use-machine.rl"
	{te = p+1;{ found_syllable (non_cluster); }}
	break;
	case 11:
#line 154 "hb-ot-shape-complex-use-machine.rl"
	{te = p;p--;{ found_syllable (independent_cluster); }}
	break;
	case 15:
#line 155 "hb-ot-shape-complex-use-machine.rl"
	{te = p;p--;{ found_syllable (virama_terminated_cluster); }}
	break;
	case 16:
#line 156 "hb-ot-shape-complex-use-machine.rl"
	{te = p;p--;{ found_syllable (sakot_terminated_cluster); }}
	break;
	case 13:
#line 157 "hb-ot-shape-complex-use-machine.rl"
	{te = p;p--;{ found_syllable (standard_cluster); }}
	break;
	case 18:
#line 158 "hb-ot-shape-complex-use-machine.rl"
	{te = p;p--;{ found_syllable (number_joiner_terminated_cluster); }}
	break;
	case 17:
#line 159 "hb-ot-shape-complex-use-machine.rl"
	{te = p;p--;{ found_syllable (numeral_cluster); }}
	break;
	case 19:
#line 160 "hb-ot-shape-complex-use-machine.rl"
	{te = p;p--;{ found_syllable (symbol_cluster); }}
	break;
	case 22:
#line 161 "hb-ot-shape-complex-use-machine.rl"
	{te = p;p--;{ found_syllable (hieroglyph_cluster); }}
	break;
	case 20:
#line 162 "hb-ot-shape-complex-use-machine.rl"
	{te = p;p--;{ found_syllable (broken_cluster); }}
	break;
	case 21:
#line 163 "hb-ot-shape-complex-use-machine.rl"
	{te = p;p--;{ found_syllable (non_cluster); }}
	break;
	case 1:
#line 157 "hb-ot-shape-complex-use-machine.rl"
	{{p = ((te))-1;}{ found_syllable (standard_cluster); }}
	break;
	case 4:
#line 162 "hb-ot-shape-complex-use-machine.rl"
	{{p = ((te))-1;}{ found_syllable (broken_cluster); }}
	break;
	case 2:
#line 1 "NONE"
	{	switch( act ) {
	case 9:
	{{p = ((te))-1;} found_syllable (broken_cluster); }
	break;
	case 10:
	{{p = ((te))-1;} found_syllable (non_cluster); }
	break;
	}
	}
	break;
	case 3:
#line 1 "NONE"
	{te = p+1;}
#line 162 "hb-ot-shape-complex-use-machine.rl"
	{act = 9;}
	break;
	case 9:
#line 1 "NONE"
	{te = p+1;}
#line 163 "hb-ot-shape-complex-use-machine.rl"
	{act = 10;}
	break;
#line 531 "hb-ot-shape-complex-use-machine.hh"
	}

_again:
	switch ( _use_syllable_machine_to_state_actions[cs] ) {
	case 6:
#line 1 "NONE"
	{ts = 0;}
	break;
#line 540 "hb-ot-shape-complex-use-machine.hh"
	}

	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	if ( _use_syllable_machine_eof_trans[cs] > 0 ) {
		_trans = _use_syllable_machine_eof_trans[cs] - 1;
		goto _eof_trans;
	}
	}

	}

#line 195 "hb-ot-shape-complex-use-machine.rl"

}

#undef found_syllable

#endif /* HB_OT_SHAPE_COMPLEX_USE_MACHINE_HH */
