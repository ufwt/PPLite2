/* Constraint class implementation (non-inline functions).
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2011 BUGSENG srl (http://bugseng.com)

This file is part of the Parma Polyhedra Library (PPL).

The PPL is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
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

#include <ppl-config.h>

#include "Constraint.defs.hh"
#include "Variable.defs.hh"
#include "Variables_Set.defs.hh"
#include "Congruence.defs.hh"
#include "math_utilities.defs.hh"

#include <iostream>
#include <sstream>
#include <stdexcept>

namespace PPL = Parma_Polyhedra_Library;

void
PPL::Constraint::throw_invalid_argument(const char* method,
					const char* message) const {
  std::ostringstream s;
  s << "PPL::Constraint::" << method << ":" << std::endl
    << message;
  throw std::invalid_argument(s.str());
}

void
PPL::Constraint::throw_dimension_incompatible(const char* method,
					      const char* name_var,
					      const Variable v) const {
  std::ostringstream s;
  s << "PPL::Constraint::" << method << ":" << std::endl
    << "this->space_dimension() == " << space_dimension() << ", "
    << name_var << ".space_dimension() == " << v.space_dimension() << ".";
  throw std::invalid_argument(s.str());
}

PPL::Constraint
PPL::Constraint::construct_epsilon_geq_zero() {
  Linear_Expression e = Variable(0);
  return Constraint(e, NONSTRICT_INEQUALITY, NOT_NECESSARILY_CLOSED);
}

PPL::Constraint::Constraint(const Congruence& cg)
  : expr(),
    kind_(LINE_OR_EQUALITY),
    topology_(NECESSARILY_CLOSED) {
  if (!cg.is_equality())
    throw_invalid_argument("Constraint(cg)",
                           "congruence cg must be an equality.");
  expr.set_space_dimension(cg.space_dimension());
  // Copy coefficients and inhomogeneous term.
  for (dimension_type i = cg.space_dimension(); i-- > 0; )
    expr.set_coefficient(Variable(i), cg.coefficient(Variable(i)));
  expr.set_inhomogeneous_term(cg.inhomogeneous_term());
  // Enforce normalization.
  strong_normalize();
  
  PPL_ASSERT(OK());
}

PPL::Constraint::Constraint(const Congruence& cg,
			    dimension_type sz,
			    dimension_type /* capacity */)
  : expr(),
    kind_(LINE_OR_EQUALITY),
    topology_(NECESSARILY_CLOSED) {
  PPL_ASSERT(sz != 0);
  if (!cg.is_equality())
    throw_invalid_argument("Constraint(cg)",
                           "congruence cg must be an equality.");
  expr.set_space_dimension(sz - 1);
  // Copy coefficients.
  PPL_ASSERT(sz > 0);
  --sz;
  while (sz-- > 0)
    expr.set_coefficient(Variable(sz), cg.coefficient(Variable(sz)));
  expr.set_inhomogeneous_term(cg.inhomogeneous_term());
  
  PPL_ASSERT(OK());
}

void
PPL::Constraint::swap_space_dimensions(Variable v1, Variable v2) {
  PPL_ASSERT(v1.space_dimension() <= space_dimension());
  PPL_ASSERT(v2.space_dimension() <= space_dimension());
  expr.swap_space_dimensions(v1, v2);
  // *this is still normalized but it may not be strongly normalized.
  sign_normalize();
  PPL_ASSERT(OK());
}

void
PPL::Constraint
::permute_space_dimensions(const std::vector<Variable>& cycle) {

  if (cycle.size() < 2)
    // No-op. No need to call sign_normalize().
    return;

  expr.permute_space_dimensions(cycle);
  
  // *this is still normalized but may be not strongly normalized: sign
  // normalization is necessary.
  sign_normalize();
  PPL_ASSERT(OK());
}

bool
PPL::Constraint::is_tautological() const {
  if (expr.all_homogeneous_terms_are_zero())
    if (is_equality())
      return expr.inhomogeneous_term() == 0;
    else
      // Non-strict inequality constraint.
      return expr.inhomogeneous_term() >= 0;
  else
    // There is a non-zero homogeneous coefficient.
    if (is_necessarily_closed())
      return false;
    else {
      // The constraint is NOT necessarily closed.
      const dimension_type eps_index = expr.space_dimension();
      PPL_ASSERT(eps_index != 0);
      const int eps_sign = sgn(expr.coefficient(Variable(eps_index - 1)));
      if (eps_sign > 0)
	// We have found the constraint epsilon >= 0.
	return true;
      if (eps_sign == 0)
	// One of the `true' dimensions has a non-zero coefficient.
	return false;
      else {
	// Here the epsilon coefficient is negative: strict inequality.
	if (expr.inhomogeneous_term() <= 0)
	  // A strict inequality such as `lhs - k > 0',
	  // where k is a non negative integer, cannot be trivially true.
	  return false;
	// Checking for another non-zero coefficient.
        // TODO: Optimize this.
	for (dimension_type i = eps_index - 1; i-- > 0; )
	  if (expr.coefficient(Variable(i)) != 0)
	    return false;
	// We have the inequality `k > 0',
	// where k is a positive integer.
	return true;
      }
    }
}

bool
PPL::Constraint::is_inconsistent() const {
  if (expr.all_homogeneous_terms_are_zero())
    // The inhomogeneous term is the only non-zero coefficient.
    if (is_equality())
      return expr.inhomogeneous_term() != 0;
    else
      // Non-strict inequality constraint.
      return expr.inhomogeneous_term() < 0;
  else
    // There is a non-zero homogeneous coefficient.
    if (is_necessarily_closed())
      return false;
    else {
      // The constraint is NOT necessarily closed.
      const dimension_type eps_index = expr.space_dimension();
      PPL_ASSERT(eps_index != 0);
      if (expr.coefficient(Variable(eps_index - 1)) >= 0)
	// If positive, we have found the constraint epsilon >= 0.
	// If zero, one of the `true' dimensions has a non-zero coefficient.
	// In both cases, it is not trivially false.
	return false;
      else {
	// Here the epsilon coefficient is negative: strict inequality.
	if (expr.inhomogeneous_term() > 0)
	  // A strict inequality such as `lhs + k > 0',
	  // where k is a positive integer, cannot be trivially false.
	  return false;
	// Checking for another non-zero coefficient.
        // TODO: Optimize this.
	for (dimension_type i = eps_index - 1; i-- > 0; )
	  if (expr.coefficient(Variable(i)) != 0)
	    return false;
	// We have the inequality `k > 0',
	// where k is zero or a negative integer.
	return true;
      }
    }
}

void
PPL::Constraint::linear_combine(const Constraint& y, Variable v) {
  expr.linear_combine(y.expr, v);
  strong_normalize();
}

void
PPL::Constraint::linear_combine_inhomogeneous(const Constraint& y) {
  expr.linear_combine_inhomogeneous(y.expr);
  strong_normalize();
}

/*! \relates Parma_Polyhedra_Library::Constraint */
int
PPL::compare(const Constraint& x, const Constraint& y) {
  const bool x_is_line_or_equality = x.is_line_or_equality();
  const bool y_is_line_or_equality = y.is_line_or_equality();
  if (x_is_line_or_equality != y_is_line_or_equality)
    // Equalities (lines) precede inequalities (ray/point).
    return y_is_line_or_equality ? 2 : -2;

  return compare(x.expr, y.expr);
}

bool
PPL::Constraint::is_equivalent_to(const Constraint& y) const {
  const Constraint& x = *this;
  const dimension_type x_space_dim = x.space_dimension();
  if (x_space_dim != y.space_dimension())
    return false;

  const Type x_type = x.type();
  if (x_type != y.type()) {
    // Check for special cases.
    if (x.is_tautological())
      return y.is_tautological();
    else
      return x.is_inconsistent() && y.is_inconsistent();
  }

  if (x_type == STRICT_INEQUALITY) {
    // Due to the presence of epsilon-coefficients, syntactically
    // different strict inequalities may actually encode the same
    // topologically open half-space.
    // First, drop the epsilon-coefficient ...
    Linear_Expression x_expr(x);
    Linear_Expression y_expr(y);
    // ... then, re-normalize ...
    x_expr.normalize();
    y_expr.normalize();
    // ... and finally check for syntactic equality.
    return x_expr.is_equal_to(y_expr);
  }

  // `x' and 'y' are of the same type and they are not strict inequalities;
  // thus, the epsilon-coefficient, if present, is zero.
  // It is sufficient to check for syntactic equality.
  return x.expr.is_equal_to(y.expr);
}

bool
PPL::Constraint::is_equal_to(const Constraint& y) const {
  return expr.is_equal_to(y.expr) && kind_ == y.kind_ && topology() == y.topology();
}

void
PPL::Constraint::sign_normalize() {
  if (is_line_or_equality()) {
    // TODO: Consider moving this code into a new method of Linear_Expression.
    // TODO: Use the space dimension instead of the size.
    const dimension_type sz = expr.space_dimension() + 1;
    // `first_non_zero' indicates the index of the first
    // coefficient of the row different from zero, disregarding
    // the very first coefficient (inhomogeneous term / divisor).
    dimension_type first_non_zero;
    // TODO: Optimize this.
    for (first_non_zero = 1; first_non_zero < sz; ++first_non_zero)
      if (expr.coefficient(Variable(first_non_zero - 1)) != 0)
        break;
    if (first_non_zero < sz)
      // If the first non-zero coefficient of the row is negative,
      // we negate the entire row.
      if (expr.coefficient(Variable(first_non_zero - 1)) < 0) {
        // TODO: Consider optimizing this.
        // We know that the first `first_non_zero' elements are nonzero
        // (except at most the inhomogeneous term), so we could exploit this
        // and avoid some work.
        neg_assign(expr);
      }
  }
}

bool
PPL::Constraint::check_strong_normalized() const {
  Constraint tmp = *this;
  tmp.strong_normalize();
  return compare(*this, tmp) == 0;
}

const PPL::Constraint* PPL::Constraint::zero_dim_false_p = 0;
const PPL::Constraint* PPL::Constraint::zero_dim_positivity_p = 0;
const PPL::Constraint* PPL::Constraint::epsilon_geq_zero_p = 0;
const PPL::Constraint* PPL::Constraint::epsilon_leq_one_p = 0;

void
PPL::Constraint::initialize() {
  PPL_ASSERT(zero_dim_false_p == 0);
  zero_dim_false_p
    = new Constraint(Linear_Expression::zero() == Coefficient_one());

  PPL_ASSERT(zero_dim_positivity_p == 0);
  zero_dim_positivity_p
    = new Constraint(Linear_Expression::zero() <= Coefficient_one());

  PPL_ASSERT(epsilon_geq_zero_p == 0);
  epsilon_geq_zero_p
    = new Constraint(construct_epsilon_geq_zero());

  PPL_ASSERT(epsilon_leq_one_p == 0);
  epsilon_leq_one_p
    = new Constraint(Linear_Expression::zero() < Coefficient_one());
}

void
PPL::Constraint::finalize() {
  PPL_ASSERT(zero_dim_false_p != 0);
  delete zero_dim_false_p;
  zero_dim_false_p = 0;

  PPL_ASSERT(zero_dim_positivity_p != 0);
  delete zero_dim_positivity_p;
  zero_dim_positivity_p = 0;

  PPL_ASSERT(epsilon_geq_zero_p != 0);
  delete epsilon_geq_zero_p;
  epsilon_geq_zero_p = 0;

  PPL_ASSERT(epsilon_leq_one_p != 0);
  delete epsilon_leq_one_p;
  epsilon_leq_one_p = 0;
}

void
PPL::Constraint::ascii_dump(std::ostream& s) const {
  s << "size " << (expr.space_dimension() + 1) << " ";
  for (dimension_type j = 0; j < expr.get_row().size(); ++j)
    s << expr.get_row()[j] << ' ';
  switch (type()) {
  case Constraint::EQUALITY:
    s << "=";
    break;
  case Constraint::NONSTRICT_INEQUALITY:
    s << ">=";
    break;
  case Constraint::STRICT_INEQUALITY:
    s << ">";
    break;
  }
  s << " ";
  if (topology() == NECESSARILY_CLOSED)
    s << "(C)";
  else
    s << "(NNC)";

  s << "\n";
}

bool
PPL::Constraint::ascii_load(std::istream& s) {
  std::string str;
  std::string str2;

  if (!(s >> str))
    return false;
  if (str != "size")
    return false;

  dimension_type new_size;
  if (!(s >> new_size))
    return false;

  expr.get_row().resize(new_size);

  for (dimension_type j = 0; j < new_size; ++j)
    if (!(s >> expr.get_row()[j]))
      return false;

  if (!(s >> str))
    return false;
  if (str == "=")
    set_is_equality();
  else if (str == ">=" || str == ">")
    set_is_inequality();
  else
    return false;

  if (!(s >> str2))
    return false;
  if (str2 == "(NNC)") {
    // TODO: Avoid the mark_as_*() methods if possible.
    if (topology() == NECESSARILY_CLOSED)
      mark_as_not_necessarily_closed();
  } else
    if (str2 == "(C)") {
      // TODO: Avoid the mark_as_*() methods if possible.
      if (topology() == NOT_NECESSARILY_CLOSED)
        mark_as_necessarily_closed();
    } else
      return false;

  // Checking for equality of actual and declared types.
  switch (type()) {
  case EQUALITY:
    if (str != "=")
      return false;
    break;
  case NONSTRICT_INEQUALITY:
    if (str != ">=")
      return false;
    break;
  case STRICT_INEQUALITY:
    if (str != ">")
      return false;
    break;
  }

  return true;
}

/*! \relates Parma_Polyhedra_Library::Constraint */
std::ostream&
PPL::IO_Operators::operator<<(std::ostream& s, const Constraint& c) {
  const dimension_type num_variables = c.space_dimension();
  PPL_DIRTY_TEMP_COEFFICIENT(cv);
  bool first = true;
  for (dimension_type v = 0; v < num_variables; ++v) {
    cv = c.coefficient(Variable(v));
    if (cv != 0) {
      if (!first) {
	if (cv > 0)
	  s << " + ";
	else {
	  s << " - ";
	  neg_assign(cv);
	}
      }
      else
	first = false;
      if (cv == -1)
	s << "-";
      else if (cv != 1)
	s << cv << "*";
      s << PPL::Variable(v);
    }
  }
  if (first)
    s << Coefficient_zero();
  const char* relation_symbol = 0;
  switch (c.type()) {
  case Constraint::EQUALITY:
    relation_symbol = " = ";
    break;
  case Constraint::NONSTRICT_INEQUALITY:
    relation_symbol = " >= ";
    break;
  case Constraint::STRICT_INEQUALITY:
    relation_symbol = " > ";
    break;
  }
  s << relation_symbol << -c.inhomogeneous_term();
  return s;
}

/*! \relates Parma_Polyhedra_Library::Constraint */
std::ostream&
PPL::IO_Operators::operator<<(std::ostream& s, const Constraint::Type& t) {
  const char* n = 0;
  switch (t) {
  case Constraint::EQUALITY:
    n = "EQUALITY";
    break;
  case Constraint::NONSTRICT_INEQUALITY:
    n = "NONSTRICT_INEQUALITY";
    break;
  case Constraint::STRICT_INEQUALITY:
    n = "STRICT_INEQUALITY";
    break;
  }
  s << n;
  return s;
}

PPL_OUTPUT_DEFINITIONS(Constraint)

bool
PPL::Constraint::OK() const {
  // Topology consistency checks.
  const dimension_type min_space_dim = is_necessarily_closed() ? 0 : 1;
  if (expr.space_dimension() < min_space_dim) {
#ifndef NDEBUG
    std::cerr << "Constraint has fewer coefficients than the minimum "
	      << "allowed by its topology:"
	      << std::endl
	      << "space dimension is " << expr.space_dimension()
	      << ", minimum is " << min_space_dim << "."
	      << std::endl;
#endif
    return false;
  }

  if (is_equality() && !is_necessarily_closed()
      && expr.coefficient(Variable(expr.space_dimension() - 1)) != 0) {
#ifndef NDEBUG
    std::cerr << "Illegal constraint: an equality cannot be strict."
	      << std::endl;
#endif
    return false;
  }

  // Normalization check.
  Constraint tmp = *this;
  tmp.strong_normalize();
  if (tmp != *this) {
#ifndef NDEBUG
    std::cerr << "Constraint is not strongly normalized as it should be."
	      << std::endl;
#endif
    return false;
  }

  // All tests passed.
  return true;
}
