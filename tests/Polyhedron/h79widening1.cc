/* Test Polyhedron::H79_widening_assign() with empty polyhedra.
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

#include "ppl_test.hh"

using namespace std;
using namespace Parma_Polyhedra_Library;

namespace {

bool
try_H79_widening_assign(C_Polyhedron& ph1, const C_Polyhedron& ph2,
			// Note intentional call-by-value!
			C_Polyhedron known_result) {
  print_constraints(ph1, "*** ph1 ***");
  print_constraints(ph2, "*** ph2 ***");

  ph1.H79_widening_assign(ph2);

  print_generators(ph1, "*** After H79_widening_assign ***");

  return ph1 == known_result;
}

} // namespace

int
main() TRY {
  set_handlers();

  Variable x(0);
  Variable y(1);

  C_Polyhedron ph1_1(2);
  ph1_1.add_constraint(x >= 0);
  ph1_1.add_constraint(y >= 0);
  ph1_1.add_constraint(x <= 2);
  ph1_1.add_constraint(y <= 2);
  C_Polyhedron ph1_2(ph1_1);

  C_Polyhedron ph2_1(2);
  ph2_1.add_constraint(x+y <= 0);
  ph2_1.add_constraint(x+y >= 2);
  C_Polyhedron ph2_2(ph2_1);
  C_Polyhedron ph2_3(ph2_1);
  C_Polyhedron ph2_4(ph2_1);

  if (!try_H79_widening_assign(ph1_1, ph2_1, ph1_1))
    return 1;


  // FIXME: this must be reactivated (in some form) when we will
  //        have a decent error policy for H79_widening_assign().
#if 0
  if (!try_H79_widening_assign(ph2_2, ph1_2, ph1_2))
    return 1;
#endif

  if (!try_H79_widening_assign(ph2_3, ph2_4, ph2_3))
    return 1;

  return 0;
}
CATCH
