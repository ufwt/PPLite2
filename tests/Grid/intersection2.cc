/* Test Grid::intersection_assign_and_minimize.
   Copyright (C) 2005 Roberto Bagnara <bagnara@cs.unipr.it>

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

using namespace Parma_Polyhedra_Library::IO_Operators;

#define find_variation find_variation_template<Grid>

// Simple grids, one dimension.

void
test1() {
  nout << "test1:" << endl;

  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs1;
  cgs1.insert((A + 0*C %= 0) / 2);

  Congruence_System cgs2;
  cgs2.insert((2*A + 0*C %= 0) / 2);

  Grid gr1(cgs1);
  Grid gr2(cgs2);

  gr1.intersection_assign_and_minimize(gr2);

  if (find_variation(gr1))
    exit(1);

  Congruence_System known_cgs;
  known_cgs.insert((A + 0*C %= 0) / 2);
  known_cgs.insert((2*A %= 0) / 2);

  Grid known_gr(known_cgs);

  if (gr1 == known_gr)
    return;

  nout << "Grid should equal known grid." << endl
       << " grid:" << endl << gr1 << endl
       << "known:" << endl << known_gr << endl;

  exit(1);
}

// First grid empty.

void
test2() {
  nout << "test2:" << endl;

  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((A + B + C %= 0) / 2);

  Grid gr1(3, Grid::EMPTY);
  Grid gr2(cgs);

  gr1.intersection_assign_and_minimize(gr2);

  if (find_variation(gr1))
    exit(1);

  Grid known_gr(3, Grid::EMPTY);

  if (gr1 == known_gr)
    return;

  nout << "Grid should equal known grid." << endl
       << " grid:" << endl << gr1 << endl
       << "known:" << endl << known_gr << endl;

  exit(1);
}

// Second grid empty.

void
test3() {
  nout << "test3:" << endl;

  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((A + B + C %= 0) / 2);

  Grid gr1(cgs);
  Grid gr2(3, Grid::EMPTY);

  gr1.intersection_assign_and_minimize(gr2);

  if (find_variation(gr1))
    exit(1);

  Grid known_gr(3, Grid::EMPTY);

  if (gr1 == known_gr)
    return;

  nout << "Grid should equal known grid." << endl
       << " grid:" << endl << gr1 << endl
       << "known:" << endl << known_gr << endl;

  exit(1);
}

// First grid universe.

void
test4() {
  nout << "test4:" << endl;

  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((  A +   B + C %= 7) / 9);
  cgs.insert((5*A + 3*B + C %= 7) / 9);

  Grid gr1(3, Grid::UNIVERSE);
  Grid gr2(cgs);

  gr1.intersection_assign_and_minimize(gr2);

  if (find_variation(gr1))
    exit(1);

  Congruence_System known_cgs;
  known_cgs.insert((  A +   B + C %= 7) / 9);
  known_cgs.insert((5*A + 3*B + C %= 7) / 9);

  Grid known_gr(known_cgs);

  if (gr1 == known_gr)
    return;

  nout << "Grid should equal known grid." << endl
       << " grid:" << endl << gr1 << endl
       << "known:" << endl << known_gr << endl;

  exit(1);
}

// Second grid universe.

void
test5() {
  nout << "test5:" << endl;

  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((2*B + 2*C %= 1) / 3);
  cgs.insert((2*B %= 1) / 3);

  Grid gr1(cgs);
  Grid gr2(3, Grid::UNIVERSE);

  gr1.intersection_assign_and_minimize(gr2);

  if (find_variation(gr1))
    exit(1);

  Congruence_System known_cgs;
  known_cgs.insert((2*B + 2*C %= 1) / 3);
  known_cgs.insert((2*B %= 1) / 3);

  Grid known_gr(known_cgs);

  if (gr1 == known_gr)
    return;

  nout << "Grid should equal known grid." << endl
       << " grid:" << endl << gr1 << endl
       << "known:" << endl << known_gr << endl;

  exit(1);
}

// Zero dimension grids.

void
test6() {
  nout << "test6:" << endl;

  Grid gr1(0);
  Grid gr2(0);

  gr1.intersection_assign_and_minimize(gr2);

  if (find_variation(gr1))
    exit(1);

  Grid known_gr(0);

  if (gr1 == known_gr)
    return;

  nout << "Grid should equal known grid." << endl
       << " grid:" << endl << gr1 << endl
       << "known:" << endl << known_gr << endl;

  exit(1);
}

// Many dimension grids from generators.

void
test7() {
  nout << "test7:" << endl;

  // FIX same grids as intersection1 test8

  Variable A(0);
  Variable B(1);
  Variable C(2);

  Generator_System gs1;
  gs1.insert(point(A + C));
  gs1.insert( line(C));

  Generator_System gs2;
  gs2.insert(point(A));
  gs2.insert(point(A + B));

  Grid gr1(gs1);

  Grid gr2(3, Grid::EMPTY);
  gr2.add_generators(gs2);

  gr1.intersection_assign_and_minimize(gr2);

  if (find_variation(gr1))
    exit(1);

  Congruence_System known_cgs;
  known_cgs.insert((C == 0) / 0);
  known_cgs.insert((A == 1) / 0);
  known_cgs.insert((B == 0) / 0);

  Grid known_gr(known_cgs);

  if (gr1 == known_gr)
    return;

  nout << "Grid should equal known grid." << endl
       << " grid:" << endl << gr1 << endl
       << "known:" << endl << known_gr << endl;

  exit(1);
}

int
main() TRY {
  set_handlers();

  nout << "intersection2:" << endl;

  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();

  return 0;
}
CATCH
