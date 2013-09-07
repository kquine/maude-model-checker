/*
 * ParamVarInfo.cc
 *
 *  Created on: Sep 3, 2013
 *      Author: kquine
 */

// utility stuff
#include "macros.hh"
#include "vector.hh"

// forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "freeTheory.hh"
#include "NA_Theory.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

// core class definitions
#include "dagArgumentIterator.hh"

// symbols
#include "freeSymbol.hh"
#include "NA_Symbol.hh"

// dagnodes
#include "freeDagNode.hh"
#include "variableDagNode.hh"

// ltlr definitions
#include "ParamVarInfo.hh"

namespace modelChecker {

ParamVarInfo::ParamVarInfo(DagNode* dag)
{
	indexVariables(dag);
}

int
ParamVarInfo::getNrVariables() const
{
	return variables.size();
}

int
ParamVarInfo::variable2Index(VariableDagNode* variableDag) const
{
	indexed_set<VariableDagNode*>::const_iterator ci = variables.find(variableDag);
	if (ci != variables.end())
		return ci->second;
	else
		return NONE;
}

int
ParamVarInfo::variable2Index(Term* variableTerm) const
{
	for (auto i : variables)
	{
		if (variableTerm->equal(i.first))
			return i.second;
	}
	return NONE;
}

VariableDagNode*
ParamVarInfo::index2Variable(int index) const
{
	return variables[index];
}


void
ParamVarInfo::indexVariables(DagNode* dag)
{
	if (dag->isGround())
		return;
	else
	{
		if (dynamic_cast<VariableSymbol*>(dag->symbol()))
			variables.insert(static_cast<VariableDagNode*>(dag));
		else
		{
			for (DagArgumentIterator i(*dag); i.valid(); i.next())
				indexVariables(i.argument());
		}
	}
}


} /* namespace modelChecker */
