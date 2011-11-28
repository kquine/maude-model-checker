/*
 * RewriteStateTransitionContainer.hh
 *
 *  Created on: Jun 7, 2011
 *      Author: kquine
 */

#ifndef REWRITESTATETRANSITIONCONTAINER_HH_
#define REWRITESTATETRANSITIONCONTAINER_HH_
#include "hashConsSet.hh"
#include "Util/PtrVector.hh"
#include "Interface/ProofTermGenerator.hh"
#include "StateDagContainer.hh"
#include "TransitionState.hh"

namespace ltlrModelChecker2 {

class RewriteGraphContainer: private StateDagContainer
{
public:
	RewriteGraphContainer(RewritingContext* initial, ProofTermGenerator* proofTerm);

	StateDagContainer::getNrStates;
	StateDagContainer::getStateDag;

	bool isFullyExplored(int stateNr);
	pair<int,DagNode*> computeNextState(int stateNr);

private:
	//
	//	State information
	//
	PtrVector<TransitionState> states;
	//
	//	Transition information
	//
	ProofTermGenerator* proofTerm;
	//
	//	Rewriting information
	//
	RewritingContext* initial;
};

}

#endif /* REWRITESTATETRANSITIONCONTAINER_HH_ */
