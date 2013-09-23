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
	Super(initial,ptg), enabledPropHandler(move(enpc)), stateEventPropLabel(move(sepl)) {}

template <typename PL> bool
StateEventEnabledSystemGraph<PL>::satisfiesStateFormula(Bdd formula, unsigned int stateNr) const
{
	auto check = [&] (unsigned int propId) { return enabledPropHandler->satisfiesStateProp(propId, *this->seen[stateNr], this->seen[stateNr]->transitions, *stateEventPropLabel); };
	return BddUtil::satisfiesFormula(formula, check);
}

template <typename PL> pair<bool,Bdd>
StateEventEnabledSystemGraph<PL>::satisfiesPartialStateFormula(Bdd formula, unsigned int stateNr) const
{
	auto domain = [&] (unsigned int propId) { return !stateEventPropLabel->isEvent(propId); };
	auto check = [&] (unsigned int propId) { return enabledPropHandler->satisfiesStateProp(propId, *this->seen[stateNr], this->seen[stateNr]->transitions, *stateEventPropLabel); };
	return BddUtil::satisfiesPartialFormula(formula, domain, check);
}

template <typename PL> bool
StateEventEnabledSystemGraph<PL>::satisfiesStateEventFormula(Bdd formula, unsigned int stateNr, unsigned int transitionNr) const
{
	auto check = [&] (unsigned int propId) {
		return stateEventPropLabel->isEvent(propId) ?
				  stateEventPropLabel->satisfiesEventProp(propId,*this->seen[stateNr]->transitions[transitionNr])
				: enabledPropHandler->satisfiesStateProp(propId, *this->seen[stateNr], this->seen[stateNr]->transitions, *stateEventPropLabel);
	};
	return BddUtil::satisfiesFormula(formula, check);
}

template <typename PL>
typename StateEventEnabledSystemGraph<PL>::LabelSet
StateEventEnabledSystemGraph<PL>::updateAllLabels(DagNode* stateDag, const vector<DagNode*>& proofDags, State& s, vector<unique_ptr<Transition> >& trs)
{
	LabelSet trueProps;
	trueProps.second.resize(trs.size());

	trueProps.first = stateEventPropLabel->updateStateLabel(stateDag,s);	// compute and store all state props (for s)

	for (unsigned int i = 0; i < trs.size(); ++i)
		trueProps.second[i] = stateEventPropLabel->updateEventLabel(proofDags[i],*trs[i]);	// compute and store all event prop (for transitions)
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

template <typename PL> int
StateEventEnabledSystemGraph<PL>::computeNextState(unsigned int stateNr, unsigned int index)
{
	if (! this->seen[stateNr])	// if not explored yet
	{
		DagNode* stateDag = this->getStateDag(stateNr);
		ActiveState as(this->initial, stateDag);

		unique_ptr<State> s = createState();
		vector<unique_ptr<Transition>> trs;
		vector<DagNode*> proofDags;
		ProtectedDagNodeSet tdags;		// to protect proofDags from the garbage collection.

		while (DagNode* ns =  as.getNextStateDag(this->initial))	// compute all transitions
		{
			auto nextState = insertState(ns);
			trs.push_back(createTransition(nextState, ++ as.transitionCount));
			DagNode* pd = this->ptGenerator.makeProofDag(as.getPosition(),*as.getRule(),as.getSubstitution());

			auto pdIndex = tdags.dagNode2Index(pd);
			if (pdIndex == NONE)
			{
				pdIndex = tdags.cardinality();
				tdags.insert(pd);
			}
			proofDags.push_back(tdags.index2DagNode(pdIndex));
			MemoryCell::okToCollectGarbage();
		}
		updateAllLabels(stateDag, proofDags, *s, trs);

		for (auto& tr : trs)
		{
			if (as.transitionPtrSet.insert(tr.get()).second)	// if a new transition identified
				s->transitions.push_back(move(tr));				// NOTE: transfer ownership..
		}
		this->seen[stateNr] = move(s);
	}

	if (index < this->getNrTransitions(stateNr))
		return this->seen[stateNr]->transitions[index]->nextState;
	else
		return NONE;
}

template <typename PL> inline unsigned int
StateEventEnabledSystemGraph<PL>::insertState(DagNode* stateDag)
{
	auto nextState = StateDagContainer::insertDag(stateDag);
	if (nextState == this->getNrStates())	// if a new state identified
		this->seen.push_back(nullptr);
	return nextState;
}

} /* namespace modelChecker */
