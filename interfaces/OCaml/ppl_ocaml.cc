/* Implementation of the OCaml interface.
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

#include "ppl.hh"

// OCaml include files.
#define CAML_NAME_SPACE
extern "C" {
#include "caml/mlvalues.h"
#include "caml/memory.h"
#include "caml/custom.h"
#include "caml/fail.h"
#include "caml/callback.h"
#include "caml/alloc.h"
}

#include <stdexcept>
#include <sstream>
#include <cstdio>
#include <cerrno>
#include <climits>
#include <iostream>
#include <algorithm>


using namespace Parma_Polyhedra_Library;
using namespace Parma_Polyhedra_Library::IO_Operators;

#define CATCH_ALL							\
  catch(std::bad_alloc&) {						\
    caml_raise_out_of_memory();						\
  }									\
  catch(std::invalid_argument& e) {					\
    caml_invalid_argument(const_cast<char*>(e.what()));			\
  }									\
  catch(std::overflow_error& e) {					\
    caml_raise_with_string(*caml_named_value("PPL_arithmetic_overflow"), \
			   (const_cast<char*>(e.what())));		\
  }									\
  catch(std::runtime_error& e) {					\
    caml_raise_with_string(*caml_named_value("PPL_internal_error"),	\
			   (const_cast<char*>(e.what())));		\
  }									\
  catch(std::exception& e) {						\
    caml_raise_with_string(*caml_named_value("PPL_unknown_standard_exception"), \
			   (const_cast<char*>(e.what())));		\
  }									\
  catch(...) {								\
    caml_raise_constant(*caml_named_value("PPL_unexpected_error"));	\
  }


// Function for the management of mpz_t integers.
extern "C" struct custom_operations _mlgmp_custom_z;

static inline mpz_t* mpz_val(value val) {
  return ((mpz_t*) (Data_custom_val(val)));
}

static inline value alloc_mpz(void) {
  return caml_alloc_custom(&_mlgmp_custom_z, sizeof(mpz_t), 0, 1);
}

Linear_Expression
build_Linear_Expression(value e) {
  switch (Tag_val(e)) {
  case 0:
    // Variable
    return Variable(Long_val(Field(e, 0)));
  case 1: {
    // Coefficient
    mpz_class z((__mpz_struct*) Data_custom_val(Field(e, 0)));
    return Linear_Expression(Coefficient(z));
  }
  case 2:
    // Unary_Plus
    return build_Linear_Expression(Field(e, 0));
  case 3:
    // Unary_Minus
    return -build_Linear_Expression(Field(e, 0));
  case 4:
    // Plus
    return build_Linear_Expression(Field(e, 0))
      + build_Linear_Expression(Field(e, 1));
  case 5:
    // Minus
    return build_Linear_Expression(Field(e, 0))
      - build_Linear_Expression(Field(e, 1));
  case 6: {
    // Times
    mpz_class z((__mpz_struct*) Data_custom_val(Field(e, 0)));
    return Coefficient(z) * build_Linear_Expression(Field(e, 1));
  }
  default:
    caml_invalid_argument("Error building PPL::Linear_Expression");
  }
}

Constraint
build_Constraint(value c) {
  value e1 = Field(c, 0);
  value e2 = Field(c, 1);
  switch (Tag_val(c)) {
  case 0:
    // Less_Than
    return build_Linear_Expression(e1) < build_Linear_Expression(e2);
  case 1:
    // Less_Than_Or_Equal
    return build_Linear_Expression(e1) <= build_Linear_Expression(e2);
  case 2:
    // Equal
    return build_Linear_Expression(e1) == build_Linear_Expression(e2);
  case 3:
    // Greater_Than
    return build_Linear_Expression(e1) > build_Linear_Expression(e2);
  case 4:
    // Greater_Than_Or_Equal
    return build_Linear_Expression(e1) >= build_Linear_Expression(e2);
  default:
    caml_invalid_argument("Error building PPL::Constraint");
  }
}

Congruence
build_Congruence(value c) {
  value e1 = Field(c, 0);
  value e2 = Field(c, 1);
  mpz_class z((__mpz_struct*) Data_custom_val(Field(c, 2)));
  Linear_Expression lhs = build_Linear_Expression(e1);
  Linear_Expression rhs = build_Linear_Expression(e2);
  return ((lhs %= rhs) / z);
}

Generator
build_Generator(value g) {
  switch (Tag_val(g)) {
  case 0:
    // Line
    return Generator::line(build_Linear_Expression(Field(g, 0)));
  case 1:
    // Ray
    return Generator::ray(build_Linear_Expression(Field(g, 0)));
  case 2: {
    // Point
    mpz_class z((__mpz_struct*) Data_custom_val(Field(g, 1)));
    return Generator::point(build_Linear_Expression(Field(g, 0)),
			    Coefficient(z));
  }
  case 3: {
    // Closure_point
    mpz_class z((__mpz_struct*) Data_custom_val(Field(g, 1)));
    return Generator::closure_point(build_Linear_Expression(Field(g, 0)),
				    Coefficient(z));
  }
  default:
    caml_invalid_argument("Error building PPL::Constraint");
  }
}

Constraint_System
build_Constraint_System(value cl) {
  Constraint_System cs;
  while (cl != Val_int(0)) {
    cs.insert(build_Constraint(Field(cl, 0)));
    cl = Field(cl, 1);
  }
  return cs;
}

Generator_System
build_Generator_System(value gl) {
  Generator_System gs;
  while (gl != Val_int(0)) {
    gs.insert(build_Generator(Field(gl, 0)));
    gl = Field(gl, 1);
  }
  return gs;
}

Congruence_System
build_Congruence_System(value cgl) {
  Congruence_System cgs;
  while (cgl != Val_int(0)) {
    cgs.insert(build_Congruence(Field(cgl, 0)));
    cgl = Field(cgl, 1);
  }
  return cgs;
}

//! Give access to the embedded Polyhedron* in \p v.
inline Polyhedron*&
p_Polyhedron_val(value v) {
  return *reinterpret_cast<Polyhedron**>(Data_custom_val(v));
}

void
custom_Polyhedron_finalize(value v) {
  std::cerr << "About to delete a polyhedron " << *p_Polyhedron_val(v)
	    << std::endl;
  delete p_Polyhedron_val(v);
}

static struct custom_operations Polyhedron_custom_operations = {
  "it.unipr.cs.ppl" "." PPL_VERSION "." "Polyhedron",
  custom_Polyhedron_finalize,
  custom_compare_default,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default
};

inline value
val_p_Polyhedron(const Polyhedron& ph) {
  value v = caml_alloc_custom(&Polyhedron_custom_operations,
			      sizeof(Polyhedron*), 0, 1);
  p_Polyhedron_val(v) = const_cast<Polyhedron*>(&ph);
  return(v);
}

extern "C"
CAMLprim value
ppl_new_C_Polyhedron_from_space_dimension(value d) try {
  CAMLparam1(d);
  int dd = Int_val(d);
  if (dd < 0)
    abort();
  CAMLreturn(val_p_Polyhedron(*new C_Polyhedron(dd)));
}
CATCH_ALL

extern "C"
CAMLprim value
ppl_new_C_Polyhedron_from_constraint_system(value cl) try {
  CAMLparam1(cl);
  Constraint_System cs = build_Constraint_System(cl);
  Generator_System gs;
  CAMLreturn(val_p_Polyhedron(*new C_Polyhedron(cs)));
}
CATCH_ALL

extern "C"
CAMLprim value
ppl_new_C_Polyhedron_from_generator_system(value gl) try {
  CAMLparam1(gl);
  Generator_System gs = build_Generator_System(gl);
  CAMLreturn(val_p_Polyhedron(*new C_Polyhedron(gs)));
}
CATCH_ALL

extern "C"
CAMLprim value
ppl_new_C_Polyhedron_from_congruence_system(value gl) try {
  CAMLparam1(gl);
  Congruence_System gs = build_Congruence_System(gl);
  CAMLreturn(val_p_Polyhedron(*new C_Polyhedron(gs)));
}
CATCH_ALL

extern "C"
CAMLprim value
ppl_Polyhedron_space_dimension(value ph) try {
  CAMLparam1(ph);
  const Polyhedron& pph = *p_Polyhedron_val(ph);
  dimension_type d = pph.space_dimension();
  if (d > INT_MAX)
    abort();
  CAMLreturn(Val_int(d));
}
CATCH_ALL

extern "C"
CAMLprim value
ppl_Polyhedron_affine_dimension(value ph) try {
  CAMLparam1(ph);
  const Polyhedron& pph = *p_Polyhedron_val(ph);
  dimension_type d = pph.affine_dimension();
  if (d > INT_MAX)
    abort();
  CAMLreturn(Val_int(d));
}
CATCH_ALL

extern "C"
CAMLprim value
ppl_Polyhedron_is_empty(value ph) try {
  CAMLparam1(ph);
  const Polyhedron& pph = *p_Polyhedron_val(ph);
  CAMLreturn(Val_bool(pph.is_empty()));
}
CATCH_ALL

extern "C"
CAMLprim value
ppl_Polyhedron_is_universe(value ph) try {
  CAMLparam1(ph);
  const Polyhedron& pph = *p_Polyhedron_val(ph);
  CAMLreturn(Val_bool(pph.is_universe()));
}
CATCH_ALL

extern "C"
CAMLprim value
ppl_Polyhedron_contains_integer_point(value ph) try {
  CAMLparam1(ph);
  const Polyhedron& pph = *p_Polyhedron_val(ph);
  CAMLreturn(Val_bool(pph.contains_integer_point()));
}
CATCH_ALL

extern "C"
CAMLprim value
ppl_Polyhedron_is_topologically_closed(value ph) try {
  CAMLparam1(ph);
  const Polyhedron& pph = *p_Polyhedron_val(ph);
  CAMLreturn(Val_bool(pph.is_topologically_closed()));
}
CATCH_ALL

extern "C"
CAMLprim value
ppl_Polyhedron_is_bounded(value ph) try {
  CAMLparam1(ph);
  const Polyhedron& pph = *p_Polyhedron_val(ph);
  CAMLreturn(Val_bool(pph.is_bounded()));
}
CATCH_ALL

extern "C"
CAMLprim value
ppl_Polyhedron_bounds_from_below(value ph, value le) try {
  CAMLparam2(ph, le);
  const Polyhedron& pph = *p_Polyhedron_val(ph);
  Linear_Expression ple = build_Linear_Expression(le);
  CAMLreturn(Val_bool(pph.bounds_from_below(ple)));
}
CATCH_ALL

extern "C"
CAMLprim value
ppl_Polyhedron_bounds_from_above(value ph, value le) try {
  CAMLparam2(ph, le);
  const Polyhedron& pph = *p_Polyhedron_val(ph);
  Linear_Expression ple = build_Linear_Expression(le);
  CAMLreturn(Val_bool(pph.bounds_from_above(ple)));
}
CATCH_ALL

extern "C"
void
ppl_Polyhedron_add_constraint(value ph, value c) try {
  CAMLparam2(ph, c);
  Polyhedron& pph = *p_Polyhedron_val(ph);
  Constraint pc = build_Constraint(c);
  pph.add_constraint(pc);
  CAMLreturn0;
}
CATCH_ALL

extern "C"
CAMLprim value
ppl_Polyhedron_add_constraint_and_minimize(value ph, value c) try {
  CAMLparam2(ph, c);
  Polyhedron& pph = *p_Polyhedron_val(ph);
  Constraint pc = build_Constraint(c);
  CAMLreturn(Val_bool(pph.add_constraint_and_minimize(pc)));
}
CATCH_ALL

extern "C"
void
ppl_Polyhedron_add_constraints(value ph, value cs) try {
  CAMLparam2(ph, cs);
  Polyhedron& pph = *p_Polyhedron_val(ph);
  Constraint_System pcs = build_Constraint_System(cs);
  pph.add_constraints(pcs);
  CAMLreturn0;
}
CATCH_ALL

extern "C"
CAMLprim value
ppl_Polyhedron_add_constraints_and_minimize(value ph, value cs) try {
  CAMLparam2(ph, cs);
  Polyhedron& pph = *p_Polyhedron_val(ph);
  Constraint_System pcs = build_Constraint_System(cs);
  CAMLreturn(Val_bool(pph.add_constraints_and_minimize(pcs)));
}
CATCH_ALL

extern "C"
void
ppl_Polyhedron_add_generator(value ph, value c) try {
  CAMLparam2(ph, c);
  Polyhedron& pph = *p_Polyhedron_val(ph);
  Generator pc = build_Generator(c);
  pph.add_generator(pc);
  CAMLreturn0;
}
CATCH_ALL

extern "C"
CAMLprim value
ppl_Polyhedron_add_generator_and_minimize(value ph, value c) try {
  CAMLparam2(ph, c);
  Polyhedron& pph = *p_Polyhedron_val(ph);
  Generator pc = build_Generator(c);
  CAMLreturn(Val_bool(pph.add_generator_and_minimize(pc)));
}
CATCH_ALL

extern "C"
void
ppl_Polyhedron_add_generators(value ph, value cs) try {
  CAMLparam2(ph, cs);
  Polyhedron& pph = *p_Polyhedron_val(ph);
  Generator_System pcs = build_Generator_System(cs);
  pph.add_generators(pcs);
  CAMLreturn0;
}
CATCH_ALL

extern "C"
CAMLprim value
ppl_Polyhedron_add_generators_and_minimize(value ph, value cs) try {
  CAMLparam2(ph, cs);
  Polyhedron& pph = *p_Polyhedron_val(ph);
  Generator_System pcs = build_Generator_System(cs);
  CAMLreturn(Val_bool(pph.add_generators_and_minimize(pcs)));
}
CATCH_ALL

extern "C"
void
ppl_Polyhedron_add_congruences(value ph, value c) try {
  CAMLparam2(ph, c);
  Polyhedron& pph = *p_Polyhedron_val(ph);
  Congruence_System pc = build_Congruence_System(c);
  pph.add_congruences(pc);
  CAMLreturn0;
}
CATCH_ALL

extern "C"
CAMLprim value
ppl_Polyhedron_is_disjoint_from(value ph1, value ph2) try {
  CAMLparam2(ph1, ph2);
  Polyhedron& pph1 = *p_Polyhedron_val(ph1);
  Polyhedron& pph2 = *p_Polyhedron_val(ph2);
  CAMLreturn(Val_bool(pph1.is_disjoint_from(pph2)));
}
CATCH_ALL

extern "C"
CAMLprim value
ppl_Polyhedron_contains(value ph1, value ph2) try {
  CAMLparam2(ph1, ph2);
  Polyhedron& pph1 = *p_Polyhedron_val(ph1);
  Polyhedron& pph2 = *p_Polyhedron_val(ph2);
  CAMLreturn(Val_bool(pph1.contains(pph2)));
}
CATCH_ALL

extern "C"
CAMLprim value
ppl_Polyhedron_strictly_contains(value ph1, value ph2) try {
  CAMLparam2(ph1, ph2);
  Polyhedron& pph1 = *p_Polyhedron_val(ph1);
  Polyhedron& pph2 = *p_Polyhedron_val(ph2);
  CAMLreturn(Val_bool(pph1.strictly_contains(pph2)));
}
CATCH_ALL

extern "C"
void
ppl_Polyhedron_intersection_assign(value ph1, value ph2) try {
  CAMLparam2(ph1, ph2);
  Polyhedron& pph1 = *p_Polyhedron_val(ph1);
  Polyhedron& pph2 = *p_Polyhedron_val(ph2);
  pph1.intersection_assign(pph2);
  CAMLreturn0;
}
CATCH_ALL

extern "C"
CAMLprim value
ppl_Polyhedron_intersection_assign_and_minimize(value ph1, value ph2) try {
  CAMLparam2(ph1, ph2);
  Polyhedron& pph1 = *p_Polyhedron_val(ph1);
  Polyhedron& pph2 = *p_Polyhedron_val(ph2);
  CAMLreturn(Val_bool(pph1.intersection_assign_and_minimize(pph2)));
}
CATCH_ALL

extern "C"
void
ppl_Polyhedron_poly_hull_assign(value ph1, value ph2) try {
  CAMLparam2(ph1, ph2);
  Polyhedron& pph1 = *p_Polyhedron_val(ph1);
  Polyhedron& pph2 = *p_Polyhedron_val(ph2);
  pph1.poly_hull_assign(pph2);
  CAMLreturn0;
}
CATCH_ALL

extern "C"
CAMLprim value
ppl_Polyhedron_poly_hull_assign_and_minimize(value ph1, value ph2) try {
  CAMLparam2(ph1, ph2);
  Polyhedron& pph1 = *p_Polyhedron_val(ph1);
  Polyhedron& pph2 = *p_Polyhedron_val(ph2);
  CAMLreturn(Val_bool(pph1.poly_hull_assign_and_minimize(pph2)));
}
CATCH_ALL


extern "C"
void
ppl_Polyhedron_upper_bound_assign(value ph1, value ph2) try {
  CAMLparam2(ph1, ph2);
  Polyhedron& pph1 = *p_Polyhedron_val(ph1);
  Polyhedron& pph2 = *p_Polyhedron_val(ph2);
  pph1.upper_bound_assign(pph2);
  CAMLreturn0;
}
CATCH_ALL

extern "C"
void
ppl_Polyhedron_poly_difference_assign(value ph1, value ph2) try {
  CAMLparam2(ph1, ph2);
  Polyhedron& pph1 = *p_Polyhedron_val(ph1);
  Polyhedron& pph2 = *p_Polyhedron_val(ph2);
  pph1.poly_difference_assign(pph2);
  CAMLreturn0;
}
CATCH_ALL

extern "C"
void
ppl_Polyhedron_difference_assign(value ph1, value ph2) try {
  CAMLparam2(ph1, ph2);
  Polyhedron& pph1 = *p_Polyhedron_val(ph1);
  Polyhedron& pph2 = *p_Polyhedron_val(ph2);
  pph1.difference_assign(pph2);
  CAMLreturn0;
}
CATCH_ALL

extern "C"
void
ppl_Polyhedron_time_elapse_assign(value ph1, value ph2) try {
  CAMLparam2(ph1, ph2);
  Polyhedron& pph1 = *p_Polyhedron_val(ph1);
  Polyhedron& pph2 = *p_Polyhedron_val(ph2);
  pph1.time_elapse_assign(pph2);
  CAMLreturn0;
}
CATCH_ALL

extern "C"
void
ppl_Polyhedron_concatenate_assign(value ph1, value ph2) try {
  CAMLparam2(ph1, ph2);
  Polyhedron& pph1 = *p_Polyhedron_val(ph1);
  Polyhedron& pph2 = *p_Polyhedron_val(ph2);
  pph1.concatenate_assign(pph2);
  CAMLreturn0;
}
CATCH_ALL

extern "C"
void
ppl_Polyhedron_ppl_Polyhedron_add_space_dimensions_and_embed(value ph,
							     value d) try {
  CAMLparam2(ph, d);
  int dd = Int_val(d);
  if (dd < 0)
    abort();
  Polyhedron& pph = *p_Polyhedron_val(ph);
  pph.add_space_dimensions_and_embed(dd);
  CAMLreturn0;
}
CATCH_ALL

extern "C"
void
ppl_Polyhedron_ppl_Polyhedron_add_space_dimensions_and_project(value ph,
							       value d) try {
  CAMLparam2(ph, d);
  int dd = Int_val(d);
  if (dd < 0)
    abort();
  Polyhedron& pph = *p_Polyhedron_val(ph);
  pph.add_space_dimensions_and_project(dd);
  CAMLreturn0;
}
CATCH_ALL

extern "C"
void
ppl_Polyhedron_ppl_Polyhedron_remove_higher_space_dimensions(value ph,
							     value d) try {
  CAMLparam2(ph, d);
  int dd = Int_val(d);
  if (dd < 0)
    abort();
  Polyhedron& pph = *p_Polyhedron_val(ph);
  pph.remove_higher_space_dimensions(dd);
  CAMLreturn0;
}
CATCH_ALL

extern "C"
CAMLprim void
test_linear_expression(value ocaml_le) {
  CAMLparam1(ocaml_le);
  Linear_Expression cxx_le = build_Linear_Expression(ocaml_le);
  std::cout << cxx_le << std::endl;
  CAMLreturn0;
}

extern "C"
CAMLprim void
test_linear_constraint(value ocaml_c) {
  CAMLparam1(ocaml_c);
  Constraint cxx_c = build_Constraint(ocaml_c);
  std::cout << cxx_c << std::endl;
  CAMLreturn0;
}

extern "C"
CAMLprim void
test_constraint_system(value cl) {
  CAMLparam1(cl);
  Constraint_System cs = build_Constraint_System(cl);
  std::cout << cs << std::endl;
  CAMLreturn0;
}

extern "C"
CAMLprim void
test_linear_generator(value ocaml_g) {
  CAMLparam1(ocaml_g);
  Generator cxx_g = build_Generator(ocaml_g);
  std::cout << cxx_g << std::endl;
  CAMLreturn0;
}

extern "C"
CAMLprim void
test_generator_system(value gl) {
  CAMLparam1(gl);
  Generator_System gs = build_Generator_System(gl);
  std::cout << gs << std::endl;
  CAMLreturn0;
}

#if 0
void
ppl_error_out_of_memory() {
  caml_raise_out_of_memory();
}

void
ppl_error_invalid_argument() {
  caml_raise_constant(*caml_named_value("invalid_argument"));
}

void
ppl_arithmetic_overflow() {
  caml_raise_constant(*caml_named_value("arithmetic_overflow"));
}

void
ppl_stdio_error() {
  caml_raise_constant(*caml_named_value("stdio_error"));
}

void
ppl_internal_error() {
  caml_raise_constant(*caml_named_value("internal_error"));
}

void
ppl_unknow_standard_exception() {
  caml_raise_constant(*caml_named_value("standard_exception"));
}

void
ppl_error_unexpected_error() {
  caml_raise_constant(*caml_named_value("unexpected_error"));
}
#endif
