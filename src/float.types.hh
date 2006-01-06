/* Architecture-dependent, floating-point number types.
   Copyright (C) 2001-2006 Roberto Bagnara <bagnara@cs.unipr.it>

This file is part of the Parma Polyhedra Library (PPL).

The PPL is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.

The PPL is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.

For the most up-to-date information see the Parma Polyhedra Library
site: http://www.cs.unipr.it/ppl/ . */

#ifndef PPL_float_types_hh
#define PPL_float_types_hh

namespace Parma_Polyhedra_Library {

#if SIZEOF_FLOAT == 4
#define FLOAT32_TYPE float
#elif SIZEOF_DOUBLE == 4
#define FLOAT32_TYPE double
#elif SIZEOF_LONG_DOUBLE == 4
#define FLOAT32_TYPE long double
#endif

#ifdef FLOAT32_TYPE
typedef FLOAT32_TYPE float32_t;
#endif

#if SIZEOF_FLOAT == 8
#define FLOAT64_TYPE float
#elif SIZEOF_DOUBLE == 8
#define FLOAT64_TYPE double
#elif SIZEOF_LONG_DOUBLE == 8
#define FLOAT64_TYPE long double
#endif

#ifdef FLOAT64_TYPE
typedef FLOAT64_TYPE float64_t;
#endif

#if SIZEOF_FLOAT == 12
#define FLOAT96_TYPE float
#elif SIZEOF_DOUBLE == 12
#define FLOAT96_TYPE double
#elif SIZEOF_LONG_DOUBLE == 12
#define FLOAT96_TYPE long double
#endif

#ifdef FLOAT96_TYPE
typedef FLOAT96_TYPE float96_t;
#endif

#if SIZEOF_FLOAT == 16
#define FLOAT128_TYPE float
#elif SIZEOF_DOUBLE == 16
#define FLOAT128_TYPE double
#elif SIZEOF_LONG_DOUBLE == 16
#define FLOAT128_TYPE long double
#endif

#ifdef FLOAT128_TYPE
typedef FLOAT128_TYPE float128_t;
#endif

#if SIZEOF_FLOAT == 4
typedef float32_t float_t;
#elif SIZEOF_FLOAT == 8
typedef float64_t float_t;
#elif SIZEOF_FLOAT == 12
typedef float96_t float_t;
#elif SIZEOF_FLOAT == 16
typedef float128_t float_t;
#else
#error "Unsupported float size"
#endif

#if SIZEOF_DOUBLE == 4
typedef float32_t double_t;
#elif SIZEOF_DOUBLE == 8
typedef float64_t double_t;
#elif SIZEOF_DOUBLE == 12
typedef float96_t double_t;
#elif SIZEOF_DOUBLE == 16
typedef float128_t double_t;
#else
#error "Unsupported double size"
#endif

#if SIZEOF_LONG_DOUBLE == 4
typedef float32_t long_double_t;
#elif SIZEOF_LONG_DOUBLE == 8
typedef float64_t long_double_t;
#elif SIZEOF_LONG_DOUBLE == 12
typedef float96_t long_double_t;
#elif SIZEOF_LONG_DOUBLE == 16
typedef float128_t long_double_t;
#else
#error "Unsupported long double size"
#endif

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_float_types_hh)
