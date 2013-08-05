/*
 * TransitionState.hh
 *
 *  Created on: May 23, 2011
 *      Author: kquine
 */

#ifndef REWRITETRANSITIONSTATE_HH_
#define REWRITETRANSITIONSTATE_HH_
#include "rewriteSearchState.hh"
#include "Interface/ProofTermGenerator.hh"

namespace modelChecker {

class TransitionState
{
	NO_COPYING(TransitionState);

public:
	TransitionState(RewritingContext* parent, DagNode* stateDag);
	DagNode* getNextStateDag(RewritingContext* parent);
	DagNode* getCurrProofDag(ProofTermSymbol* pfg) const;
	bool isFullyExplored() const;

private:
	auto_ptr<RewriteSearchState> rewriteState;
};

}

#endif /* REWRITETRANSITIONSTATE_HH_ */
