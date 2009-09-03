/* PIP_Problem class implementation: inline functions.
   Copyright (C) 2001-2009 Roberto Bagnara <bagnara@cs.unipr.it>

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

#ifndef PPL_PIP_Problem_inlines_hh
#define PPL_PIP_Problem_inlines_hh 1

namespace Parma_Polyhedra_Library {

inline
PIP_Problem::Rational_Matrix::Rational_Matrix()
  : Matrix(),
    denominator(1) {
}

inline
PIP_Problem::Rational_Matrix::Rational_Matrix(dimension_type n_rows,
                                              dimension_type n_columns,
                                              Row::Flags row_flags)
  : Matrix(n_rows, n_columns, row_flags),
    denominator(1) {
}

inline
PIP_Problem::Rational_Matrix::Rational_Matrix(const Rational_Matrix& y)
  : Matrix(y),
    denominator(1) {
}

inline bool
PIP_Problem::Rational_Matrix::is_integer() {
  return denominator == 1;
}

inline const Coefficient&
PIP_Problem::Rational_Matrix::get_denominator() const {
  return denominator;
}

inline
PIP_Problem::~PIP_Problem() {
}

inline dimension_type
PIP_Problem::max_space_dimension() {
  return Constraint::max_space_dimension();
}

} // namespace Parma_Polyhedra_Library

namespace std {

#if 0
/*! \relates Parma_Polyhedra_Library::PIP_Problem */
inline void
swap(Parma_Polyhedra_Library::PIP_Problem& x,
     Parma_Polyhedra_Library::PIP_Problem& y) {
  x.swap(y);
}
#endif

} // namespace std

#endif // !defined(PPL_PIP_Problem_inlines_hh)
