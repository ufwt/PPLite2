/* Tests Grid manipulation methods.
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

// intersection_assign

void
test1() {
  nout << "test1:" << endl;

  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs1;
  cgs1.insert((A + 0*C %= 0) / 2);

  Congruence_System cgs2;
  cgs2.insert((B + 0*C %= 0) / 2);

  Grid gr1(cgs1);
  Grid gr2(cgs2);

  if (find_variation(gr1) || find_variation(gr2))
    exit(1);

  gr1.intersection_assign(gr2);

  if (find_variation(gr1))
    exit(1);

  Congruence_System known_cgs;
  known_cgs.insert((A + 0*C %= 0) / 2);
  known_cgs.insert((B %= 0) / 2);

  Grid known_gr(known_cgs);

  if (find_variation(known_gr))
    exit(1);

  if (gr1 == known_gr)
    return;

  nout << "Grid should equal known grid." << endl
       << " grid:" << endl << gr1 << endl
       << "known:" << endl << known_gr << endl;

  exit(1);
}

// intersection_assign_and_minimize

void
test2() {
  nout << "test2:" << endl;

  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs1;
  cgs1.insert((A + 0*C %= 0) / 2);

  Congruence_System cgs2;
  cgs2.insert((2*A + 0*C %= 0) / 2);

  Grid gr1(cgs1);
  Grid gr2(cgs2);

  if (find_variation(gr1) || find_variation(gr2))
    exit(1);

  gr1.intersection_assign_and_minimize(gr2);

  if (find_variation(gr1))
    exit(1);

  Congruence_System known_cgs;
  known_cgs.insert((A + 0*C %= 0) / 2);
  known_cgs.insert((2*A %= 0) / 2);

  Grid known_gr(known_cgs);

  if (find_variation(known_gr))
    exit(1);

  if (gr1 == known_gr)
    return;

  nout << "Grid should equal known grid." << endl
       << " grid:" << endl << gr1 << endl
       << "known:" << endl << known_gr << endl;

  exit(1);
}

// join_assign

void
test3() {
  nout << "test3:" << endl;

  Variable A(0);
  Variable B(1);
  Variable C(2);

  Generator_System gs1;
  gs1.insert(point(C));

  Generator_System gs2;
  gs2.insert(point(B + 0*C));

  Grid gr1(gs1);
  Grid gr2(gs2);

  if (find_variation(gr1) || find_variation(gr2))
    exit(1);

  gr1.join_assign(gr2);

  if (find_variation(gr1))
    exit(1);

  Generator_System known_gs;
  known_gs.insert(point(C));
  known_gs.insert(point(B));

  Grid known_gr(known_gs);

  if (find_variation(known_gr))
    exit(1);

  if (gr1 == known_gr)
    return;

  nout << "Grid should equal known grid." << endl
       << " grid:" << endl << gr1 << endl
       << "known:" << endl << known_gr << endl;

  exit(1);
}

// join_assign_and_minimize

void
test4() {
  nout << "test4:" << endl;

  Variable A(0);
  Variable B(1);
  Variable C(2);

  Generator_System gs1;
  gs1.insert(point(A + 0*C));

  Generator_System gs2;
  gs2.insert(point(2*A + 0*C));

  Grid gr1(gs1);
  Grid gr2(gs2);

  if (find_variation(gr1) || find_variation(gr2))
    exit(1);

  gr1.join_assign_and_minimize(gr2);

  if (find_variation(gr1))
    exit(1);

  Generator_System known_gs;
  known_gs.insert(point(A + 0*C));
  known_gs.insert(point(2*A));

  Grid known_gr(known_gs);

  if (find_variation(known_gr))
    exit(1);

  if (gr1 == known_gr)
    return;

  nout << "Grid should equal known grid." << endl
       << " grid:" << endl << gr1 << endl
       << "known:" << endl << known_gr << endl;

  exit(1);
}

#if 0
// add_congruences_and_minimize, one dimension.

void
test3() {
  nout << "test3:" << endl;

  Variable A(0);

  Congruence_System cgs;
  cgs.insert((A %= 0) / 3);

  Grid gr(1);
  gr.add_congruences_and_minimize(cgs);

  if (find_variation(gr))
    exit(1);

  Generator_System known_gs;
  known_gs.insert(point(0*A));
  known_gs.insert(point(3*A));

  Grid known_gr(known_gs);

  if (find_variation(known_gr))
    exit(1);

  if (gr == known_gr)
    return;

  nout << "Grid should equal known grid." << endl
       << "grid:" << endl << gr << endl
       << "known grid:" << endl << known_gr << endl;
  exit(1);
}

// add_congruences_and_minimize, one dimension with factors.

void
test4() {
  nout << "test4:" << endl;

  Variable A(0);

  Congruence_System cgs;
  cgs.insert((A %= 7) / 3);

  Grid gr(1);
  gr.add_congruences_and_minimize(cgs);

  if (find_variation(gr))
    exit(1);

  Generator_System known_gs;
  known_gs.insert(point(1*A));
  known_gs.insert(point(4*A));

  Grid known_gr(known_gs);

  if (find_variation(known_gr))
    exit(1);

  if (gr == known_gr)
    return;

  nout << "Grid should equal known grid." << endl
       << "grid:" << endl << gr << endl
       << "known grid:" << endl << known_gr << endl;
  exit(1);
}

// add_congruences_and_minimize, two dimensions.

void
test5() {
  nout << "test5:" << endl;

  Variable A(0);
  Variable B(1);

  Congruence_System cgs;
  cgs.insert((A - B %= 0) / 3);

  Grid gr(2);
  gr.add_congruences_and_minimize(cgs);

  if (find_variation(gr))
    exit(1);

  Generator_System known_gs;
  known_gs.insert(point(0*A + 0*B));
  known_gs.insert(point(A + B));
  known_gs.insert(point(3*B));

  Grid known_gr(known_gs);

  if (find_variation(known_gr))
    exit(1);

  if (gr == known_gr)
    return;

  nout << "Grid should equal known grid." << endl
       << "grid:" << endl << gr << endl
       << "known grid:" << endl << known_gr << endl;
  exit(1);
}

// cong_test0 from Chiara conversion_test2.cc.

void
test6() {
  nout << "test6:" << endl;

  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert(0*A %= -1);
  cgs.insert(  A %= 0);
  cgs.insert(  A + B %= 0);
  cgs.insert(  A + B + C %= 0);

  Grid gr(3);
  gr.add_congruences_and_minimize(cgs);

  if (find_variation(gr))
    exit(1);

  Generator_System known_gs;
  known_gs.insert(point());
  known_gs.insert(point(A - B));
  known_gs.insert(point(B - C));
  known_gs.insert(point(C));

  Grid known_gr(known_gs);

  if (find_variation(known_gr))
    exit(1);

  if (gr == known_gr)
    return;

  nout << "Grid should equal known grid." << endl
       << "grid:" << endl << gr << endl
       << "known grid:" << endl << known_gr << endl;
  exit(1);
}

// cong_test1 from Chiara conversion_test2.cc.

void
test7() {
  nout << "test7:" << endl;

  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert( 0*A %= -1);
  cgs.insert(  -A %= 64);
  cgs.insert(-6*A +   B + 0*C %= -8);
  cgs.insert( 3*A + 2*B +   C %= -4);

  Grid gr(3);
  gr.add_congruences_and_minimize(cgs);

  if (find_variation(gr))
    exit(1);

  Generator_System known_gs;
  known_gs.insert(point(64*A + 392*B - 972*C, -1));
  known_gs.insert(point(65*A + 398*B - 987*C, -1));
  known_gs.insert(point(64*A + 391*B - 970*C, -1));
  known_gs.insert(point(64*A + 392*B - 973*C, -1));

  Grid known_gr(known_gs);

  if (find_variation(known_gr))
    exit(1);

  if (gr == known_gr)
    return;

  nout << "Grid should equal known grid." << endl
       << "grid:" << endl << gr << endl
       << "known grid:" << endl << known_gr << endl;
  exit(1);
}

/* Adding a false equality (cong_test2 from Chiara
   conversion_test2.cc).  */

void
test8() {
  nout << "test8:" << endl;

  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((0*A %= -1) / 0);
  cgs.insert((  A %= -1) / 2);

  Grid gr(3);
  gr.add_congruences_and_minimize(cgs);

  if (find_variation(gr))
    exit(1);

  Grid known_gr(3);

  if (find_variation(known_gr))
    exit(1);

  if (gr == known_gr)
    return;

  nout << "Grid should equal known grid." << endl
       << "grid:" << endl << gr << endl
       << "known grid:" << endl << known_gr << endl;
  exit(1);
}

// cong_test3 from Chiara conversion_test2.cc.

void
test9() {
  nout << "test9:" << endl;

  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((0*A %= -2) / 2);
  cgs.insert((  A %=  0) / 2);
  cgs.insert((        B %= 0) / 2);
  cgs.insert((  A +   B +   C %= 0) / 2);

  Grid gr(3);
  gr.add_congruences_and_minimize(cgs);

  if (find_variation(gr))
    exit(1);

  Generator_System known_gs;
  known_gs.insert(point(0*A + 0*B + 0*C));
  known_gs.insert(point(2*A + 0*B - 2*C));
  known_gs.insert(point(      2*B - 2*C));
  known_gs.insert(point(            2*C));

  Grid known_gr(known_gs);

  if (find_variation(known_gr))
    exit(1);

  if (gr == known_gr)
    return;

  nout << "Grid should equal known grid." << endl
       << "grid:" << endl << gr << endl
       << "known grid:" << endl << known_gr << endl;
  exit(1);
}

// cong_test4 from Chiara conversion_test2.cc.

void
test10() {
  nout << "test10:" << endl;

  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((3*A             %= -2) / 3);
  cgs.insert((5*A + 9*B +   C %= -1) / 3);
  cgs.insert((        B + 3*C %= -2) / 3);
  cgs.insert((      2*B + 3*C %= -2) / 3);

  Grid gr(3);
  gr.add_congruences_and_minimize(cgs);

  if (find_variation(gr))
    exit(1);

  Generator_System known_gs;
  known_gs.insert(point(-2*A +  0*B +   7*C, 3));
  known_gs.insert(point( 7*A +  0*B -  38*C, 3));
  known_gs.insert(point(-2*A + 27*B - 236*C, 3));
  known_gs.insert(point(-2*A +  0*B +  34*C, 3));

  Grid known_gr(known_gs);

  if (find_variation(known_gr))
    exit(1);

  if (gr == known_gr)
    return;

  nout << "Grid should equal known grid." << endl
       << "grid:" << endl << gr << endl
       << "known grid:" << endl << known_gr << endl;
  exit(1);
}

// cong_test5 from Chiara conversion_test2.cc.

void
test11() {
  nout << "test11:" << endl;

  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((A + 2*B +   C %= -2) / 5);
  cgs.insert((    3*B       %=  0) / 5);
  cgs.insert((      B       %=  0) / 5);
  cgs.insert((          3*C %= -4) / 5);
  cgs.insert((    3*B +   C %= -3) / 5);

  Grid gr(3);
  gr.add_congruences_and_minimize(cgs);

  if (find_variation(gr))
    exit(1);

  Generator_System known_gs;
  known_gs.insert(point(  A       -  3*C));
  known_gs.insert(point(6*A + 5*B - 18*C));
  known_gs.insert(point(  A + 5*B - 18*C));
  known_gs.insert(point(  A       +  2*C));

  Grid known_gr(known_gs);

  if (find_variation(known_gr))
    exit(1);

  if (gr == known_gr)
    return;

  nout << "Grid should equal known grid." << endl
       << "grid:" << endl << gr << endl
       << "known grid:" << endl << known_gr << endl;

  exit(1);
}

// cong_test6 from Chiara conversion_test2.cc.

void
test12() {
  nout << "test12:" << endl;

  Variable A(0);
  Variable B(1);
  Variable C(2);

  Congruence_System cgs;
  cgs.insert((3*A           %= -2) / 5);
  cgs.insert((      B + 2*C %=  0) / 5);
  cgs.insert((    2*B + 3*C %= -3) / 5);

  Grid gr(3);
  gr.add_congruences_and_minimize(cgs);

  if (find_variation(gr))
    exit(1);

  Generator_System known_gs;
  known_gs.insert(point(-2*A - 18*B +  9*C, 3));
  known_gs.insert(point( 3*A - 18*B +  9*C, 3));
  known_gs.insert(point(-2*A -  3*B -  6*C, 3));
  known_gs.insert(point(-2*A - 18*B + 24*C, 3));

  Grid known_gr(known_gs);

  if (find_variation(known_gr))
    exit(1);

  if (gr == known_gr)
    return;

  nout << "Grid should equal known grid." << endl
       << "grid:" << endl << gr << endl
       << "known grid:" << endl << known_gr << endl;

  exit(1);
}
#endif
int
main() TRY {
  set_handlers();

  nout << "grid3:" << endl;

  test1();
  test2();
  test3();
  test4();
#if 0
  test5();
  test6();
  test7();
  test8();
  test9();
  test10();
  test11();
  test12();
  test13();
  test14();
#endif

  return 0;
}
CATCH
