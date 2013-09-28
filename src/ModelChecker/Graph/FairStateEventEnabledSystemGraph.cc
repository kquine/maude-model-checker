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
#include "FairStateEventEnabledSystemGraph.hh"

namespace modelChecker {


template <typename PL, typename FL>
FairStateEventEnabledSystemGraph<PL,FL>::FairStateEventEnabledSystemGraph(unique_ptr<PL>&& pl, unique_ptr<FL>&& fl, unique_ptr<EnabledPropHandler>&& enpc,
		RewritingContext& initial, const ProofTermGenerator& ptg, const PropositionTable& propTable):
			Super(initial,ptg,propTable), propLabel(move(pl)), fairLabel(move(fl)), enabledHandler(move(enpc)) {}

template <typename PL, typename FL> unique_ptr<FairSet>
FairStateEventEnabledSystemGraph<PL,FL>::makeFairSet(unsigned int stateNr, unsigned int transitionNr) const
{
	return this->fairLabel->makeFairSet(static_cast<State*>(this->seen[stateNr].get()), static_cast<Transition*>(this->seen[stateNr]->transitions[transitionNr].get()));
}

template <typename PL, typename FL> bool
SystemGraphTraits<FairStateEventEnabledSystemGraph<PL,FL>>::satisfiesStateProp(unsigned int propId, const State& s) const
{
	const auto self = static_cast<const FairStateEventEnabledSystemGraph<PL,FL>*>(this);
	if (self->propTable.isEnabledProp(propId))
	{
		auto evtId = self->propTable.getEnabledEventId(propId);
		for (auto& j : s.transitions)
			if (self->propLabel->satisfiesEventProp(evtId, static_cast<Transition&>(*j))) return true;
		return false;
	}
	return self->propLabel->satisfiesStateProp(propId, s);
}

template <typename PL, typename FL> bool
SystemGraphTraits<FairStateEventEnabledSystemGraph<PL,FL>>::satisfiesEventProp(unsigned int propId, const Transition& t) const
{
	return static_cast<const FairStateEventEnabledSystemGraph<PL,FL>*>(this)->propLabel->satisfiesEventProp(propId, t);
}

template <typename PL, typename FL> void
SystemGraphTraits<FairStateEventEnabledSystemGraph<PL,FL>>::updateAllLabels(DagNode* stateDag, const vector<DagNode*>& proofDags, State& s, vector<unique_ptr<Transition>>& trs)
{
	const auto self = static_cast<const FairStateEventEnabledSystemGraph<PL,FL>*>(this);

	unique_ptr<PropSet> trueStateProps = self->propLabel->updateStateLabel(stateDag,s);		// compute and store all state props (for s)
	vector<unique_ptr<PropSet>> trueEventProps(trs.size());
	for (unsigned int i = 0; i < trs.size(); ++i)
		trueEventProps[i] = self->propLabel->updateEventLabel(proofDags[i],*trs[i]);	// compute and store all event prop (for transitions)

	PropSet::merge(trueStateProps, self->enabledHandler->computeEnabledProps(trueEventProps));	// compute all enabled props for (state and/or event) fairness conditions

	if (trueStateProps)
		self->fairLabel->updateStateLabel(*trueStateProps,s);	// compute all state fairness conditions

	for (unsigned int i = 0; i < trs.size(); ++i)
	{
		PropSet::merge(trueEventProps[i], trueStateProps);					// transfer truth
		if (trueEventProps[i])
			self->fairLabel->updateEventLabel(*trueEventProps[i],*trs[i]); 	// compute and store all SE fairness conditions
	}
}


template <typename PL, typename FL> bool
SystemGraphTraits<FairStateEventEnabledSystemGraph<PL,FL>>::Transition::operator<(const Transition& t) const
{
	return PreTransition::operator<(t) || FL::EventLabel::operator<(t);
}

} /* namespace modelChecker */
