/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2003 SRI International, Menlo Park, CA 94025, USA.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.

*/

//
//	Class for subproblems created by variable abstraction.
//
#ifndef _variableAbstractionSubproblem_hh_
#define _variableAbstractionSubproblem_hh_
#include "subproblem.hh"

class VariableAbstractionSubproblem : public Subproblem
{
public:
  VariableAbstractionSubproblem(LhsAutomaton* abstractedPattern,
				int abstractionVariable,
				int nrVariables);
  ~VariableAbstractionSubproblem();

  bool solve(bool findFirst, RewritingContext& solution);

private:
  LhsAutomaton* abstractedPattern;
  int abstractionVariable;
  LocalBinding* difference;
  Subproblem* subproblem;
  Substitution local;
};

#endif
