/*
 * StatePropHandler.cc
 *
 *  Created on: Aug 13, 2013
 *      Author: kquine
 */

// utility stuff
#include "macros.hh"
#include "vector.hh"

// forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "freeTheory.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

// interface class definitions
#include "symbol.hh"
#include "dagNodeSet.hh"

// core class definitions
#include "rewritingContext.hh"

// ltlr definitions
#include "StatePropHandler.hh"

namespace modelChecker {

StatePropHandler::StatePropHandler(const DagNodeSet& atoms, const NatSet& stateProps, const PropChecker& spc):
	atoms(atoms), stateProps(stateProps), spChecker(spc) {}

bool
StatePropHandler::satisfiesStateProp(int propId, const StateLabel& s) const
{
	return s.label.contains(propId);
}

void
StatePropHandler::updateStateLabel(DagNode* stateDag, StateLabel* s) const
{
	FOR_EACH_CONST(i, NatSet, stateProps)
	{
		if (spChecker.computeLabel(stateDag, atoms.index2DagNode(*i)))
			s->label.insert(*i);
	}

}

} /* namespace modelChecker */
