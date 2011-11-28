/*
 * RewriteStateTransitionContainer.cc
 *
 *  Created on: Jun 7, 2011
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
#include "rewriteSearchState.hh"

#include "RewriteGraphContainer.hh"

namespace ltlrModelChecker2 {

RewriteGraphContainer::RewriteGraphContainer(RewritingContext* initial, ProofTermGenerator* proofTerm):
	initial(initial), proofTerm(proofTerm) {}

bool
RewriteGraphContainer::isFullyExplored(int stateNr)
{
	Assert(stateNr < states.size(), "Invalid state lookup (RewriteStateTransitionContainer::isFullyExplored)");
	return states[stateNr] == NULL;
}

pair<int,DagNode*>
RewriteGraphContainer::computeNextState(int stateNr)
{
	Assert(stateNr < states.size(), "Invalid state lookup (RewriteStateTransitionContainer::computeNextState)");
	TransitionState* currState = states[stateNr];
	DagNode* ns = currState->getNextStateDag(initial);
	int nextState = NONE;
	DagNode* proofDag = NULL;
	if (ns != NULL)
	{
		nextState = insertState(ns);
		if (nextState == states.size())
			states.append(new TransitionState(initial, getStateDag(nextState)));
		proofDag = currState->getCurrProofDag(proofTerm);
	}
	return make_pair(nextState,proofDag);
}


}
