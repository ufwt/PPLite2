/* Multiplication_Floating_Point_Expression class implementation:
   non-inline template functions.
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>

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

#ifndef PPL_Multiplication_Floating_Point_Expression_templates_hh
#define PPL_Multiplication_Floating_Point_Expression_templates_hh 1

namespace Parma_Polyhedra_Library {

template <typename FP_Interval_Type, typename FP_Format>
void Multiplication_Floating_Point_Expression<FP_Interval_Type, FP_Format>
::linearize(const FP_Interval_Abstract_Store& int_store,
            const FP_Linear_Form_Abstract_Store& lf_store,
            FP_Linear_Form& result) const {
  /*
    FIXME: we currently adopt the Interval-Size Local strategy in order to
    decide which of the two linear forms must be intervalized, as described
    in section 6.2.4 of Antoine Mine's thesis. We should also consider more
    refined strategies.
  */

  // Here we choose which of the two linear forms must be intervalized.

  // true if we intervalize the first form, false if we intervalize the second.
  bool intervalize_first;
  FP_Linear_Form linearized_first_operand;
  first_operand->linearize(int_store, lf_store, linearized_first_operand);
  FP_Interval_Type intervalized_first_operand;
  this->intervalize(linearized_first_operand, int_store,
                    intervalized_first_operand);
  FP_Linear_Form linearized_second_operand;
  second_operand->linearize(int_store, lf_store, linearized_second_operand);
  FP_Interval_Type intervalized_second_operand;
  this->intervalize(linearized_second_operand, int_store,
                    intervalized_second_operand);
  boundary_type first_interval_size, second_interval_size;

  // FIXME: we are not sure that what we do here is policy-proof.
  if (intervalized_first_operand.is_bounded()) {
    if (intervalized_second_operand.is_bounded()) {
      first_interval_size = intervalized_first_operand.upper() -
                            intervalized_first_operand.lower();
      second_interval_size = intervalized_second_operand.upper() -
                             intervalized_second_operand.lower();
      if (first_interval_size <= second_interval_size)
        intervalize_first = true;
      else
        intervalize_first = false;
    }
    else {
      if (this->overflows(linearized_second_operand))
        throw Linearization_Failed();
      intervalize_first = true;
    }
  }
  else {
    if (intervalized_second_operand.is_bounded()) {
      if (this->overflows(linearized_first_operand))
        throw Linearization_Failed();
      intervalize_first = false;
    }
    else
      throw Linearization_Failed();
  }

  // Here we do the actual computation.
  // For optimizing, we store the relative error directly into result.
  if (intervalize_first) {
    relative_error(linearized_second_operand, result);
    linearized_second_operand *= intervalized_first_operand;
    result *= intervalized_first_operand;
    result += linearized_second_operand;
  }
  else {
    relative_error(linearized_first_operand, result);
    linearized_first_operand *= intervalized_second_operand;
    result *= intervalized_second_operand;
    result += linearized_first_operand;
  }

  FP_Interval_Type abs_error(-this->absolute_error);
  // FIXME: this may be incorrect for some policies.
  abs_error.join_assign(this->absolute_error);
  result += abs_error;
  return;
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Multiplication_Floating_Point_Expression_templates_hh)