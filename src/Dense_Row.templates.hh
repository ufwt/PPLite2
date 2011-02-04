/* Dense_Row class implementation: non-inline template functions.
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2011 BUGSENG srl (http://bugseng.com)

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

#ifndef PPL_Dense_Row_templates_hh
#define PPL_Dense_Row_templates_hh 1

namespace Parma_Polyhedra_Library {


template <typename Func1, typename Func2>
void
Dense_Row::combine_needs_first(const Dense_Row& y, const Func1& /* f */,
                               const Func2& g) {
  for (dimension_type i = size(); i-- > 0; )
    g((*this)[i], y[i]);
}

template <typename Func1, typename Func2>
void
Dense_Row::combine_needs_second(const Dense_Row& y, const Func1& g,
                                const Func2& /* h */) {
  for (dimension_type i = size(); i-- > 0; )
    g((*this)[i], y[i]);
}

template <typename Func1, typename Func2, typename Func3>
void
Dense_Row::combine(const Dense_Row& y, const Func1& /* f */, const Func2& g,
                   const Func3& /* h */) {
  for (dimension_type i = size(); i-- > 0; )
    g((*this)[i], y[i]);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Dense_Row_templates_hh)