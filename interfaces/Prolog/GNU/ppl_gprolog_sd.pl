:- foreign(ppl_init).
:- foreign(ppl_initialize).
:- foreign(ppl_finalize).
:- foreign(ppl_new_C_Polyhedron_from_dimension(+term, +term)).
:- foreign(ppl_new_NNC_Polyhedron_from_dimension(+term, +term)).
:- foreign(ppl_new_C_Polyhedron_empty_from_dimension(+term, +term)).
:- foreign(ppl_new_NNC_Polyhedron_empty_from_dimension(+term, +term)).
:- foreign(ppl_new_C_Polyhedron_from_C_Polyhedron(+term, +term)).
:- foreign(ppl_new_C_Polyhedron_from_NNC_Polyhedron(+term, +term)).
:- foreign(ppl_new_NNC_Polyhedron_from_C_Polyhedron(+term, +term)).
:- foreign(ppl_new_NNC_Polyhedron_from_NNC_Polyhedron(+term, +term)).
:- foreign(ppl_new_C_Polyhedron_from_ConSys(+term, +term)).
:- foreign(ppl_new_NNC_Polyhedron_from_ConSys(+term, +term)).
:- foreign(ppl_new_C_Polyhedron_from_GenSys(+term, +term)).
:- foreign(ppl_new_NNC_Polyhedron_from_GenSys(+term, +term)).
:- foreign(ppl_delete_Polyhedron(+term)).
:- foreign(ppl_Polyhedron_space_dimension(+term, +term)).
:- foreign(ppl_Polyhedron_intersection_assign(+term, +term)).
:- foreign(ppl_Polyhedron_intersection_assign_and_minimize(+term, +term)).
:- foreign(ppl_Polyhedron_poly_hull_assign(+term, +term)).
:- foreign(ppl_Polyhedron_poly_hull_assign_and_minimize(+term, +term)).
:- foreign(ppl_Polyhedron_poly_difference_assign(+term, +term)).
:- foreign(ppl_Polyhedron_poly_difference_assign_and_minimize(+term, +term)).
:- foreign(ppl_Polyhedron_widening_assign(+term, +term)).
:- foreign(ppl_Polyhedron_limited_widening_assign(+term, +term)).
:- foreign(ppl_Polyhedron_get_constraints(+term, +term)).
:- foreign(ppl_Polyhedron_get_minimized_constraints(+term, +term)).
:- foreign(ppl_Polyhedron_get_generators(+term, +term)).
:- foreign(ppl_Polyhedron_get_minimized_generators(+term, +term)).
:- foreign(ppl_Polyhedron_add_constraint(+term, +term)).
:- foreign(ppl_Polyhedron_add_generator(+term, +term)).
:- foreign(ppl_Polyhedron_add_constraints(+term, +term)).
:- foreign(ppl_Polyhedron_add_constraints_and_minimize(+term, +term)).
:- foreign(ppl_Polyhedron_add_generators(+term, +term)).
:- foreign(ppl_Polyhedron_add_generators_and_minimize(+term, +term)).
:- foreign(ppl_Polyhedron_add_dimensions_and_constraints(+term, +term)).
:- foreign(ppl_Polyhedron_add_dimensions_and_project(+term, +term)).
:- foreign(ppl_Polyhedron_add_dimensions_and_embed(+term, +term)).
:- foreign(ppl_Polyhedron_remove_dimensions(+term, +term)).
:- foreign(ppl_Polyhedron_remove_higher_dimensions(+term, +term)).
:- foreign(ppl_Polyhedron_affine_image(+term, +term, +term, +term)).
:- foreign(ppl_Polyhedron_affine_preimage(+term, +term, +term, +term)).
:- foreign(ppl_Polyhedron_relation_with_constraint(+term, +term, +term)).
:- foreign(ppl_Polyhedron_relation_with_generator(+term, +term, +term)).
:- foreign(ppl_Polyhedron_check_empty(+term)).
:- foreign(ppl_Polyhedron_check_universe(+term)).
:- foreign(ppl_Polyhedron_is_bounded(+term)).
:- foreign(ppl_Polyhedron_contains_Polyhedron(+term, +term)).
:- foreign(ppl_Polyhedron_strictly_contains_Polyhedron(+term, +term)).
:- foreign(ppl_Polyhedron_equals_Polyhedron(+term, +term)).
