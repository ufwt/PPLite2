/* Test Polyhedron::add_space_dimensions_and_embed(): shows a bug in versions
   up to 0.5pre12 caused by the missing protection against auto-assignments
   in class Matrix.
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

int
main() TRY {
  set_handlers();

  Variable A(0);

  C_Polyhedron ph1(2);
  ph1.add_constraint(A >= 0);
  ph1.add_constraint(A <= 2);

  // This will change the size of the rows, but not their capacity.
  ph1.add_space_dimensions_and_embed(1);

  // Assigning the polyhedron to itself:
  // this used to recompute the row capacity based on row size,
  // without actually increasing the capacity of the rows,
  // leading to an inconsistent state.
  ph1 = ph1;

  (void) ph1.OK();

  return 0;
}
CATCH
