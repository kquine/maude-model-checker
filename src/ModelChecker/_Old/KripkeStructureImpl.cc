/*
 * KripkeStructureImpl.cc
 *
 *  Created on: May 24, 2011
 *      Author: kquine
 */

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "freeTheory.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

#include "KripkeStructureImpl.hh"

namespace ltlrModelChecker2 {

int
KripkeStructureImpl::State::getNextState(int index) const
{
	return nextStates.ithElement(index);
}

bool
KripkeStructureImpl::State::insertTransition(const KripkeStructure* /* ks */, int nextState)
{
	return nextStates.insert(nextState) == transitionCounts.size();		// true if new transition identified
}

KripkeStructureImpl::State*
KripkeStructureImpl::createNewState(DagNode* stateDag) const
{
	return new State(initial, stateDag);
}


}
