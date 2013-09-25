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
FairStateEventEnabledSystemGraph<PL,FL>::FairStateEventEnabledSystemGraph(unique_ptr<PL>&& sepl, unique_ptr<FL>&& sefl, unique_ptr<EnabledPropHandler>&& enpc,
		RewritingContext& initial, const ProofTermGenerator& ptg, const PropositionTable& propTable):
	Super(move(sepl),move(enpc),initial,ptg,propTable), fairLabel(move(sefl)) {}

template <typename PL, typename FL> unique_ptr<FairSet>
FairStateEventEnabledSystemGraph<PL,FL>::makeFairSet(unsigned int stateNr, unsigned int transitionNr) const
{
	return fairLabel->makeFairSet(static_cast<State*>(this->seen[stateNr].get()), static_cast<Transition*>(this->seen[stateNr]->transitions[transitionNr].get()));
}

template <typename PL, typename FL> bool
FairStateEventEnabledSystemGraph<PL,FL>::Transition::operator<(const PreTransition& t) const
{
	return PreTransition::operator<(t) || FL::EventLabel::operator<(static_cast<const Transition&>(t));
}

template <typename PL, typename FL>
unique_ptr<typename FairStateEventEnabledSystemGraph<PL,FL>::PreState>
FairStateEventEnabledSystemGraph<PL,FL>::createState() const
{
	return unique_ptr<PreState>(new State);
}

template <typename PL, typename FL>
unique_ptr<typename FairStateEventEnabledSystemGraph<PL,FL>::PreTransition>
FairStateEventEnabledSystemGraph<PL,FL>::createTransition(unsigned int nextState, unsigned int transitionIndex) const
{
	return unique_ptr<PreTransition>(new Transition(nextState, transitionIndex));
}

template <typename PL, typename FL>
typename FairStateEventEnabledSystemGraph<PL,FL>::LabelSet
FairStateEventEnabledSystemGraph<PL,FL>::updateAllLabels(DagNode* stateDag, const vector<DagNode*>& proofDags, PreState& s, vector<unique_ptr<PreTransition> >& trs)
{
	LabelSet trueProps = Super::updateAllLabels(stateDag, proofDags, s, trs);
	PropSet::merge(trueProps.first, this->enabledHandler->computeEnabledProps(trueProps.second));	// compute all enabled props for (state and/or event) fairness conditions

	if (trueProps.first)
		fairLabel->updateStateLabel(*trueProps.first,static_cast<State&>(s));	// compute all state fairness conditions

	for (unsigned int i = 0; i < trs.size(); ++i)
	{
		PropSet::merge(trueProps.second[i], trueProps.first);					// transfer truth
		if (trueProps.second[i])
			fairLabel->updateEventLabel(*trueProps.second[i],static_cast<Transition&>(*trs[i])); 	// compute and store all SE fairness conditions
	}
	return trueProps;
}


} /* namespace modelChecker */
