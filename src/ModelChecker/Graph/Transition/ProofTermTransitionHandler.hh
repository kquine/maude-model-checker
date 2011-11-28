/*
 * ProofTermTransitionHandler.hh
 *
 *  Created on: Jun 30, 2011
 *      Author: kquine
 */

#ifndef PROOFTERMTRANSITIONHANDLER_HH_
#define PROOFTERMTRANSITIONHANDLER_HH_
#include "Interface/ProofTermGenerator.hh"
#include "Graph/RewriteTransitionState.hh"

namespace modelChecker {

class ProofTermTransitionHandler
{
public:
	struct StateData
	{
		StateData(): count(-1) {}
		int count;
	};
	struct TransitionData	{ int transitionCount; };

	ProofTermTransitionHandler(ProofTermGenerator* pfg, RewritingContext* parentContext);

	DagNode* getDag(const TransitionData* thData, DagNode* stateDag);
	void updateStateData(StateData* sdata);
	void setTransitionData(TransitionData* td, const StateData* sd, const RewriteTransitionState* rs);
private:
	ProofTermGenerator* pfg;
	RewritingContext* parent;
};

inline
ProofTermTransitionHandler::ProofTermTransitionHandler(ProofTermGenerator* pfg, RewritingContext* parentContext):
	pfg(pfg), parent(parentContext) {}

inline DagNode*
ProofTermTransitionHandler::getDag(const TransitionData* thData, DagNode* stateDag)
{
	auto_ptr<RewriteTransitionState>  tr(new RewriteTransitionState(parent, stateDag));
	for (int i = -1; i < thData->transitionCount; ++i)
		tr->getNextStateDag(parent);
	return pfg->makeProofDag(tr->getPosition(), *tr->getRule(), tr->getSubstitution());
}

inline void
ProofTermTransitionHandler::updateStateData(StateData* sdata)
{
	sdata->count++;
}

inline void
ProofTermTransitionHandler::setTransitionData(TransitionData* td, const StateData* sd, const RewriteTransitionState* rs)
{
	td->transitionCount = sd->count;
}

}

#endif /* PROOFTERMTRANSITIONHANDLER_HH_ */
