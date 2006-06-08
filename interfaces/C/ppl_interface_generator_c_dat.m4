divert(-1)

dnl Include the language independent `dat' information.
include(`ppl_interface_generator_common_dat.m4')

dnl m4_procedure_list
dnl This class using patterns wherever possible.
dnl Which classes the schema applies to is determined by the following codes:
dnl If code is POINTS = the point-domain classes ie grid and polyhedra classes;
dnl            All = all classes
dnl            SHAPE = the polyhedra-shape classes;
dnl A class name with an "X" in front means it is not included.
dnl Where "4CLASS4" is replaced by the class name, then that class only
dnl is applicable for that schema.
dnl
dnl Note that the code for the schema "<name>_code" must be defined
dnl in the ppl_interface_generator_*_code.m4 file.
dnl The <name> must be exactly as written here.
dnl
define(`m4_procedure_list', `dnl
ppl_new_4TOPOLOGY44CLASS4_from_space_dimension POINTS,
ppl_new_4TOPOLOGY44CLASS4_from_4INTOPOLOGY44CLASS4 All,
ppl_new_4TOPOLOGY44CLASS4_from_4UREPRESENT4_System POINTS,
ppl_new_4TOPOLOGY44CLASS4_recycle_4UREPRESENT4_System POINTS,
ppl_new_4TOPOLOGY44CLASS4_from_4BOX4 POINTS XBD_Shape,
ppl_delete_4CLASS4 All,
ppl_assign_4TOPOLOGY44CLASS4_from_4TOPOLOGY44CLASS4 POINTS,
ppl_4CLASS4_4DIMENSION4 All,
ppl_4CLASS4_4DESCRIBE4s POINTS,
ppl_4CLASS4_minimized_4DESCRIBE4s POINTS,
ppl_4CLASS4_relation_with_4UDESCRIBE4 POINTS,
ppl_4CLASS4_shrink_4BOX4 SHAPE,
ppl_Grid_get_4BOX4 GRID,
ppl_4CLASS4_is_4STATE4 POINTS,
ppl_4CLASS4_topological_closure_assign POINTS,
ppl_4CLASS4_bounds_from_4ABOVEBELOW4 POINTS XBD_Shape,
ppl_4CLASS4_4MAXMIN4 POINTS XBD_Shape,
ppl_4CLASS4_4MAXMIN4_with_point POINTS XBD_Shape,
ppl_4CLASS4_4COMPARISON4_4CLASS4 POINTS,
ppl_4CLASS4_equals_4CLASS4 POINTS,
ppl_4CLASS4_OK All,
ppl_4CLASS4_add_4REPRESENT4 POINTS,
ppl_4CLASS4_add_4REPRESENT4_and_minimize POINTS,
ppl_4CLASS4_add_4REPRESENT4s POINTS,
ppl_4CLASS4_add_4REPRESENT4s_and_minimize POINTS,
ppl_4CLASS4_add_recycled_4REPRESENT4 POINTS,
ppl_4CLASS4_add_recycled_4REPRESENT4_and_minimize POINTS,
ppl_4CLASS4_add_recycled_4REPRESENT4s POINTS,
ppl_4CLASS4_add_recycled_4REPRESENT4s_and_minimize POINTS,
ppl_4CLASS4_4BINOP4 POINTS,
ppl_4CLASS4_4BINOPPMIN4 POINTS,
ppl_4CLASS4_4AFFIMAGE4 POINTS,
ppl_4CLASS4_bounded_4AFFIMAGE4 SHAPE XBD_Shape,
ppl_4CLASS4_generalized_4AFFIMAGE4 SHAPE,
ppl_4CLASS4_generalized_4AFFIMAGE4_lhs_rhs SHAPE,
ppl_Grid_generalized_4AFFIMAGE4 GRID,
ppl_Grid_generalized_4AFFIMAGE4_lhs_rhs GRID,
ppl_4CLASS4_4WIDENEXP4_widening_assign_with_tokens POINTS,
ppl_4CLASS4_4WIDENEXP4_widening_assign POINTS,
ppl_4CLASS4_limited_4WIDENEXP4_extrapolation_assign_with_tokens POINTS,
ppl_4CLASS4_limited_4WIDENEXP4_extrapolation_assign POINTS,
ppl_4CLASS4_bounded_4WIDENEXP4_extrapolation_assign_with_tokens POINTS XGrid XBD_Shape,
ppl_4CLASS4_bounded_4WIDENEXP4_extrapolation_assign POINTS XGrid XBD_Shape,
ppl_BD_Shape_CC76_narrowing_assign,
ppl_4CLASS4_add_space_dimensions_4EMBEDPROJECT4 POINTS,
ppl_4CLASS4_remove_space_dimensions POINTS,
ppl_4CLASS4_remove_higher_space_dimensions POINTS,
ppl_4CLASS4_expand_space_dimension POINTS,
ppl_4CLASS4_fold_space_dimensions POINTS,
ppl_4CLASS4_map_space_dimensions POINTS
')

divert`'dnl

