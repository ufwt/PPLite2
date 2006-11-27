/* Abstract checked arithmetic function container
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

#ifndef PPL_checked_defs_hh
#define PPL_checked_defs_hh 1

#include <iostream>
#include <gmpxx.h>
#include "compiler.hh"
#include "Rounding_Dir.defs.hh"
#include "Numeric_Format.defs.hh"

namespace Parma_Polyhedra_Library {

namespace Checked {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! A policy checking for overflows.
/*! \ingroup PPL_CXX_interface */
#endif // PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
struct Check_Overflow_Policy {
  const_bool_nodef(check_overflow, true);
  const_bool_nodef(check_inf_add_inf, false);
  const_bool_nodef(check_inf_sub_inf, false);
  const_bool_nodef(check_inf_mul_zero, false);
  const_bool_nodef(check_div_zero, false);
  const_bool_nodef(check_inf_div_inf, false);
  const_bool_nodef(check_inf_mod, false);
  const_bool_nodef(check_sqrt_neg, false);
  const_bool_nodef(has_nan, false);
  const_bool_nodef(has_infinity, false);
  const_bool_nodef(convertible, true);
  const_bool_nodef(fpu_check_inexact, true);
  const_bool_nodef(check_nan_result, true);
};

// It is a pity that function partial specialization is not permitted
// by C++.  To (partly) overcome this limitation, we use class
// encapsulated functions and partial specialization of containing
// classes.

#define FUNCTION_CLASS(name) name ## _function_struct

#define DECLARE_FUN1_0_0(name, ret_type, qual, type) \
template <typename Policy, typename type> \
struct FUNCTION_CLASS(name); \
template <typename Policy, typename type> \
inline ret_type name(qual type& arg) { \
  return FUNCTION_CLASS(name)<Policy, type>::function(arg); \
}

#define DECLARE_FUN1_0_1(name, ret_type, qual, type, after1) \
template <typename Policy, typename type> \
struct FUNCTION_CLASS(name); \
template <typename Policy, typename type> \
inline ret_type name(qual type& arg, after1 a1) { \
  return FUNCTION_CLASS(name)<Policy, type>::function(arg, a1); \
}

#define DECLARE_FUN1_0_2(name, ret_type, qual, type, after1, after2) \
template <typename Policy, typename type> \
struct FUNCTION_CLASS(name); \
template <typename Policy, typename type> \
inline ret_type name(qual type& arg, after1 a1, after2 a2) { \
  return FUNCTION_CLASS(name)<Policy, type>::function(arg, a1, a2); \
}

#define DECLARE_FUN1_0_3(name, ret_type, qual, type, after1, after2, after3) \
template <typename Policy, typename type> \
struct FUNCTION_CLASS(name); \
template <typename Policy, typename type> \
inline ret_type name(qual type& arg, after1 a1, after2 a2, after3 a3) { \
  return FUNCTION_CLASS(name)<Policy, type>::function(arg, a1, a2, a3); \
}

#define DECLARE_FUN1_1_1(name, ret_type, before1, qual, type, after1) \
template <typename Policy, typename type> \
struct FUNCTION_CLASS(name); \
template <typename Policy, typename type> \
inline ret_type name(before1 b1, qual type& arg, after1 a1) { \
  return FUNCTION_CLASS(name)<Policy, type>::function(b1, arg, a1); \
}

#define DECLARE_FUN1_1_2(name, ret_type, before1, qual, type, after1, after2) \
template <typename Policy, typename type> \
struct FUNCTION_CLASS(name); \
template <typename Policy, typename type> \
inline ret_type name(before1 b1, qual type& arg, after1 a1, after2 a2) { \
  return FUNCTION_CLASS(name)<Policy, type>::function(b1, arg, a1, a2); \
}

#define DECLARE_FUN1_2_2(name, ret_type, before1, before2, qual, type, after1, after2) \
template <typename Policy, typename type> \
struct FUNCTION_CLASS(name); \
template <typename Policy, typename type> \
inline ret_type name(before1 b1, before2 b2, qual type& arg, after1 a1, after2 a2) { \
  return FUNCTION_CLASS(name)<Policy, type>::function(b1, b2, arg, a1, a2); \
}

#define DECLARE_FUN2_0_0(name, ret_type, qual1, type1, qual2, type2) \
template <typename Policy1, typename Policy2, typename type1, typename type2>	     \
struct FUNCTION_CLASS(name); \
template <typename Policy1, typename Policy2, typename type1, typename type2> \
inline ret_type name(qual1 type1& arg1, qual2 type2& arg2) { \
  return FUNCTION_CLASS(name)<Policy1, Policy2, type1, type2>::function(arg1, arg2); \
}

#define DECLARE_FUN2_0_1(name, ret_type, qual1, type1, qual2, type2, after1) \
template <typename Policy1, typename Policy2, typename type1, typename type2>		\
struct FUNCTION_CLASS(name); \
template <typename Policy1, typename Policy2, typename type1, typename type2> \
inline ret_type name(qual1 type1& arg1, qual2 type2& arg2, after1 a1) { \
  return FUNCTION_CLASS(name)<Policy1, Policy2, type1, type2>::function(arg1, arg2, a1); \
}

#define DECLARE_FUN2_0_2(name, ret_type, qual1, type1, qual2, type2, after1, after2) \
template <typename Policy1, typename Policy2, typename type1, typename type2> \
struct FUNCTION_CLASS(name); \
template <typename Policy1, typename Policy2, typename type1, typename type2> \
inline ret_type name(qual1 type1& arg1, qual2 type2& arg2, after1 a1, after2 a2) { \
  return FUNCTION_CLASS(name)<Policy1, Policy2, type1, type2>::function(arg1, arg2, a1, a2); \
}

#define DECLARE_FUN3_0_1(name, ret_type, qual1, type1, qual2, type2, qual3, type3, after1) \
template <typename Policy1, typename Policy2, typename Policy3, typename type1, typename type2, typename type3> \
struct FUNCTION_CLASS(name); \
template <typename Policy1, typename Policy2, typename Policy3, typename type1, typename type2, typename type3> \
inline ret_type name(qual1 type1& arg1, qual2 type2& arg2, qual3 type3& arg3, after1 a1) { \
  return FUNCTION_CLASS(name)<Policy1, Policy2, Policy3, type1, type2, type3>::function(arg1, arg2, arg3, a1); \
}

#define DECLARE_FUN5_0_1(name, ret_type,				\
			 qual1, type1, qual2, type2, qual3, type3,	\
			 qual4, type4, qual5, type5,			\
			 after1)					\
template <typename Policy1, typename Policy2, typename Policy3,		\
	  typename Policy4,typename Policy5,				\
	  typename type1, typename type2, typename type3,		\
	  typename type4, typename type5>				\
struct FUNCTION_CLASS(name);						\
template <typename Policy1, typename Policy2, typename Policy3,		\
	  typename Policy4,typename Policy5,				\
	  typename type1, typename type2, typename type3,		\
	  typename type4, typename type5>				\
inline ret_type name(qual1 type1& arg1, qual2 type2& arg2,		\
		     qual3 type3& arg3, qual4 type4& arg4,		\
		     qual5 type5& arg5,	after1 a1) {			\
  return FUNCTION_CLASS(name)<Policy1, Policy2, Policy3, Policy4, Policy5, type1, type2, type3, type4, type5> \
    ::function(arg1, arg2, arg3, arg4, arg5, a1);			\
}

#define SPECIALIZE_FUN1_0_0(name, func, ret_type, qual, type) \
template <typename Policy> \
struct FUNCTION_CLASS(name)<Policy, type> { \
  static inline ret_type function(qual type& arg) { \
    return func<Policy>(arg); \
  } \
};

#define SPECIALIZE_FUN1_0_1(name, func, ret_type, qual, type, after1) \
template <typename Policy> \
struct FUNCTION_CLASS(name)<Policy, type> { \
  static inline ret_type function(qual type& arg, after1 a1) { \
    return func<Policy>(arg, a1); \
  } \
};

#define SPECIALIZE_FUN1_0_2(name, func, ret_type, qual, type, after1, after2) \
template <typename Policy> \
struct FUNCTION_CLASS(name)<Policy, type> { \
  static inline ret_type function(qual type& arg, after1 a1, after2 a2) { \
    return func<Policy>(arg, a1, a2); \
  } \
};

#define SPECIALIZE_FUN1_0_3(name, func, ret_type, qual, type, after1, after2, after3) \
template <typename Policy> \
struct FUNCTION_CLASS(name)<Policy, type> { \
  static inline ret_type function(qual type& arg, after1 a1, after2 a2, after3 a3) { \
    return func<Policy>(arg, a1, a2, a3); \
  } \
};

#define SPECIALIZE_FUN1_1_1(name, func, ret_type, before1, qual, type, after1) \
template <typename Policy> \
struct FUNCTION_CLASS(name)<Policy, type> { \
  static inline ret_type function(before1 b1, qual type& arg, after1 a1) { \
    return func<Policy>(b1, arg, a1); \
  } \
};

#define SPECIALIZE_FUN1_1_2(name, func, ret_type, before1, qual, type, after1, after2) \
template <typename Policy> \
struct FUNCTION_CLASS(name)<Policy, type> { \
  static inline ret_type function(before1 b1, qual type& arg, after1 a1, after2 a2) { \
    return func<Policy>(b1, arg, a1, a2); \
  } \
};

#define SPECIALIZE_FUN1_2_2(name, func, ret_type, before1, before2, qual, type, after1, after2) \
template <typename Policy> \
struct FUNCTION_CLASS(name)<Policy, type> { \
  static inline ret_type function(before1 b1, before2 b2, qual type& arg, after1 a1, after2 a2) { \
    return func<Policy>(b1, b2, arg, a1, a2); \
  } \
};

#define SPECIALIZE_FUN2_0_0(name, func, ret_type, qual1, type1, qual2, type2) \
template <typename Policy1, typename Policy2>				\
struct FUNCTION_CLASS(name)<Policy1, Policy2, type1, type2> { \
  static inline ret_type function(qual1 type1& arg1, qual2 type2 &arg2) { \
    return func<Policy1, Policy2>(arg1, arg2); \
  } \
};

#define SPECIALIZE_FUN2_0_1(name, func, ret_type, qual1, type1, qual2, type2, after1) \
template <typename Policy1, typename Policy2> \
struct FUNCTION_CLASS(name)<Policy1, Policy2, type1, type2> { \
  static inline ret_type function(qual1 type1& arg1, qual2 type2 &arg2, after1 a1) { \
    return func<Policy1, Policy2>(arg1, arg2, a1); \
  } \
};

#define SPECIALIZE_FUN2_0_2(name, func, ret_type, qual1, type1, qual2, type2, after1, after2) \
template <typename Policy1, typename Policy2> \
struct FUNCTION_CLASS(name)<Policy1, Policy2, type1, type2> { \
  static inline ret_type function(qual1 type1& arg1, qual2 type2 &arg2, after1 a1, after2 a2) { \
    return func<Policy1, Policy2>(arg1, arg2, a1, a2); \
  } \
};

#define SPECIALIZE_FUN3_0_1(name, func, ret_type, qual1, type1, qual2, type2, qual3, type3, after1) \
template <typename Policy1, typename Policy2, typename Policy3>			\
struct FUNCTION_CLASS(name) <Policy1, Policy2, Policy3, type1, type2, type3> { \
  static inline Result function(qual1 type1& arg1, qual2 type2 &arg2, qual3 type3 &arg3, after1 a1) { \
    return func<Policy1, Policy2, Policy3>(arg1, arg2, arg3, a1); \
  } \
};

#define SPECIALIZE_FUN5_0_1(name, func, ret_type,			\
			    qual1, type1, qual2, type2, qual3, type3,	\
			    qual4, type4, qual5, type5, after1)		\
  template <typename Policy1, typename Policy2, typename Policy3,	\
	    typename Policy4, typename Policy5>				\
  struct FUNCTION_CLASS(name) <Policy1, Policy2, Policy3, Policy4, Policy5, \
			       type1, type2, type3, type4, type5> {	\
  static inline Result							\
  function(qual1 type1& arg1, qual2 type2 &arg2, qual3 type3 &arg3,	\
	   qual4 type4 &arg4, qual5 type5 &arg5, after1 a1) {		\
    return func<Policy1, Policy2, Policy3, Policy4, Policy5>(arg1, arg2, arg3, arg4, arg5, a1);	\
  }									\
};

#define nonconst

#define SPECIALIZE_COPY(func, Type) \
  SPECIALIZE_FUN2_0_0(copy, func, void, nonconst, Type, const, Type)
#define SPECIALIZE_SGN(func, From) \
  SPECIALIZE_FUN1_0_0(sgn, func, Result, const, From)
#define SPECIALIZE_CMP(func, Type1, Type2) \
  SPECIALIZE_FUN2_0_0(cmp, func, Result, const, Type1, const, Type2)
#define SPECIALIZE_SET_SPECIAL(func, Type) \
  SPECIALIZE_FUN1_0_1(set_special, func, Result, nonconst, Type, Result)
#define SPECIALIZE_CLASSIFY(func, Type) \
  SPECIALIZE_FUN1_0_3(classify, func, Result, const, Type, bool, bool, bool)
#define SPECIALIZE_IS_NAN(func, Type) \
  SPECIALIZE_FUN1_0_0(is_nan, func, bool, const, Type)
#define SPECIALIZE_IS_MINF(func, Type) \
  SPECIALIZE_FUN1_0_0(is_minf, func, bool, const, Type)
#define SPECIALIZE_IS_PINF(func, Type) \
  SPECIALIZE_FUN1_0_0(is_pinf, func, bool, const, Type)
#define SPECIALIZE_IS_INT(func, Type) \
  SPECIALIZE_FUN1_0_0(is_int, func, bool, const, Type)
#define SPECIALIZE_CONSTRUCT(func, To, From) \
  SPECIALIZE_FUN2_0_1(construct, func, Result, nonconst, To, const, From, Rounding_Dir)
#define SPECIALIZE_ASSIGN(func, To, From) \
  SPECIALIZE_FUN2_0_1(assign, func, Result, nonconst, To, const, From, Rounding_Dir)
#define SPECIALIZE_FLOOR(func, To, From) \
  SPECIALIZE_FUN2_0_1(floor, func, Result, nonconst, To, const, From, Rounding_Dir)
#define SPECIALIZE_CEIL(func, To, From) \
  SPECIALIZE_FUN2_0_1(ceil, func, Result, nonconst, To, const, From, Rounding_Dir)
#define SPECIALIZE_TRUNC(func, To, From) \
  SPECIALIZE_FUN2_0_1(trunc, func, Result, nonconst, To, const, From, Rounding_Dir)
#define SPECIALIZE_NEG(func, To, From) \
  SPECIALIZE_FUN2_0_1(neg, func, Result, nonconst, To, const, From, Rounding_Dir)
#define SPECIALIZE_ABS(func, To, From) \
  SPECIALIZE_FUN2_0_1(abs, func, Result, nonconst, To, const, From, Rounding_Dir)
#define SPECIALIZE_SQRT(func, To, From) \
  SPECIALIZE_FUN2_0_1(sqrt, func, Result, nonconst, To, const, From, Rounding_Dir)
#define SPECIALIZE_ADD(func, To, From1, From2) \
  SPECIALIZE_FUN3_0_1(add, func, Result, nonconst, To, const, From1, const, From2, Rounding_Dir)
#define SPECIALIZE_SUB(func, To, From1, From2) \
  SPECIALIZE_FUN3_0_1(sub, func, Result, nonconst, To, const, From1, const, From2, Rounding_Dir)
#define SPECIALIZE_MUL(func, To, From1, From2) \
  SPECIALIZE_FUN3_0_1(mul, func, Result, nonconst, To, const, From1, const, From2, Rounding_Dir)
#define SPECIALIZE_DIV(func, To, From1, From2) \
  SPECIALIZE_FUN3_0_1(div, func, Result, nonconst, To, const, From1, const, From2, Rounding_Dir)
#define SPECIALIZE_REM(func, To, From1, From2) \
  SPECIALIZE_FUN3_0_1(rem, func, Result, nonconst, To, const, From1, const, From2, Rounding_Dir)
#define SPECIALIZE_MUL2EXP(func, To, From) \
  SPECIALIZE_FUN2_0_2(mul2exp, func, Result, nonconst, To, const, From, int, Rounding_Dir)
#define SPECIALIZE_DIV2EXP(func, To, From) \
  SPECIALIZE_FUN2_0_2(div2exp, func, Result, nonconst, To, const, From, int, Rounding_Dir)
#define SPECIALIZE_ADD_MUL(func, To, From1, From2) \
  SPECIALIZE_FUN3_0_1(add_mul, func, Result, nonconst, To, const, From1, const, From2, Rounding_Dir)
#define SPECIALIZE_SUB_MUL(func, To, From1, From2) \
  SPECIALIZE_FUN3_0_1(sub_mul, func, Result, nonconst, To, const, From1, const, From2, Rounding_Dir)
#define SPECIALIZE_GCD(func, To, From1, From2) \
  SPECIALIZE_FUN3_0_1(gcd, func, Result, nonconst, To, const, From1, const, From2, Rounding_Dir)
#define SPECIALIZE_GCDEXT(func, To1, From1, From2, To2, To3)		\
  SPECIALIZE_FUN5_0_1(gcdext, func, Result, nonconst, To1,		\
		      const, From1, const, From2, nonconst, To2, nonconst, To3, Rounding_Dir)
#define SPECIALIZE_LCM(func, To, From1, From2) \
  SPECIALIZE_FUN3_0_1(lcm, func, Result, nonconst, To, const, From1, const, From2, Rounding_Dir)
#define SPECIALIZE_INPUT(func, Type) \
  SPECIALIZE_FUN1_0_2(input, func, Result, nonconst, Type, std::istream&, Rounding_Dir)
#define SPECIALIZE_OUTPUT(func, Type) \
  SPECIALIZE_FUN1_1_2(output, func, Result, std::ostream&, const, Type, const Numeric_Format&, Rounding_Dir)


DECLARE_FUN2_0_0(copy,        void, nonconst, Type1, const, Type2)
DECLARE_FUN1_0_0(sgn,         Result, const, From)
DECLARE_FUN2_0_0(cmp,         Result, const, Type1, const, Type2)
DECLARE_FUN1_0_1(set_special, Result, nonconst, Type, Result)
DECLARE_FUN1_0_3(classify,    Result, const, Type, bool, bool, bool)
DECLARE_FUN1_0_0(is_nan,      bool, const, Type)
DECLARE_FUN1_0_0(is_minf,     bool, const, Type)
DECLARE_FUN1_0_0(is_pinf,     bool, const, Type)
DECLARE_FUN1_0_0(is_int,      bool, const, Type)
DECLARE_FUN2_0_1(construct,   Result, nonconst, To, const, From, Rounding_Dir)
DECLARE_FUN2_0_1(assign,      Result, nonconst, To, const, From, Rounding_Dir)
DECLARE_FUN2_0_1(floor,       Result, nonconst, To, const, From, Rounding_Dir)
DECLARE_FUN2_0_1(ceil,        Result, nonconst, To, const, From, Rounding_Dir)
DECLARE_FUN2_0_1(trunc,       Result, nonconst, To, const, From, Rounding_Dir)
DECLARE_FUN2_0_1(neg,         Result, nonconst, To, const, From, Rounding_Dir)
DECLARE_FUN2_0_1(abs,         Result, nonconst, To, const, From, Rounding_Dir)
DECLARE_FUN2_0_1(sqrt,        Result, nonconst, To, const, From, Rounding_Dir)
DECLARE_FUN3_0_1(add,         Result, nonconst, To, const, From1, const, From2, Rounding_Dir)
DECLARE_FUN3_0_1(sub,         Result, nonconst, To, const, From1, const, From2, Rounding_Dir)
DECLARE_FUN3_0_1(mul,         Result, nonconst, To, const, From1, const, From2, Rounding_Dir)
DECLARE_FUN3_0_1(div,         Result, nonconst, To, const, From1, const, From2, Rounding_Dir)
DECLARE_FUN3_0_1(rem,         Result, nonconst, To, const, From1, const, From2, Rounding_Dir)
DECLARE_FUN2_0_2(mul2exp,     Result, nonconst, To, const, From, int, Rounding_Dir)
DECLARE_FUN2_0_2(div2exp,     Result, nonconst, To, const, From, int, Rounding_Dir)
DECLARE_FUN3_0_1(add_mul,     Result, nonconst, To, const, From1, const, From2, Rounding_Dir)
DECLARE_FUN3_0_1(sub_mul,     Result, nonconst, To, const, From1, const, From2, Rounding_Dir)
DECLARE_FUN3_0_1(gcd,         Result, nonconst, To, const, From1, const, From2, Rounding_Dir)
DECLARE_FUN5_0_1(gcdext,      Result, nonconst, To1, const, From1, const, From2,
		                      nonconst, To2, nonconst, To3, Rounding_Dir)
DECLARE_FUN3_0_1(lcm,         Result, nonconst, To, const, From1, const, From2, Rounding_Dir)
DECLARE_FUN1_0_2(input,       Result, nonconst, Type, std::istream&, Rounding_Dir)
DECLARE_FUN1_1_2(output,      Result, std::ostream&, const, Type, const Numeric_Format&, Rounding_Dir)

template <typename Policy, typename To>
Result round(To& to, Result r, Rounding_Dir dir);

Result input_mpq(mpq_class& to, std::istream& is);

} // namespace Checked

struct Minus_Infinity {
};

struct Plus_Infinity {
};

struct Not_A_Number {
};

extern Minus_Infinity MINUS_INFINITY;
extern Plus_Infinity PLUS_INFINITY;
extern Not_A_Number NOT_A_NUMBER;

template <typename T>
struct Checked_Supports : public False {
};

} // namespace Parma_Polyhedra_Library

#define CHECK_P(cond, check) ((cond) ? (check) : (assert(!(check)), false))

#include "checked.inlines.hh"
#include "checked_int.inlines.hh"
#include "checked_float.inlines.hh"
#include "checked_mpz.inlines.hh"
#include "checked_mpq.inlines.hh"
#include "checked_ext.inlines.hh"

#endif // !defined(PPL_checked_defs_hh)
