/*
 * FairStateEventSystemGraph.hh
 *
 *  Created on: Aug 27, 2013
 *      Author: kquine
 */

#ifndef FAIRSTATEEVENTSYSTEMGRAPH_HH_
#define FAIRSTATEEVENTSYSTEMGRAPH_HH_
#include "StateEventSystemGraph.hh"

namespace modelChecker {

template <typename SPH, typename EPH, typename SFH, typename EFH>
class FairStateEventSystemGraph: public StateEventSystemGraph<SPH,EPH>
{
	typedef StateEventSystemGraph<SPH,EPH>	Super;
	typedef typename Super::State			PreState;
	typedef typename Super::Transition		PreTransition;
public:
	FairStateEventSystemGraph(RewritingContext& initial, PropChecker& spc, PropChecker& epc, FairnessChecker& sfc, FairnessChecker& sefc, ProofTermGenerator& ptg);

private:
	struct State: public PreState, public SFH::Label
	{
		State(RewritingContext& parent, DagNode* stateDag): PreState(parent,stateDag) {}
	};
	struct Transition: public PreTransition, public EFH::Label
	{
		Transition(int nextState, int transitionIndex): PreTransition(nextState, transitionIndex) {}
		/* override */ bool operator<(const PreTransition& t) const;
	};

	/* override */ unique_ptr<PropSet> updateStateLabel(DagNode* stateDag, PreState& s);
	/* override */ unique_ptr<PreState> createState(DagNode* stateDag) const;

	/* override */ unique_ptr<PropSet> updateTransitionLabel(RewriteTransitionState& rts, PreTransition& t, PreState& s);
	/* override */ unique_ptr<PreTransition> createTransition(int nextState, int transitionIndex) const;

	SFH sfHandler;
	EFH sefHandler;

	FairnessChecker& stateFC;
	FairnessChecker& stateEventFC;
};

} /* namespace modelChecker */

#include "FairStateEventSystemGraph.cc"

#endif /* FAIRSTATEEVENTSYSTEMGRAPH_HH_ */
