/*
 * FairStateEventEnabledSystemGraph.hh
 *
 *  Created on: Aug 27, 2013
 *      Author: kquine
 */

#ifndef FAIRSTATEEVENTENABLEDSYSTEMGRAPH_HH_
#define FAIRSTATEEVENTENABLEDSYSTEMGRAPH_HH_
#include "StateEventEnabledSystemGraph.hh"

namespace modelChecker {

template <typename SPH, typename EPH, typename SFH, typename EFH>
class FairStateEventEnabledSystemGraph: public StateEventEnabledSystemGraph<SPH,EPH>
{
	typedef StateEventEnabledSystemGraph<SPH,EPH>	Super;
	typedef typename Super::State					PreState;
	typedef typename Super::Transition				PreTransition;
	typedef typename Super::LabelSet				LabelSet;
public:
	FairStateEventEnabledSystemGraph(RewritingContext& initial, PropChecker& spc, PropChecker& epc, const EnabledPropTransferer& enpc,
			FairnessChecker& sfc, FairnessChecker& sefc, ProofTermGenerator& ptg);

private:
	struct State: public PreState, public SFH::Label
	{
		/* */
	};
	struct Transition: public PreTransition, public EFH::Label
	{
		Transition(int nextState, int transitionIndex): PreTransition(nextState, transitionIndex) {}
		/* override */ bool operator<(const PreTransition& t) const;
	};

	/* override */ unique_ptr<LabelSet> updateAllLabels(DagNode* stateDag, const vector<DagNode*>& proofDags, PreState& s, vector<unique_ptr<PreTransition> >& trs);
	/* override */ unique_ptr<PreState> createState() const;
	/* override */ unique_ptr<PreTransition> createTransition(int nextState, int transitionIndex) const;

	SFH sfHandler;
	EFH sefHandler;

	const EnabledPropTransferer& enpc;
	FairnessChecker& stateFC;
	FairnessChecker& stateEventFC;
};

} /* namespace modelChecker */

#include "FairStateEventEnabledSystemGraph.cc"

#endif /* FAIRSTATEEVENTENABLEDSYSTEMGRAPH_HH_ */
