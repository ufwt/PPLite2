/* Octagonal_Shape class implementation: strong-closure function.
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

template <typename T>
void
Octagonal_Shape<T>::strong_closure_assign() const {
  // This closure algorithm is the classical Floyd-Warshall algorithm with
  // the optimization of the inner loop.
.
  using Implementation::BD_Shapes::min_assign;

  // Do something only if necessary (zero-dim implies strong closure).
  if (marked_empty() || marked_strongly_closed() || space_dim == 0)
    return;

  // Even though the octagon will not change, its internal representation
  // is going to be modified by the closure algorithm.
  Octagonal_Shape& x = const_cast<Octagonal_Shape<T>&>(*this);

  // Use these type aliases for short.
  typedef typename OR_Matrix<N>::row_iterator Row_Iterator;
  typedef typename OR_Matrix<N>::row_reference_type Row_Reference;
  // Avoid recomputations.
  const dimension_type n_rows = x.matrix.num_rows();
  const Row_Iterator m_begin = x.matrix.row_begin();
  const Row_Iterator m_end = x.matrix.row_end();

  // Fill the main diagonal with zeros.
  for (Row_Iterator i = m_begin; i != m_end; ++i) {
    assert(is_plus_infinity((*i)[i.index()]));
    assign_r((*i)[i.index()], 0, ROUND_NOT_NEEDED);
  }

  // This algorithm is given by two steps: the first one is a simple
  // adaptation of the `shortest-path closure' using the Floyd-Warshall
  // algorithm; the second one is the `strong-coherence' algorithm.
  // It is important to note that after the strong-coherence,
  // the octagon is still shortest-path closed and hence, strongly closed.

  // Recall that, given an index `h', we indicate with `ch' the coherent
  // index, i.e., the index such that:
  //   ch = h + 1, if h is an even number;
  //   ch = h - 1, if h is an odd number.

  // Allocate it here, once and for all.
  N sum;

  // Step 1: closure.
  for (Row_Iterator k_iter = m_begin; k_iter != m_end; ++k_iter) {
    const dimension_type k = k_iter.index();
    const dimension_type ck = coherent_index(k);
    const dimension_type rs_k = k_iter.row_size();
    Row_Reference x_k = *k_iter;
    Row_Reference x_ck = (k%2) ? *(k_iter-1) : *(k_iter+1);

    for (Row_Iterator i_iter = m_begin; i_iter != m_end; ++i_iter) {
      const dimension_type i = i_iter.index();
      const dimension_type ci = coherent_index(i);
      const dimension_type rs_i = i_iter.row_size();
      Row_Reference x_i = *i_iter;
      const N& x_i_k = (k < rs_i) ? x_i[k] : x_ck[ci];

      Row_Reference x_ci = *((i%2) ? i_iter-1 : i_iter+1);

      // OPTIMIZED VERSION OF THE INNER LOOP STARTS HERE.
      // The inner loop is divided in four loops, based on the relation
      // between the three indices `k', `i', and `j'.
      const dimension_type min_rs = std::min(rs_i, rs_k);
      const dimension_type max_rs = std::max(rs_i, rs_k);
      // The first part of the optimized inner loop.
      for (dimension_type j = 0; j < min_rs; ++j) {
	const N& x_k_j = x_k[j];
	add_assign_r(sum, x_i_k, x_k_j, ROUND_UP);
	min_assign(x_i[j], sum);
      }
      // The second part of the optimized inner loop.
      // NOTE: the following two are mutually exclusive loops.
      if (rs_i == min_rs) {
	// Note: both `rs_i' and `rs_k' are even numbers;
	// unrolling two iterations so as to optimize away
	// the computation of coherent indexes.
	for (dimension_type j = rs_i; j < rs_k; j += 2) {
	  // The loop body for index `j'.
	  const N& x_k_j = x_k[j];
	  add_assign_r(sum, x_i_k, x_k_j, ROUND_UP);
	  assert(coherent_index(j) == j+1);
	  min_assign(x.matrix[j+1][ci], sum);
	  // The loop body for index `j+1'.
	  const N& x_k_j1 = x_k[j+1];
	  add_assign_r(sum, x_i_k, x_k_j1, ROUND_UP);
	  assert(coherent_index(j+1) == j);
	  min_assign(x.matrix[j][ci], sum);
	}
      }
      else {
	// Note: both `rs_i' and `rs_k' are even numbers;
	// unrolling two iterations so as to optimize away
	// the computation of coherent indexes.
	for (dimension_type j = rs_k; j < rs_i; j += 2) {
	  // The loop body for index `j'.
	  assert(coherent_index(j) == j+1);
	  const N& x_k_j = x.matrix[j+1][ck];
	  add_assign_r(sum, x_i_k, x_k_j, ROUND_UP);
	  min_assign(x_i[j], sum);
	  // The loop body for index `j+1'.
	  assert(coherent_index(j+1) == j);
	  const N& x_k_j1 = x.matrix[j][ck];
	  add_assign_r(sum, x_i_k, x_k_j1, ROUND_UP);
	  min_assign(x_i[j+1], sum);
	}
      }
      // The third part of the optimized inner loop.
      // Note: both `max_rs' and `n_rows' are even numbers;
      // unrolling two iterations so as to optimize away
      // the computation of coherent indexes.
      for (dimension_type j = max_rs; j < n_rows; j += 2) {
	// The loop body for index `j'.
	assert(coherent_index(j) == j+1);
	Row_Iterator x_cj_iter = m_begin + (j+1);
	Row_Reference x_cj = *x_cj_iter;
	const N& x_k_j = x_cj[ck];
	add_assign_r(sum, x_i_k, x_k_j, ROUND_UP);
	min_assign(x_cj[ci], sum);
	// The loop body for index `j+1'.
	assert(coherent_index(j+1) == j);
	Row_Reference x_cj1 = *(--x_cj_iter);
	const N& x_k_j1 = x_cj1[ck];
	add_assign_r(sum, x_i_k, x_k_j1, ROUND_UP);
	min_assign(x_cj1[ci], sum);
      }
      // OPTIMIZED VERSION OF INNER LOOP ENDS HERE.
    }

  }

  // Check for emptyness: the octagon is empty if and only if there is a
  // negative value in the main diagonal.
  for (Row_Iterator i = m_begin; i != m_end; ++i) {
    N& x_i_i = (*i)[i.index()];
    if (x_i_i < 0) {
      x.status.set_empty();
      return;
    }
    else {
      assert(x_i_i == 0);
      // Restore PLUS_INFINITY on the main diagonal.
      x_i_i = PLUS_INFINITY;
    }
  }

  // Step 2: we enforce the strong coherence.
  x.strong_coherence_assign();
  // The octagon is not empty and it is now strongly closed.
  x.status.set_strongly_closed();
}
