m4_divert(-1)
m4_define(`dnl', `m4_dnl')
dnl
dnl m4_procedure_list
dnl This class using patterns wherever possible.
dnl Which classes the schema applies to is determined by +/-group_name.
dnl The group_names represent sets of PPL classes +group1 and -group2.
dnl These are defined in ../ppl_interface_generator_common_dat.m4.
dnl The actual classes the schema applies to is the set difference
dnl +group1 \ -group2 where a missing +group1 or -group2 is
dnl assumed to be the empty set.
dnl Where "@CLASS@" is replaced by the class name, then that class only
dnl is applicable for that schema.
dnl
dnl Note that the code for the schema "<name>_code" must be defined
dnl in the ppl_interface_generator_*_code.m4 file.
dnl The <name> must be exactly as written here.
dnl
m4_define(`m4_procedure_list', `dnl
ppl_delete_@CLASS@/1 +simple_pps,
ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension/3 +simple_pps,
ppl_new_@TOPOLOGY@@CLASS@_from_@INTOPOLOGY@@FRIEND@/2 +simple_pps,
ppl_new_@TOPOLOGY@@CLASS@_from_@BUILD_REPRESENT@s/2 +simple_pps,
ppl_new_@TOPOLOGY@@CLASS@_from_@BOX@/2 +simple -wr_shape,
ppl_@TOPOLOGY@@CLASS@_@UB_EXACT@/2 +polyhedron,
ppl_@CLASS@_swap/2 +simple_pps,
ppl_@CLASS@_@DIMENSION@/2 +simple_pps,
ppl_@CLASS@_get_@GET_REPRESENT@s/2 +simple,
ppl_@CLASS@_get_minimized_@GET_REPRESENT@s/2 +simple,
ppl_@CLASS@_get_disjuncts/2 +pointset_powerset,
ppl_@CLASS@_relation_with_@RELATION_REPRESENT@/3 +simple,
ppl_@CLASS@_get_bounding_box/3 +simple,
ppl_@CLASS@_get_covering_box/2 +grid,
ppl_@CLASS@_@HAS_PROPERTY@/1 +simple,
ppl_@CLASS@_@SIMPLIFY@/1 +simple_pps,
ppl_@CLASS@_bounds_from_@ABOVEBELOW@/2 +simple -wr_shape,
ppl_@CLASS@_@MAXMIN@/5 +simple,
ppl_@CLASS@_@MAXMIN@_with_point/6 +simple,
ppl_@CLASS@_@COMPARISON@_@CLASS@/2 +simple_pps,
ppl_@CLASS@_equals_@CLASS@/2 +simple,
ppl_@CLASS@_OK/1 +simple_pps,
ppl_@CLASS@_add_disjunct/2 +pointset_powerset,
ppl_@CLASS@_add_@ADD_REPRESENT@/2 +simple_pps,
ppl_@CLASS@_add_@ADD_REPRESENT@_and_minimize/2 +simple_pps,
ppl_@CLASS@_add_@ADD_REPRESENT@s/2 +simple_pps,
ppl_@CLASS@_add_@ADD_REPRESENT@s_and_minimize/2 +simple_pps,
ppl_@CLASS@_@BINOP@/2 +simple_pps,
ppl_@CLASS@_@BINMINOP@/2 +simple,
ppl_@CLASS@_@AFFIMAGE@/4 +simple,
ppl_@CLASS@_bounded_@AFFIMAGE@/5 +shape -wr_shape,
ppl_@CLASS@_generalized_@AFFIMAGE@/5 +shape,
ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs/4 +shape,
ppl_Grid_generalized_@AFFIMAGE@/6 +grid,
ppl_Grid_generalized_@AFFIMAGE@_lhs_rhs/5 +grid,
ppl_@CLASS@_@WIDEN@_widening_assign_with_tokens/4 +simple,
ppl_@CLASS@_@WIDEN@_widening_assign/2 +simple,
ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_with_tokens/5 +simple,
ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign/3 +simple,
ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_with_tokens/4 +wr_shape,
ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign/2 +wr_shape,
ppl_@CLASS@_@EXTRAPOLATION@_narrowing_assign/2 +wr_shape,
ppl_@CLASS@_add_space_dimensions_@EMBEDPROJECT@/2 +simple_pps,
ppl_@CLASS@_remove_space_dimensions/2 +simple_pps,
ppl_@CLASS@_remove_higher_space_dimensions/2 +simple_pps,
ppl_@CLASS@_expand_space_dimension/3 +simple,
ppl_@CLASS@_fold_space_dimensions/3  +simple,
ppl_@CLASS@_map_space_dimensions/2 +simple_pps
')`'dnl
dnl
m4_divert`'dnl
