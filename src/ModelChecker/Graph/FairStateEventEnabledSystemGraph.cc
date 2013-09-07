/*
 * FairStateEventEnabledSystemGraph.cc
 *
 *  Created on: Aug 27, 2013
 *      Author: kquine
 */

// utility stuff
#include "macros.hh"
#include "vector.hh"

// forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "freeTheory.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

// interface class definitions
#include "symbol.hh"
#include "dagNodeSet.hh"

// core class definitions
#include "rewritingContext.hh"

// ltlr definitions
#include "FairHandler.hh"
#include "FairStateEventEnabledSystemGraph.hh"

namespace modelChecker {


template <typename SPH, typename EPH, typename SFH, typename EFH>
FairStateEventEnabledSystemGraph<SPH,EPH,SFH,EFH>::FairStateEventEnabledSystemGraph(RewritingContext& initial,
		PropChecker& spc, PropChecker& epc, const EnabledPropTransferer& enpc,
		FairnessChecker& sfc, FairnessChecker& sefc, ProofTermGenerator& ptg):
	Super(initial,spc,epc,ptg), enpc(enpc), stateFC(sfc), stateEventFC(sefc) {}

template <typename SPH, typename EPH, typename SFH, typename EFH> bool
FairStateEventEnabledSystemGraph<SPH,EPH,SFH,EFH>::Transition::operator<(const PreTransition& t) const
{
	return PreTransition::operator<(t) || EFH::Label::operator<(static_cast<const Transition&>(t));
}

template <typename SPH, typename EPH, typename SFH, typename EFH>
unique_ptr<typename FairStateEventEnabledSystemGraph<SPH,EPH,SFH,EFH>::PreState>
FairStateEventEnabledSystemGraph<SPH,EPH,SFH,EFH>::createState() const
{
	return unique_ptr<PreState>(new State);
}

template <typename SPH, typename EPH, typename SFH, typename EFH>
unique_ptr<typename FairStateEventEnabledSystemGraph<SPH,EPH,SFH,EFH>::PreTransition>
FairStateEventEnabledSystemGraph<SPH,EPH,SFH,EFH>::createTransition(int nextState, int transitionIndex) const
{
	return unique_ptr<PreTransition>(new Transition(nextState, transitionIndex));
}

template<typename SPH, typename EPH, typename SFH, typename EFH>
unique_ptr<typename FairStateEventEnabledSystemGraph<SPH,EPH,SFH,EFH>::LabelSet>
FairStateEventEnabledSystemGraph<SPH,EPH,SFH,EFH>::updateAllLabels(DagNode* stateDag, const vector<DagNode*>& proofDags, PreState& s, vector<unique_ptr<PreTransition> >& trs)
{
	unique_ptr<LabelSet> trueProps(Super::updateAllLabels(stateDag, proofDags, s, trs));

	enpc.computeEnabledPropIDs(*trueProps->stateProps, trueProps->eventProps); 		// compute all enabled props for (state and/or event) fairness conditions

	unique_ptr<FairSet> sfs(stateFC.computeAllFairness(*trueProps->stateProps));	// compute all state fairness conditions
	sfHandler.updateLabel(*sfs, static_cast<State&>(s)); 							// store the state fairness conditions

	for (unsigned int i = 0; i < trs.size(); ++i)
	{
		trueProps->eventProps[i]->setTrue(*trueProps->stateProps); 								// transfer truth
		unique_ptr<FairSet> efs(stateEventFC.computeAllFairness(*trueProps->eventProps[i])); 		// compute all SE fairness conditions
		sefHandler.updateLabel(*efs, static_cast<Transition&>(*trs[i])); 						// store the SE fairness conditions
	}
	return trueProps;
}


} /* namespace modelChecker */
