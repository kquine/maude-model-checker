/*
 * FairStateSystemGraph.hh
 *
 *  Created on: Aug 27, 2013
 *      Author: kquine
 */

#ifndef FAIRSTATESYSTEMGRAPH_HH_
#define FAIRSTATESYSTEMGRAPH_HH_
#include "StateSystemGraph.hh"

namespace modelChecker {

template <typename PropHandler, typename FairHandler>
class FairStateSystemGraph: public StateSystemGraph<PropHandler>
{
	typedef StateSystemGraph<PropHandler>	Super;
	typedef typename Super::State			PreState;
public:
	FairStateSystemGraph(RewritingContext& initial, PropChecker& spc, FairnessChecker& fc, ProofTermGenerator& ptg);

private:
	struct State: public PreState, public FairHandler::Label
	{
		State(RewritingContext& parent, DagNode* stateDag): PreState(parent, stateDag) {}
	};

	/* override */ unique_ptr<PropSet> updateStateLabel(DagNode* stateDag, PreState& s);
	/* override */ unique_ptr<PreState> createState(DagNode* stateDag) const;

	FairHandler fHandler;
	FairnessChecker& fairC;
};

} /* namespace modelChecker */

#include "FairStateSystemGraph.cc"

#endif /* FAIRSTATESYSTEMGRAPH_HH_ */
