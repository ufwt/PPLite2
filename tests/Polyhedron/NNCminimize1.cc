/* Full minimization of a NNC-redundant constraint system.
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

  Variable x(0);
  Variable y(1);

  // Building a square.
  Constraint_System cs;
  cs.insert(x >= 0);
  cs.insert(x <= 6);
  cs.insert(y >= 0);
  cs.insert(y <= 6);

  NNC_Polyhedron ph(cs);

  nout << "Topologically closed square" << endl;
  print_constraints(ph.constraints(), "*** ph constraints ***");
  print_generators(ph.generators(), "*** ph generators ***");

  // Removing all the vertices using strict inequalities.
  cs.clear();
  cs.insert(x + y > 0);
  cs.insert(x + y < 12);
  cs.insert(x - y < 6);
  cs.insert(x - y > -6);

  ph.add_constraints_and_minimize(cs);

  nout << "After vertices removal" << endl;
  print_constraints(ph.constraints(), "*** ph constraints ***");
  print_generators(ph.generators(), "*** ph generators ***");

  ph.minimized_generators();

  Generator_System gs;
  gs.insert(closure_point());
  gs.insert(closure_point(6*x));
  gs.insert(closure_point(6*y));
  gs.insert(closure_point(6*x + 6*y));
  gs.insert(point(3*x));
  gs.insert(point(3*y));
  gs.insert(point(3*x + 6*y));
  gs.insert(point(6*x + 3*y));

  NNC_Polyhedron known_result(gs);

  bool equal = (ph == known_result);

  nout << "After NNC minimization" << endl;
  print_constraints(ph.constraints(), "*** ph constraints ***");
  print_generators(ph.generators(), "*** ph generators ***");

  // FIXME: find a way to correctly check if the output
  // is strongly minimized.
  // return (equal && ph.generators().num_rows() == 8) ? 0 : 1;

  return equal ? 0 : 1;
}
CATCH
