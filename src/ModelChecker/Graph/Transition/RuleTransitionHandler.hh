/*
 * RuleTransitionHandler.hh
 *
 *  Created on: Jun 30, 2011
 *      Author: kquine
 */

#ifndef RULETRANSITIONHANDLER_HH_
#define RULETRANSITIONHANDLER_HH_
#include "Interface/ProofTermGenerator.hh"
#include "Graph/RewriteTransitionState.hh"

namespace modelChecker {

class RuleTransitionHandler
{
public:
	struct StateData		{ };
	struct TransitionData
	{
		TransitionData(): rule(NULL) {}
		const Rule* rule;
	};

	RuleTransitionHandler(ProofTermSymbol* pfg): pfg(pfg) {}

	DagNode* getDag(const TransitionData* thData, DagNode* stateDag);
	void updateStateData(StateData* sdata);
	void setTransitionData(TransitionData* td, const StateData* sd, const RewriteTransitionState* rs);
private:
	ProofTermSymbol* pfg;
};

inline DagNode*
RuleTransitionHandler::getDag(const TransitionData* thData, DagNode* stateDag)
{
	return pfg->makeProofDag(NULL, *thData->rule, NULL);
}

inline void
RuleTransitionHandler::updateStateData(StateData* sdata)
{
}

inline void
RuleTransitionHandler::setTransitionData(TransitionData* td, const StateData* sd, const RewriteTransitionState* rs)
{
	td->rule = rs->getRule();
}

}

#endif /* RULETRANSITIONHANDLER_HH_ */
