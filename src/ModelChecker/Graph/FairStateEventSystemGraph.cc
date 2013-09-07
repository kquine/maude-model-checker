/*
 * FairStateEventSystemGraph.cc
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
#include "FairStateEventSystemGraph.hh"

namespace modelChecker {

template <typename SPH, typename EPH, typename SFH, typename EFH>
FairStateEventSystemGraph<SPH,EPH,SFH,EFH>::FairStateEventSystemGraph(RewritingContext& initial, PropChecker& spc, PropChecker& epc,
		FairnessChecker& sfc, FairnessChecker& sefc, ProofTermGenerator& ptg):
	Super(initial,spc,epc,ptg), stateFC(sfc), stateEventFC(sefc) {}

template <typename SPH, typename EPH, typename SFH, typename EFH> bool
FairStateEventSystemGraph<SPH,EPH,SFH,EFH>::Transition::operator<(const PreTransition& t) const
{
	return PreTransition::operator<(t) || EFH::Label::operator<(static_cast<const Transition&>(t));
}

template <typename SPH, typename EPH, typename SFH, typename EFH>
unique_ptr<PropSet>
FairStateEventSystemGraph<SPH,EPH,SFH,EFH>::updateStateLabel(DagNode* stateDag, PreState& s)
{
	unique_ptr<PropSet> truePropIds = Super::updateStateLabel(stateDag, s);
	unique_ptr<FairSet> fs(stateFC.computeAllFairness(*truePropIds));				// compute all state fairness conditions
	sfHandler.updateLabel(*fs, static_cast<State&>(s));						// store the state fairness conditions
	return truePropIds;
}

template <typename SPH, typename EPH, typename SFH, typename EFH>
unique_ptr<typename FairStateEventSystemGraph<SPH,EPH,SFH,EFH>::PreState>
FairStateEventSystemGraph<SPH,EPH,SFH,EFH>::createState(DagNode* stateDag) const
{
	return unique_ptr<PreState>(new State(Super::initial, stateDag));
}

template <typename SPH, typename EPH, typename SFH, typename EFH>
unique_ptr<PropSet>
FairStateEventSystemGraph<SPH,EPH,SFH,EFH>::updateTransitionLabel(RewriteTransitionState& r, PreTransition& t, PreState& s)
{
	unique_ptr<PropSet> truePropIds = Super::updateTransitionLabel(r, t, s);
	Super::spHandler.transferTruth(s, *truePropIds);						// transfer the cached state props
	unique_ptr<FairSet> fs(stateEventFC.computeAllFairness(*truePropIds));	// compute all SE fairness conditions
	sefHandler.updateLabel(*fs, static_cast<Transition&>(t));				// store the SE fairness conditions
	return truePropIds;
}

template <typename SPH, typename EPH, typename SFH, typename EFH>
unique_ptr<typename FairStateEventSystemGraph<SPH,EPH,SFH,EFH>::PreTransition>
FairStateEventSystemGraph<SPH,EPH,SFH,EFH>::createTransition(int nextState, int transitionIndex) const
{
	return unique_ptr<PreTransition>(new Transition(nextState, transitionIndex));
}


} /* namespace modelChecker */
