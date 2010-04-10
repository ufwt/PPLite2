/* CO_Tree class implementation
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>

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

#include <ppl-config.h>
#include "CO_Tree.defs.hh"
#include <iostream>

namespace PPL = Parma_Polyhedra_Library;

PPL::CO_Tree::CO_Tree(dimension_type reserved_size1) {

  dimension_type l = 0;

  if (reserved_size1 == 0)
    reserved_size1 = 1;

  // Calculate the base-2 log of reserved_size
  while (reserved_size1 != 1) {
    reserved_size1 /= 2;
    l++;
  }
  // Add 1 to the log.
  l++;

  reserved_size = ((dimension_type)1 << l) - 1;
  data = new value_type[reserved_size + 1];
  // Mark all pairs as unused.
  for (dimension_type i = 1; i <= reserved_size; ++i)
    data[i].first = unused_index;

  max_depth = l;
  rebuild_level_data(l);

  size = 0;

  PPL_ASSERT(OK());
}

PPL::CO_Tree::~CO_Tree() {

  delete [] level;
  delete [] data;
}

PPL::dimension_type
PPL::CO_Tree::external_memory_in_bytes() const {
  dimension_type size = 0;
  // Adding the size of data[]
  size += (reserved_size + 1)*sizeof(data[0]);
  // Adding the size of level[]
  size += max_depth*sizeof(level[0]);
  return size;
}

void
PPL::CO_Tree::move_data_from(CO_Tree& tree) {
  PPL_ASSERT(size == 0);
  if (tree.size == 0)
    return;


  inorder_iterator root(&*this);
  inorder_iterator itr = tree.before_begin();
  itr.get_next_value();

  PPL_ASSERT(itr->first != unused_index);

  // This is static and with static allocation, to improve performance.
  static std::pair<dimension_type,char> stack[5*8*sizeof(dimension_type)];
  dimension_type stack_first_empty = 0;

  // A pair (n, operation) in the stack means:
  //
  // * Go to the parent, if operation is 0.
  // * Go to the left child, then visit the current tree (with size n), if
  //   operation is 1.
  // * Go to the right child, then visit the current tree (with size n), if
  //   operation is 2.
  // * Visit the current tree (with size n), if operation is 4.

  stack[0].first = tree.size;
  stack[0].second = 4;
  ++stack_first_empty;

  while (stack_first_empty != 0) {

    // top_n         = stack.top().first;
    // top_operation = stack.top().second;
    const dimension_type top_n = stack[stack_first_empty - 1].first;
    const char top_operation = stack[stack_first_empty - 1].second;

    switch (top_operation) {

    case 0:
      root.get_parent();
      --stack_first_empty;
      continue;

    case 1:
      root.get_left_child();
      break;

    case 2:
      root.get_right_child();
      break;
#ifndef NDEBUG
    case 4:
      break;

    default:
      // We should not be here
      PPL_ASSERT(false);
#endif
    }

    // We now visit the current tree

    if (top_n == 0) {
      --stack_first_empty;
    } else {
      if (top_n == 1) {
        PPL_ASSERT(root != itr);
        PPL_ASSERT(root->first == unused_index);
        root->first = itr->first;
        move_data_element(root->second, itr->second);
        itr.get_next_value();
        --stack_first_empty;
      } else {
        PPL_ASSERT(stack_first_empty + 3 < sizeof(stack)/sizeof(stack[0]));

        const dimension_type half = (top_n + 1) / 2;
        stack[stack_first_empty - 1].second = 0;
        stack[stack_first_empty    ] = std::make_pair(top_n - half, 2);
        stack[stack_first_empty + 1] = std::make_pair(1, 4);
        stack[stack_first_empty + 2].second = 0;
        stack[stack_first_empty + 3] = std::make_pair(half - 1, 1);
        stack_first_empty += 4;
      }
    }
  }
  size = tree.size;
  PPL_ASSERT(structure_OK());
  PPL_ASSERT(tree.OK());
}

bool
PPL::CO_Tree::OK() const {

  if (!structure_OK())
    return false;

  if (size > 0) {
    const float density
      = size / (float) (((dimension_type)1 << max_depth) - 1);
    if (density > max_density)
      // Found too high density.
      return false;
    if (density < min_density) {
      // This is the density we could achieve by calling rebuild_bigger_tree.
      const float candidate_density
        = size / (float) (((dimension_type)1 << (max_depth-1)) - 1);
      if (candidate_density <= max_density)
        // Found too low density
        return false;
    }
  }

  return true;
}

bool
PPL::CO_Tree::structure_OK() const {
  {
    inorder_const_iterator itr = before_begin();
    inorder_const_iterator itr_end = end();
    dimension_type real_size = 0;

    itr.get_next_value();

    for ( ; itr != itr_end; ++itr)
      if (itr->first != unused_index)
        ++real_size;

    if (real_size != size)
      // There are \p real_size elements in the tree, but size is \p size.
      return false;
  }

  if (size == 0) {

    inorder_const_iterator itr(&*this);
    if (itr->first != unused_index)
      return false;

  } else {
    inorder_const_iterator itr(&*this);
    dimension_type real_size = count_used_in_subtree(itr);
    if (real_size != size)
      // There are \p real_size elements in the tree that are reachable by the
      // root, but size is \p size.
      return false;
  }

  if (size != 0) {
    inorder_const_iterator itr = before_begin();
    inorder_const_iterator itr_end = end();

    itr.get_next_value();

    if (itr != itr_end) {
      dimension_type last_index = itr->first;
      for (itr.get_next_value(); itr != itr_end; itr.get_next_value()) {
        if (last_index >= itr->first)
          // Found index \p itr->first after index \p last_index.
          return false;
        last_index = itr->first;
      }
    }
  }

  return true;
}

void
PPL::CO_Tree::rebuild_level_data_helper(dimension_type min_depth,
                                        dimension_type max_depth) {
  if (min_depth == max_depth)
    return;

  dimension_type d0 = (min_depth + max_depth) / 2;

  level[d0].depth_of_root_of_top_tree = min_depth;
  level[d0].bottom_tree_size = ((dimension_type)1 << (max_depth - d0)) - 1;
  level[d0].top_tree_size = ((dimension_type)1 << (d0 - min_depth + 1)) - 1;

  rebuild_level_data_helper(min_depth, d0);
  rebuild_level_data_helper(d0 + 1, max_depth);
}

void
PPL::CO_Tree::insert(dimension_type key1, const data_type& data1) {
  PPL_ASSERT(key1 != unused_index);

  if (size == 0) {
    if (reserved_size == 1)
      rebuild_bigger_tree();
    inorder_iterator itr(&*this);
    PPL_ASSERT(itr->first == unused_index);
    itr->first = key1;
    itr->second = data1;
    size++;

    PPL_ASSERT(OK());
    return;
  }

  inorder_iterator itr(&*this);
  lower_bound(itr, key1);

  if (itr->first == key1) {
    itr->second = data1;
    PPL_ASSERT(OK());
    return;
  }

  if ((size + 1) / (float) (((dimension_type)1 << max_depth) - 1)
    > max_density) {

    rebuild_bigger_tree();

    // itr was invalidated by the rebuild operation
    itr.get_root();
    lower_bound(itr, key1);

    PPL_ASSERT(itr->first != key1);
  }

  PPL_ASSERT((size + 1) / (float) (((dimension_type)1 << max_depth)-1)
         <= max_density);

  if (!itr.is_leaf()) {
    if (key1 < itr->first)
      itr.get_left_child();
    else
      itr.get_right_child();
    PPL_ASSERT(itr->first == unused_index);
    itr->first = key1;
    itr->second = data1;
    size++;

  } else {

    PPL_ASSERT(OK());

    rebalance(itr, key1, data1);

    // itr is no longer valid
  }
  PPL_ASSERT(OK());
}

void
PPL::CO_Tree::rebalance(inorder_iterator& itr, dimension_type key,
                        const data_type& value) {
#ifndef NDEBUG
  if (itr->first != unused_index) {
    PPL_ASSERT(!itr.get_left_child_value());
    PPL_ASSERT(!itr.get_right_child_value());
  }
#endif
  dimension_type height = max_depth + 1 - itr.depth();
  dimension_type subtree_size;
  const bool deleting = itr->first == unused_index;
  PPL_ASSERT(deleting || key != unused_index);
  if (deleting)
    subtree_size = 0;
  else
    // The existing element and the element with index key we want to add.
    subtree_size = 2;

  float density = subtree_size / (float) (((dimension_type)1 << height) - 1);
  const float coeff1 = (1 - max_density)/(max_depth - 1);
  const float coeff2 = (min_density - min_leaf_density)/(max_depth - 1);

  while (density > max_density + (itr.depth() - 1)*coeff1
         || density < min_density - (itr.depth() - 1)*coeff2) {
    if (itr.depth() == 1) {
      // TODO: Check if this is correct
      // We could arrive here because we are using a fake subtree_size.
#ifndef NDEBUG
      dimension_type real_subtree_size = subtree_size;
      if (!deleting)
        --real_subtree_size;
      float real_density
        = real_subtree_size / (float) (((dimension_type)1 << height) - 1);
      PPL_ASSERT(real_density <= max_density);
      if (real_density > min_density) {
        const float candidate_density
          = real_subtree_size
            / (float) (((dimension_type)1 << (height-1)) - 1);
        PPL_ASSERT(candidate_density > max_density);
      }
#endif
      break;
    }
    bool is_right_brother = itr.is_right_child();
    itr.get_parent();
    if (is_right_brother) {
      if (itr.get_left_child_value()) {
        subtree_size += count_used_in_subtree(itr);
        itr.get_parent();
      }
    } else {
      if (itr.get_right_child_value()) {
        subtree_size += count_used_in_subtree(itr);
        itr.get_parent();
      }
    }
    PPL_ASSERT(itr->first != unused_index);
    ++subtree_size;
    ++height;
    density = subtree_size / (float) (((dimension_type)1 << height) - 1);
  };

  redistribute_elements_in_subtree(itr, subtree_size, deleting, key, value);

  PPL_ASSERT(OK());
}

bool
PPL::CO_Tree::erase(dimension_type key) {
  PPL_ASSERT(key != unused_index);

  if (size == 0)
    return false;

  inorder_iterator itr(&*this);
  lower_bound(itr, key);

  if (itr->first != key)
    return false;

  erase(itr);

  return true;
}

void
PPL::CO_Tree::erase(inorder_iterator& itr) {
  PPL_ASSERT(!itr.is_before_begin());
  PPL_ASSERT(!itr.is_at_end());

  PPL_ASSERT(size != 0);

  if ((size - 1) / (float) (((dimension_type)1 << max_depth) - 1)
      < min_density
      && (size - 1) / (float) (((dimension_type)1 << (max_depth-1)) - 1)
      <= max_density) {
    const dimension_type key = itr->first;

    PPL_ASSERT(size <= (((dimension_type)1 << max_depth) - 1)*max_density);

    rebuild_smaller_tree();
    itr.get_root();
    lower_bound(itr, key);

    PPL_ASSERT(itr->first == key);
  }

#ifndef NDEBUG
  if (size > 1)
    if ((size - 1) / (float) (((dimension_type)1 << max_depth) - 1)
        < min_density)
      PPL_ASSERT((size - 1)
                 / (float) (((dimension_type)1 << (max_depth-1)) - 1)
                 > max_density);
#endif

  while (1) {
    dimension_type& current_key  = itr->first;
    data_type&      current_data = itr->second;
    if (itr.get_right_child_value()) {
      while (itr.get_left_child_value())
        ;
    } else
      if (itr.get_left_child_value()) {
        while (itr.get_right_child_value())
          ;
      } else
        break;
    std::swap(current_key, itr->first);
    move_data_element(current_data, itr->second);
  }

  PPL_ASSERT(itr->first != unused_index);
  itr->first = unused_index;
  --size;

  PPL_ASSERT(OK());

  static const data_type data = data_type();

  rebalance(itr, 0, data);

  PPL_ASSERT(OK());
}

PPL::dimension_type
PPL::CO_Tree::count_used_in_subtree(inorder_iterator& itr) {
  PPL_ASSERT(itr->first != unused_index);
  dimension_type n = 0;
  const dimension_type depth = itr.depth();
#ifndef NDEBUG
  const dimension_type root_index = itr->first;
#endif
  while (itr.get_left_child_value())
    ;
  while (itr.depth() != depth) {
    itr.get_next_value();
    ++n;
  }
  ++n;
  while (itr.get_right_child_value())
    ;
  while (itr.depth() != depth) {
    itr.get_previous_value();
    ++n;
  }

  PPL_ASSERT(itr->first == root_index);
  return n;
}

PPL::dimension_type
PPL::CO_Tree::count_used_in_subtree(inorder_const_iterator& itr) {
  PPL_ASSERT(itr->first != unused_index);
  dimension_type n = 0;
  const dimension_type depth = itr.depth();
#ifndef NDEBUG
  const dimension_type root_index = itr->first;
#endif
  while (itr.get_left_child_value())
    ;
  while (itr.depth() != depth) {
    itr.get_next_value();
    ++n;
  }
  ++n;
  while (itr.get_right_child_value())
    ;
  while (itr.depth() != depth) {
    itr.get_previous_value();
    ++n;
  }

  PPL_ASSERT(itr->first == root_index);
  return n;
}

void
PPL::CO_Tree::redistribute_elements_in_subtree(inorder_iterator& itr,
                                               dimension_type n,
                                               bool deleting,
                                               dimension_type key,
                                               const data_type& value) {
  // Step 1: compact elements of this subtree in the rightmost end, from right
  //         to left.
  inorder_iterator itr2 = itr;
  while (!itr2.is_leaf())
    itr2.get_right_child();
  bool added_key = false;
  bool can_add_key = true;
  if (deleting)
    added_key = true;
  compact_elements_in_the_rightmost_end(itr, itr2, n, key, value, added_key,
                                        can_add_key);
  if (!added_key && can_add_key) {
    PPL_ASSERT(itr2->first == unused_index);
    itr2->first = key;
    itr2->second = value;
    added_key = true;
  } else
    ++itr2;

  // Step 2: redistribute the elements, from left to right.
  redistribute_elements_in_subtree_helper(itr, n, itr2, key, value,
                                          added_key);

  if (!deleting)
    size++;

  PPL_ASSERT(structure_OK());
}

void
PPL::CO_Tree
::compact_elements_in_the_rightmost_end(inorder_iterator& root,
                                        inorder_iterator& first_unused,
                                        dimension_type subtree_size,
                                        dimension_type key,
                                        const data_type& value,
                                        bool& added_key,
                                        bool& can_add_key) {
  if (root->first == unused_index)
    return;

#ifndef NDEBUG
  inorder_iterator root_copy(root);
#endif

  const dimension_type depth = root.depth();

  while (root.get_right_child_value())
    ;

  if (!added_key && can_add_key)
    while (subtree_size != 0) {
      if (root.is_before_begin() || key > root->first) {
        if (!root.is_before_begin() && root == first_unused) {
          can_add_key = false;
          --subtree_size;
        } else {
          PPL_ASSERT(!first_unused.is_before_begin());
          PPL_ASSERT(first_unused->first == unused_index);
          first_unused->first = key;
          first_unused->second = value;
          added_key = true;
          --first_unused;
          --subtree_size;
        }
        break;
      } else {
        if (root != first_unused) {
          PPL_ASSERT(!first_unused.is_before_begin());
          PPL_ASSERT(!root.is_before_begin());
          PPL_ASSERT(first_unused->first == unused_index);
          first_unused->first = root->first;
          root->first = unused_index;
          move_data_element(first_unused->second, root->second);
        }
        root.get_previous_value();
        --first_unused;
        --subtree_size;
      }
    }
  while (subtree_size != 0) {
    if (root != first_unused) {
      PPL_ASSERT(!first_unused.is_before_begin());
      PPL_ASSERT(!root.is_before_begin());
      PPL_ASSERT(first_unused->first == unused_index);
      first_unused->first = root->first;
      root->first = unused_index;
      move_data_element(first_unused->second, root->second);
    }
    root.get_previous_value();
    --first_unused;
    --subtree_size;
  }

  // Restore the root iterator to the original position in the tree.
  ++root;
  while (root.depth() != depth)
    root.get_parent();
  PPL_ASSERT(root == root_copy);
}

void
PPL::CO_Tree
::redistribute_elements_in_subtree_helper(inorder_iterator& root,
                                          dimension_type subtree_size,
                                          inorder_iterator& itr,
                                          dimension_type key,
                                          const data_type& value,
                                          bool added_key) {
  // This is static and with static allocation, to improve performance.
  static std::pair<dimension_type,char> stack[5*8*sizeof(dimension_type)];
  dimension_type stack_first_empty = 0;

  // A pair (n, operation) in the stack means:
  //
  // * Go up, if operation is 0.
  // * Go left, then visit the current tree (with size n), if operation is 1.
  // * Go right, then visit the current tree (with size n), if operation is 2.
  // * Visit the current tree (with size n), if operation is 4.

  if (subtree_size == 0)
    return;

  stack[0].first = subtree_size;
  stack[0].second = 4;
  ++stack_first_empty;

  while (stack_first_empty != 0) {

    // top_n         = stack.top().first;
    // top_operation = stack.top().second;
    const dimension_type top_n = stack[stack_first_empty - 1].first;
    const char top_operation = stack[stack_first_empty - 1].second;

    switch (top_operation) {

    case 0:
      root.get_parent();
      --stack_first_empty;
      continue;

    case 1:
      root.get_left_child();
      break;

    case 2:
      root.get_right_child();
      break;
#ifndef NDEBUG
    case 4:
      break;

    default:
      // We should not be here
      PPL_ASSERT(false);
#endif
    }

    // We now visit the current tree

    if (top_n == 0) {
      --stack_first_empty;
    } else {
      if (top_n == 1) {
        if (!added_key && (itr.is_at_end() || itr->first > key)) {
          PPL_ASSERT(root != itr);
          added_key = true;
          root->first = key;
          root->second = value;
        } else {
          if (root != itr) {
            PPL_ASSERT(root->first == unused_index);
            root->first = itr->first;
            itr->first = unused_index;
            move_data_element(root->second, itr->second);
          }
          ++itr;
        }
        --stack_first_empty;
      } else {
        PPL_ASSERT(stack_first_empty + 3 < sizeof(stack)/sizeof(stack[0]));

        const dimension_type half = (top_n + 1) / 2;
        stack[stack_first_empty - 1].second = 0;
        stack[stack_first_empty    ] = std::make_pair(top_n - half, 2);
        stack[stack_first_empty + 1] = std::make_pair(1, 4);
        stack[stack_first_empty + 2].second = 0;
        stack[stack_first_empty + 3] = std::make_pair(half - 1, 1);
        stack_first_empty += 4;
      }
    }
  }

  PPL_ASSERT(added_key);
}


PPL::CO_Tree::inorder_iterator&
PPL::CO_Tree::inorder_iterator::operator=(const inorder_iterator& itr2) {
  tree = itr2.tree;
  if (tree != 0) {
    at_end = itr2.at_end;
    before_begin = itr2.before_begin;
    if (!at_end && !before_begin) {
      d = itr2.d;
      i = itr2.i;
      for (dimension_type i = 1; i <= itr2.d; ++i)
        pos[i] = itr2.pos[i];
    }
  }

  return *this;
}


PPL::CO_Tree::inorder_const_iterator&
PPL::CO_Tree::inorder_const_iterator
::operator=(const inorder_const_iterator& itr2) {
  tree = itr2.tree;
  if (tree != 0) {
    at_end = itr2.at_end;
    before_begin = itr2.before_begin;
    if (!at_end && !before_begin) {
      d = itr2.d;
      i = itr2.i;
      for (dimension_type i = 1; i <= itr2.d; ++i)
        pos[i] = itr2.pos[i];
    }
  }

  return *this;
}

PPL::CO_Tree::inorder_const_iterator&
PPL::CO_Tree::inorder_const_iterator
::operator=(const inorder_iterator& itr2) {
  tree = itr2.tree;
  if (tree != 0) {
    at_end = itr2.at_end;
    before_begin = itr2.before_begin;
    if (!at_end && !before_begin) {
      d = itr2.d;
      i = itr2.i;
      for (dimension_type i = 1; i <= itr2.d; ++i)
        pos[i] = itr2.pos[i];
    }
  }

  return *this;
}
