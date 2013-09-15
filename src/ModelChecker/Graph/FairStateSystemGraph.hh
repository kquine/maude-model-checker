/*
 * FairStateSystemGraph.hh
 *
 *  Created on: Aug 27, 2013
 *      Author: kquine
 */

#ifndef FAIRSTATESYSTEMGRAPH_HH_
#define FAIRSTATESYSTEMGRAPH_HH_
#include "StateSystemGraph.hh"
#include "FairChecker/FairnessChecker.hh"

namespace modelChecker {

class FairStateSystemGraph: public StateSystemGraph
{
public:
	FairStateSystemGraph(RewritingContext& initial, PropChecker& spc, const NatSet& formulaPropIds, FairnessChecker& fc, const ProofTermGenerator& ptg);

	unique_ptr<FairSet> makeFairSet(int stateNr, int transitionNr) const;

private:
	using Super =		StateSystemGraph;
	using PreState =	typename Super::State;

	struct State: public PreState
	{
		State(RewritingContext& parent, DagNode* stateDag): PreState(parent, stateDag) {}

		unique_ptr<FairSet> fs;
	};

	unique_ptr<PropSet> updateStateLabel(DagNode* stateDag, PreState& s) override;
	unique_ptr<PreState> createState(DagNode* stateDag) const override;

	FairnessChecker& fairC;
	const NatSet& formulaPropIds;
};

} /* namespace modelChecker */

#endif /* FAIRSTATESYSTEMGRAPH_HH_ */
