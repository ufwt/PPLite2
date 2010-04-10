/* Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>

This file is free software; as a special exception the author gives
unlimited permission to copy and/or distribute it, with or without
modifications, as long as this notice is preserved.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE. */

#ifndef PPL_Unlimited_Sparse_Row_types_hh
#define PPL_Unlimited_Sparse_Row_types_hh 1

#include "Unlimited_Sparse_Row_Over_Linear_Sequence.types.hh"
#include "Unlimited_Sparse_Row_Over_CO_Tree.types.hh"

namespace Parma_Polyhedra_Library {

#ifdef USE_PPL_SPARSE_BACKEND_CO_TREE

typedef Unlimited_Sparse_Row_Over_CO_Tree Unlimited_Sparse_Row;

#else

typedef Unlimited_Sparse_Row_Over_Linear_Sequence Unlimited_Sparse_Row;

#endif // defined(USE_PPL_SPARSE_BACKEND_CO_TREE)

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Unlimited_Sparse_Row_types_hh)
