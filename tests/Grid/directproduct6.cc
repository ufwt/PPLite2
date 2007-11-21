/* Test Grid::congruences().
   Copyright (C) 2001-2007 Roberto Bagnara <bagnara@cs.unipr.it>

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
site: http://www.cs.unipr.it/ppl/ .

Tests bounds_from_above(), bounds_from_below(), maximize() and minimize()
for the product domains */

#include "ppl_test.hh"

#define PH_IS_FIRST

// ONE AND ONLY ONE OF THESE MUST BE 1
#define NNC_Poly_Class 0
#define C_Poly_Class 1
#define BD_Shape_Class 0
#define Octagonal_Shape_Class 0

#if Octagonal_Shape_Class
typedef TOctagonal_Shape Poly;
#endif

#if BD_Shape_Class
typedef BD_Shape<mpq_class> Poly;
#endif

#if NNC_Poly_Class
typedef NNC_Polyhedron Poly;
#endif

#if C_Poly_Class
typedef C_Polyhedron Poly;
#endif

#ifdef PH_IS_FIRST
typedef Domain_Product<Poly, Grid>::Direct_Product Product;
#else
typedef Domain_Product<Grid, Poly>::Direct_Product Product;
#endif

namespace {

// Empty.
bool
test01() {
  Product dp(7, EMPTY);

  bool ok = (dp.bounds_from_above(Linear_Expression(0))
	     && dp.bounds_from_below(Linear_Expression(0)));

  print_congruences(dp, "*** dp congruences ***");
  print_constraints(dp, "*** dp constraints ***");

  return ok;
}

// Zero dimension empty.
bool
test02() {
  Product dp(0, EMPTY);

  bool ok = (dp.bounds_from_above(Linear_Expression(3))
	     && dp.bounds_from_below(Linear_Expression(3)));

  print_congruences(dp, "*** dp congruences ***");
  print_constraints(dp, "*** dp constraints ***");

  return ok;
}

// Zero dimension universe.
bool
test03() {
  Product dp(0);

  bool ok = (dp.bounds_from_above(Linear_Expression(1))
	     && dp.bounds_from_below(Linear_Expression(1)));

  print_congruences(dp, "*** dp congruences ***");
  print_constraints(dp, "*** dp constraints ***");

  return ok;
}

// Point.
bool
test04() {
  Variable A(0);
  Variable B(1);

  Product dp(2);
  dp.add_constraint(A == 1);
  dp.add_constraint(3*B == 2);

  Linear_Expression le = A + B;
  bool ok = dp.bounds_from_above(le)
    && dp.bounds_from_below(le);

  print_congruences(dp, "*** dp congruences ***");
  print_constraints(dp, "*** dp constraints ***");

  return ok;
}

// only one component is bounded.
bool
test05() {
  Variable A(0);
  Variable B(1);

  Product dp(2);
  dp.add_constraint(A - B >= 1);
  dp.add_constraint(A - B <= 1);
  dp.add_congruence(3*B %= 2);

  Linear_Expression le = A - B;
  bool ok = dp.bounds_from_above(le)
    && dp.bounds_from_below(le);

  print_congruences(dp, "*** dp congruences ***");
  print_constraints(dp, "*** dp constraints ***");

  return ok;
}

// Line and neither component is bounded.
bool
test06() {
  Variable A(0);
  Variable B(1);

  Product dp(2);
  dp.add_constraint(B == 1);

  Linear_Expression le = 2*A - B;

  bool ok = !dp.bounds_from_above(le)
    && !dp.bounds_from_below(le);

  print_congruences(dp, "*** dp congruences ***");
  print_constraints(dp, "*** dp constraints ***");

  return ok;
}

// Empty. maximize() and minimize()
bool
test07() {
  Product dp(7, EMPTY);

  Coefficient extr_n;
  Coefficient extr_d;
  bool dummy;

  bool ok = (!dp.maximize(Linear_Expression(0), extr_n, extr_d, dummy));

  print_congruences(dp, "*** dp congruences ***");
  print_constraints(dp, "*** dp constraints ***");

  return ok;
}

// Zero dimension empty.
bool
test08() {
  Product dp(0, EMPTY);

  Coefficient extr_n;
  Coefficient extr_d;
  bool dummy;
  Generator pnt(point());

  bool ok = !dp.maximize(Linear_Expression(0), extr_n, extr_d, dummy)
    && !dp.maximize(Linear_Expression(0), extr_n, extr_d, dummy, pnt);

  print_congruences(dp, "*** dp congruences ***");
  print_constraints(dp, "*** dp constraints ***");

  return ok;
}

// Zero dimension universe.
bool
test09() {
  Product dp(0);

  Coefficient extr_n, extr_d;
  bool dummy;
  Generator pnt(point());

  bool ok = dp.maximize(Linear_Expression(0), extr_n, extr_d, dummy)
    && dp.maximize(Linear_Expression(0), extr_n, extr_d, dummy, pnt);

  print_congruences(dp, "*** dp congruences ***");
  print_constraints(dp, "*** dp constraints ***");

  return ok;
}

// Point.
bool
test10() {
  Variable A(0);
  Variable B(1);

  Product dp(2);
  dp.add_constraint(A == 1);
  dp.add_constraint(3*B == 2);

  Linear_Expression le = A + B;

  Coefficient max_n;
  Coefficient max_d;
  Coefficient min_n;
  Coefficient min_d;
  Generator pnt_max(point());
  Generator pnt_min(point());
  Generator known_pnt(point(3*A + 2*B, 3));
  bool max;
  bool min;

  bool ok = dp.maximize(le, max_n, max_d, max)
    && dp.minimize(le, min_n, min_d, min)
    && dp.maximize(le, max_n, max_d, max, pnt_max)
    && dp.minimize(le, min_n, min_d, min, pnt_min);

  ok = ok
    && max && min && max_n == 5 && max_d == 3 && min_n == 5 && min_d == 3
    && pnt_max == known_pnt && pnt_min == known_pnt;

  print_generator(pnt_max, "*** max point ***");
  print_generator(pnt_min, "*** min point ***");

  print_congruences(dp, "*** dp congruences ***");
  print_constraints(dp, "*** dp constraints ***");

  return ok;
}

// only one component is bounded.
bool
test11() {
  Variable A(0);
  Variable B(1);

  Product dp(2);
  dp.add_constraint(A - B >= 1);
  dp.add_constraint(A - B <= 1);
  dp.add_congruence(3*B %= 2);
;
  Linear_Expression le = A - B;

  Coefficient max_n;
  Coefficient max_d;
  Coefficient min_n;
  Coefficient min_d;
  Generator pnt_max(point());
  Generator pnt_min(point());
  bool max;
  bool min;

  bool ok = dp.maximize(le, max_n, max_d, max)
    && dp.minimize(le, min_n, min_d, min)
    && dp.maximize(le, max_n, max_d, max, pnt_max)
    && dp.minimize(le, min_n, min_d, min, pnt_min);

  ok = ok
    && max && min && max_n == 1 && max_d == 1 && min_n == 1 && min_d == 1
    && pnt_max == pnt_min;

  print_generator(pnt_max, "*** maximum point ***");

  print_congruences(dp, "*** dp congruences ***");
  print_constraints(dp, "*** dp constraints ***");

  return ok;
}

// Line and neither component is bounded.
bool
test12() {
  Variable A(0);
  Variable B(1);

  Product dp(2);
  dp.add_constraint(B == 1);

  Linear_Expression le = 2*A - B;

  Coefficient max_n;
  Coefficient max_d;
  Coefficient min_n;
  Coefficient min_d;
  Generator pnt_max(point());
  Generator pnt_min(point());
  bool max;
  bool min;

  bool ok = !dp.maximize(le, max_n, max_d, max)
    && !dp.minimize(le, min_n, min_d, min)
    && !dp.maximize(le, max_n, max_d, max, pnt_max)
    && !dp.minimize(le, min_n, min_d, min, pnt_min);

  print_congruences(dp, "*** dp congruences ***");
  print_constraints(dp, "*** dp constraints ***");

  return ok;
}

// only one component is strictly bounded.
bool
test13() {
  Variable A(0);
  Variable B(1);

  Product dp(2);
#if NNC_Poly_Class
  dp.add_constraint(A - B > 0);
  dp.add_constraint(A - B < 1);
#else
  dp.add_constraint(A - B >= 0);
  dp.add_constraint(A - B <= 1);
#endif
  dp.add_congruence(3*B %= 2);

  Linear_Expression le = A - B;

  Coefficient max_n;
  Coefficient max_d;
  Coefficient min_n;
  Coefficient min_d;
  Generator pnt_max(point());
  Generator pnt_min(point());

  bool max;
  bool min;

  bool ok = dp.maximize(le, max_n, max_d, max)
    && dp.minimize(le, min_n, min_d, min)
    && dp.maximize(le, max_n, max_d, max, pnt_max)
    && dp.minimize(le, min_n, min_d, min, pnt_min);

#if NNC_Poly_Class
  ok = ok && !max && !min;
#else
  ok = ok && max && min;
#endif

  ok = ok && max_n == 1 && max_d == 1 && min_n == 0 && min_d == 1;

  print_generator(pnt_max, "*** maximum point ***");

  print_congruences(dp, "*** dp congruences ***");
  print_constraints(dp, "*** dp constraints ***");

  return ok;
}

// Non-empty product. bounded_affine_image/3
bool
test14() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Product dp(3);
  dp.add_congruence((A ==  0) / 0);
  dp.add_congruence((B ==  0) / 0);
  dp.add_congruence((C == -2) / 0);

  print_congruences(dp, "*** dp congruences ***");
  print_constraints(dp, "*** dp constraints ***");

  dp.bounded_affine_image(A, 7-B, B+3);

  Product known_dp(3);
  known_dp.add_constraint(C == -2);
  known_dp.add_constraint(B == 0);
  known_dp.add_constraint(A <= 3);
  known_dp.add_constraint(A - B >= 7);

  bool ok = (dp == known_dp);

  print_congruences(dp,
     "*** dp.bounded_affine_image(A, 7-B, B+3) congruences ***");
  print_constraints(dp,
     "*** dp.bounded_affine_image(A, 7-B, B+3) constraints ***");

  return ok;
}

// Empty product component. bounded_affine_image/3
bool
test15() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Product dp(3);
  dp.add_constraint(A ==  0);
  dp.add_constraint(A ==  1);
  dp.add_constraint(C == -2);

  print_congruences(dp, "*** dp congruences ***");
  print_constraints(dp, "*** dp constraints ***");

  dp.bounded_affine_image(A, 7-B, B+3);

  Product known_dp(3, EMPTY);

  bool ok = (dp == known_dp);

  print_congruences(dp, "*** dp congruences ***");
  print_constraints(dp, "*** dp constraints ***");

  return ok;
}

// Non-empty product. bounded_affine_preimage/3
bool
test16() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Product dp(3);
  dp.add_congruence((A ==  0) / 0);
  dp.add_congruence((B ==  0) / 0);
  dp.add_congruence((C == -2) / 0);

  print_congruences(dp, "*** dp congruences ***");
  print_constraints(dp, "*** dp constraints ***");

  dp.bounded_affine_preimage(A, 7-B, B+3);

  Constraint_System cs;
  cs.insert(C == -2);
  cs.insert(B == 0);
  cs.insert(C >= 3);

  Product known_dp(3);
  known_dp.add_constraints(cs);

  bool ok = (dp == known_dp);

  print_congruences(dp, "*** dp congruences ***");
  print_constraints(dp, "*** dp constraints ***");

  return ok;
}

// Empty product. bounded_affine_preimage/3
bool
test17() {
  Variable A(0);
  Variable B(1);
  Variable C(2);

  Product dp(3);
  dp.add_constraint(A ==  0);
  dp.add_constraint(A ==  1);
  dp.add_constraint(C == -2);

  print_congruences(dp, "*** dp congruences ***");
  print_constraints(dp, "*** dp constraints ***");

  dp.bounded_affine_preimage(A, 7-B, B+3);

  Product known_dp(3, EMPTY);

  bool ok = (dp == known_dp);

  print_congruences(dp, "*** dp congruences ***");
  print_constraints(dp, "*** dp constraints ***");

  return ok;
}

} // namespace

BEGIN_MAIN
  DO_TEST(test01);
  DO_TEST(test02);
  DO_TEST(test03);
  DO_TEST(test04);
  DO_TEST(test05);
  DO_TEST(test06);
  DO_TEST(test07);
  DO_TEST(test08);
  DO_TEST(test09);
  DO_TEST(test10);
  DO_TEST(test11);
  DO_TEST(test12);
  DO_TEST(test13);
  DO_TEST(test14);
  DO_TEST(test15);
  DO_TEST(test16);
  DO_TEST(test17);
END_MAIN
