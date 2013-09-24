/*
 * StateEventEnabledSystemGraph.cc
 *
 *  Created on: Aug 14, 2013
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
#include "protectedDagNodeSet.hh"

// core class definitions
#include "rewritingContext.hh"

// ltlr definitions
#include "StateEventEnabledSystemGraph.hh"

namespace modelChecker {

template <typename PL>
StateEventEnabledSystemGraph<PL>::StateEventEnabledSystemGraph(unique_ptr<PL>&& sepl, unique_ptr<EnabledPropTransferer>&& enpc,
		RewritingContext& initial, const ProofTermGenerator& ptg):
	Super(initial,ptg), enabledHandler(move(enpc)), propLabel(move(sepl)) {}

template <typename PL> bool
StateEventEnabledSystemGraph<PL>::satisfiesStateFormula(Bdd formula, unsigned int stateNr) const
{
	auto check = [&] (unsigned int propId) { return enabledHandler->satisfiesStateProp(propId, *this->seen[stateNr], this->seen[stateNr]->transitions, *propLabel); };
	return BddUtil::satisfiesFormula(formula, check);
}

template <typename PL> pair<bool,Bdd>
StateEventEnabledSystemGraph<PL>::satisfiesPartialStateFormula(Bdd formula, unsigned int stateNr) const
{
	auto domain = [&] (unsigned int propId) { return !propLabel->isEvent(propId); };
	auto check = [&] (unsigned int propId) { return enabledHandler->satisfiesStateProp(propId, *this->seen[stateNr], this->seen[stateNr]->transitions, *propLabel); };
	return BddUtil::satisfiesPartialFormula(formula, domain, check);
}

template <typename PL> bool
StateEventEnabledSystemGraph<PL>::satisfiesStateEventFormula(Bdd formula, unsigned int stateNr, unsigned int transitionNr) const
{
	auto check = [&] (unsigned int propId) { return propLabel->isEvent(propId) ?
			  propLabel->satisfiesEventProp(propId,*this->seen[stateNr]->transitions[transitionNr])
			: enabledHandler->satisfiesStateProp(propId, *this->seen[stateNr], this->seen[stateNr]->transitions, *propLabel); };
	return BddUtil::satisfiesFormula(formula, check);
}

template <typename PL>
typename StateEventEnabledSystemGraph<PL>::LabelSet
StateEventEnabledSystemGraph<PL>::updateAllLabels(DagNode* stateDag, const vector<DagNode*>& proofDags, State& s, vector<unique_ptr<Transition> >& trs)
{
	LabelSet trueProps;
	trueProps.second.resize(trs.size());

	trueProps.first = propLabel->updateStateLabel(stateDag,s);	// compute and store all state props (for s)

	for (unsigned int i = 0; i < trs.size(); ++i)
		trueProps.second[i] = propLabel->updateEventLabel(proofDags[i],*trs[i]);	// compute and store all event prop (for transitions)
	return trueProps;
}

template <typename PL>
unique_ptr<typename StateEventEnabledSystemGraph<PL>::State>
StateEventEnabledSystemGraph<PL>::createState() const
{
	return unique_ptr<State>(new State);
}

template <typename PL>
unique_ptr<typename StateEventEnabledSystemGraph<PL>::Transition>
StateEventEnabledSystemGraph<PL>::createTransition(unsigned int nextState, unsigned int transitionIndex) const
{
	return unique_ptr<Transition>(new Transition(nextState,transitionIndex));
}

} /* namespace modelChecker */
