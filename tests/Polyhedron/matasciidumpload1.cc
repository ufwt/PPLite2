/* Test Matrix::ascii_dump() and Matrix::ascii_load().
   Copyright (C) 2001-2005 Roberto Bagnara <bagnara@cs.unipr.it>

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
#include "files.hh"
#include <fstream>
#include <ctime>
#include <gmpxx.h>

using namespace std;
using namespace Parma_Polyhedra_Library;

#ifndef NOISY
#define NOISY 0
#endif

namespace {

const char* data_file = "matasciidumpload1.dat";

} // namespace

int
main() TRY {
  set_handlers();
  Variable A(0);
  Variable B(1);

  gmp_randclass r(gmp_randinit_default);
  // Seed the random number generator.
  r.seed((unsigned long) time(0));

#define ROWS 3
#define COLS 4

  mpz_class cmin = 0;
  mpz_class range = 0;
  if (std::numeric_limits<COEFFICIENT_TYPE>::is_bounded) {
    cmin = raw_value(std::numeric_limits<COEFFICIENT_TYPE>::min());
    range = raw_value(std::numeric_limits<COEFFICIENT_TYPE>::max()) - cmin + 1;
  }

  Matrix m1(ROWS, COLS);
  mpz_class n;
  for (dimension_type row = 0; row < ROWS; ++row)
    for (dimension_type col = 0; col < COLS; ++col) {
      if (std::numeric_limits<COEFFICIENT_TYPE>::is_bounded)
	n = cmin + r.get_z_range(range);
      else
	n = r.get_z_bits(512);
      m1[row][col] = n;
    }

  fstream f;
  open(f, data_file, ios_base::out);
  m1.ascii_dump(f);
  close(f);

  open(f, data_file, ios_base::in);
  Matrix m2;
  m2.ascii_load(f);
  close(f);

  int retval = (m1 == m2) ? 0 : 1;

  return retval;
}
CATCH
