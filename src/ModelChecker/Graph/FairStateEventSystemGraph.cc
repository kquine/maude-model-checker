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
#include "FairStateEventSystemGraph.hh"

namespace modelChecker {

template <typename PL, typename FL>
FairStateEventSystemGraph<PL,FL>::FairStateEventSystemGraph(unique_ptr<PL>&& sepl, unique_ptr<FL>&& sefl, RewritingContext& initial, const ProofTermGenerator& ptg):
	Super(move(sepl),initial,ptg), stateEventFairLabel(move(sefl)) {}

template <typename PL, typename FL> unique_ptr<FairSet>
FairStateEventSystemGraph<PL,FL>::makeFairSet(unsigned int stateNr, unsigned int transitionNr) const
{
	return stateEventFairLabel->makeFairSet(static_cast<State*>(this->seen[stateNr].get()), static_cast<Transition*>(this->seen[stateNr]->transitions[transitionNr].get()));
}

template <typename PL, typename FL> bool
FairStateEventSystemGraph<PL,FL>::Transition::operator<(const PreTransition& t) const
{
	return PreTransition::operator<(t) || FL::EventLabel::operator<(static_cast<const Transition&>(t));
}

template <typename PL, typename FL>
unique_ptr<PropSet>
FairStateEventSystemGraph<PL,FL>::updateStateLabel(DagNode* stateDag, PreState& s)
{
	unique_ptr<PropSet> truePropIds = Super::updateStateLabel(stateDag, s);
	stateEventFairLabel->updateStateLabel(*truePropIds, static_cast<State&>(s));	// compute and store all state fairness conditions
	return truePropIds;
}

template <typename PL, typename FL>
unique_ptr<typename FairStateEventSystemGraph<PL,FL>::PreState>
FairStateEventSystemGraph<PL,FL>::createState(DagNode* stateDag) const
{
	return unique_ptr<PreState>(new State(Super::initial, stateDag));
}

template <typename PL, typename FL>
unique_ptr<PropSet>
FairStateEventSystemGraph<PL,FL>::updateTransitionLabel(RewriteTransitionState& r, PreTransition& t, PreState& s)
{
	unique_ptr<PropSet> truePropIds = Super::updateTransitionLabel(r, t, s);
	this->stateEventPropLabel->transferStateLabel(s, *truePropIds);								// transfer the cached state props
	stateEventFairLabel->updateEventLabel(*truePropIds, static_cast<Transition&>(t));	// compute and store all SE fairness conditions
	return truePropIds;
}

template <typename PL, typename FL>
unique_ptr<typename FairStateEventSystemGraph<PL,FL>::PreTransition>
FairStateEventSystemGraph<PL,FL>::createTransition(unsigned int nextState, unsigned int transitionIndex) const
{
	return unique_ptr<PreTransition>(new Transition(nextState, transitionIndex));
}


} /* namespace modelChecker */
