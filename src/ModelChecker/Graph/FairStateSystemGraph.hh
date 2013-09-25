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

template <typename PL, typename FL>
class FairStateSystemGraph: public StateSystemGraph<PL>
{
public:
	FairStateSystemGraph(unique_ptr<PL>&& spl, unique_ptr<FL>&& sfl, RewritingContext& initial, const ProofTermGenerator& ptg, const PropositionTable& propTable);

	unique_ptr<FairSet> makeFairSet(unsigned int stateNr, unsigned int transitionNr) const;

private:
	using Super =		StateSystemGraph<PL>;
	using PreState =	typename Super::State;

	struct State: public PreState, public FL::StateLabel
	{
		State(RewritingContext& parent, DagNode* stateDag): PreState(parent, stateDag) {}
	};

	unique_ptr<PropSet> updateStateLabel(DagNode* stateDag, PreState& s) override;
	unique_ptr<PreState> createState(DagNode* stateDag) const override;

	const unique_ptr<FL> fairLabel;
};

} /* namespace modelChecker */

#include "FairStateSystemGraph.cc"	// to separate the implementation for the template

#endif /* FAIRSTATESYSTEMGRAPH_HH_ */
