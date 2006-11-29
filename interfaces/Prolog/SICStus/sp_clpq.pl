/* Main program for the toy PPL/SICStus-Prolog CLP(Q) interpreter.
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

:- set_prolog_flag(language, iso).

:- ensure_loaded('ppl_sicstus.pl').
:- use_module(library(lists), [append/3, member/2]).

eat_eol :-
	get_code(user_input, _EOL).

main(CLPQ) :-
    ensure_loaded(CLPQ),
    set_prolog_flag(fileerrors, off),
    common_main.
