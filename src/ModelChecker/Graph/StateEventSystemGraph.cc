/*
 * StateEventSystemGraph.cc
 *
 *  Created on: Aug 13, 2013
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
#include "StateEventSystemGraph.hh"

namespace modelChecker {


template <typename PL>
StateEventSystemGraph<PL>::StateEventSystemGraph(unique_ptr<PL>&& sepl, RewritingContext& initial, const ProofTermGenerator& ptg):
	Super(initial,ptg), propLabel(move(sepl)) {}

template <typename PL> bool
StateEventSystemGraph<PL>::satisfiesStateFormula(Bdd formula, unsigned int stateNr) const
{
	return BddUtil::satisfiesFormula(formula, [&] (unsigned int propId) { return propLabel->satisfiesStateProp(propId, *this->seen[stateNr]); });
}

template <typename PL> pair<bool,Bdd>
StateEventSystemGraph<PL>::satisfiesPartialStateFormula(Bdd formula, unsigned int stateNr) const
{
	auto domain = [&] (unsigned int propId) { return ! propLabel->isEvent(propId); };
	auto check = [&] (unsigned int propId) { return propLabel->satisfiesStateProp(propId, *this->seen[stateNr]); };
	return BddUtil::satisfiesPartialFormula(formula, domain, check);
}

template <typename PL> bool
StateEventSystemGraph<PL>::satisfiesStateEventFormula(Bdd formula, unsigned int stateNr, unsigned int transitionNr) const
{
	auto check = [&] (unsigned int propId) { return propLabel->isEvent(propId) ?
			  propLabel->satisfiesEventProp(propId,*this->seen[stateNr]->transitions[transitionNr])
			: propLabel->satisfiesStateProp(propId,*this->seen[stateNr]); };
	return BddUtil::satisfiesFormula(formula, check);
}


template <typename PL>
unique_ptr<PropSet>
StateEventSystemGraph<PL>::updateStateLabel(DagNode* stateDag, State& s)
{
	return propLabel->updateStateLabel(stateDag, s);	// compute all state props and store the formula state props (+ extra for SE fairness)
}

template <typename PL>
unique_ptr<typename StateEventSystemGraph<PL>::State>
StateEventSystemGraph<PL>::createState(DagNode* stateDag) const
{
	return unique_ptr<State>(new State(this->initial, stateDag));
}

template <typename PL>
unique_ptr<PropSet>
StateEventSystemGraph<PL>::updateTransitionLabel(RewriteTransitionState& r, Transition& t, State&)
{
	DagNode* eventDag = this->ptGenerator.makeProofDag(r.getPosition(), *r.getRule(), r.getSubstitution());
	return propLabel->updateEventLabel(eventDag, t);	// compute all event props and store the formula event props
}

template <typename PL>
unique_ptr<typename StateEventSystemGraph<PL>::Transition>
StateEventSystemGraph<PL>::createTransition(unsigned int nextState, unsigned int transitionIndex) const
{
	return unique_ptr<Transition>(new Transition(nextState, transitionIndex));
}

template <typename PL> inline bool
StateEventSystemGraph<PL>::insertTransition(unsigned int nextState, State& n)
{
	ActiveState& as = *n.explore;
	unique_ptr<Transition> t(createTransition(nextState, ++ as.transitionCount));
	updateTransitionLabel(as, *t, n);

	if (as.transitionPtrSet.insert(t.get()).second)	// if a new transition identified
	{
		n.transitions.push_back(std::move(t));
		return true;
	}
	return false;
}

template <typename PL> bool
BaseSystemGraphTraits<StateEventSystemGraph<PL> >::Transition::operator<(const Transition& t) const
{
	return nextState < t.nextState;
}

template <typename PL> DagNode*
BaseSystemGraphTraits<StateEventSystemGraph<PL> >::Transition::makeDag(RewritingContext& parent, DagNode* stateDag, const ProofTermGenerator& ptg) const
{
	unique_ptr<RewriteTransitionState>  tr(new RewriteTransitionState(parent, stateDag));
	for (unsigned int i = 0; i < transitionIndex; ++i)
		tr->getNextStateDag(parent);
	return ptg.makeProofDag(tr->getPosition(), *tr->getRule(), tr->getSubstitution());
}

template <typename PL> bool
BaseSystemGraphTraits<StateEventSystemGraph<PL> >::trans_ptr_compare::operator()(const Transition* a, const Transition* b) const
{
	return (*a) < (*b);
}


} /* namespace modelChecker */
