/* Test Polyhedron::BBRZ02_widening_assign(): the number of closure
   points of ph2 is greater of the the number of closure points of ph1.
   Copyright (C) 2001, 2002 Roberto Bagnara <bagnara@cs.unipr.it>

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
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
USA.

For the most up-to-date information see the Parma Polyhedra Library
site: http://www.cs.unipr.it/ppl/ . */

#include "ppl_test.hh"

using namespace std;
using namespace Parma_Polyhedra_Library;

#ifndef NOISY
#define NOISY 0
#endif

int 
main () {
  set_handlers();

  Variable A(0);
  Variable B(1);

  GenSys gs1;
  gs1.insert(closure_point());
  gs1.insert(closure_point(A + B));
  gs1.insert(point(2*A + B, 2));
  gs1.insert(ray(A));
  NNC_Polyhedron ph1(gs1);

  GenSys gs2;
  gs2.insert(closure_point());
  gs2.insert(closure_point(A + B));
  gs2.insert(closure_point(B, 2));
  gs2.insert(point(2*A + B, 2));
  gs2.insert(ray(A));
  NNC_Polyhedron ph2(gs2);

#if NOISY
  print_generators(ph1, "*** ph1 ***");
  print_generators(ph2, "*** ph2 ***");
#endif

  ph2.BBRZ02_widening_assign(ph1);

  NNC_Polyhedron known_result(2);
  known_result.add_constraint(B > 0);
  known_result.add_constraint(B < 1);

  int retval = (ph2 == known_result) ? 0 : 1;

#if NOISY
  print_constraints(ph2, "*** After  ph2.BBRZ02_widening_assign(ph1) ***");
#endif

  return retval;
}
