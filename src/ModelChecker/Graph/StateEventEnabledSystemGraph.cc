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

template <typename SPH, typename EPH>
StateEventEnabledSystemGraph<SPH,EPH>::StateEventEnabledSystemGraph(RewritingContext& initial,
		PropChecker& spc, PropChecker& epc, ProofTermGenerator& ptg):
	Super(initial,ptg), statePC(spc), eventPC(epc) {}

template <typename SPH, typename EPH>
unique_ptr<typename StateEventEnabledSystemGraph<SPH,EPH>::LabelSet>
StateEventEnabledSystemGraph<SPH,EPH>::updateAllLabels(DagNode* stateDag, const vector<DagNode*>& proofDags, State& s, vector<unique_ptr<Transition> >& trs)
{
	unique_ptr<LabelSet> trueProps(new LabelSet);

	trueProps->stateProps = std::move(statePC.computeCheckResult(stateDag));	// compute all state props (for s)
	spHandler.updateLabel(*trueProps->stateProps, s); 							// store the formula state props

	trueProps->eventProps.resize(trs.size());

	for (unsigned int i = 0; i < trs.size(); ++i)
	{
		trueProps->eventProps[i] = std::move(eventPC.computeCheckResult(proofDags[i]));		// compute all event prop for each transition
		epHandler.updateLabel(*trueProps->eventProps[i], *trs[i]); 							// store the formula event props
	}
	return trueProps;
}

template <typename SPH, typename EPH>
unique_ptr<typename StateEventEnabledSystemGraph<SPH,EPH>::State>
StateEventEnabledSystemGraph<SPH,EPH>::createState() const
{
	return unique_ptr<State>(new State);
}

template <typename SPH, typename EPH>
unique_ptr<typename StateEventEnabledSystemGraph<SPH,EPH>::Transition>
StateEventEnabledSystemGraph<SPH,EPH>::createTransition(int nextState, int transitionIndex) const
{
	return unique_ptr<Transition>(new Transition(nextState,transitionIndex));
}

template <typename SPH, typename EPH> int
StateEventEnabledSystemGraph<SPH,EPH>::computeNextState(int stateNr, int index)
{
	if (! Super::seen[stateNr])	// if not explored yet
	{
		DagNode* stateDag = Super::getStateDag(stateNr);
		ActiveState as(Super::initial, stateDag);

		unique_ptr<State> s(createState());
		vector<unique_ptr<Transition> > trs;
		vector<DagNode*> proofDags;
		ProtectedDagNodeSet tdags;		// to protect proofDags from the garbage collection.

		while (DagNode* ns =  as.getNextStateDag(Super::initial))	// compute all transitions
		{
			int nextState = insertState(ns);
			trs.push_back(createTransition(nextState, ++ as.transitionCount));
			DagNode* pd = Super::ptGenerator.makeProofDag(as.getPosition(),*as.getRule(),as.getSubstitution());
			int pdIndex = tdags.dagNode2Index(pd);
			if (pdIndex == NONE)
			{
				pdIndex = tdags.cardinality();
				tdags.insert(pd);
			}
			proofDags.push_back(tdags.index2DagNode(pdIndex));
			MemoryCell::okToCollectGarbage();
		}
		updateAllLabels(stateDag, proofDags, *s, trs);

		for (unique_ptr<Transition>& tr : trs)
		{
			if (as.transitionPtrSet.insert(tr.get()).second)		// if a new transition identified
				s->transitions.push_back(std::move(tr));	// NOTE: transfer ownership..
		}
		Super::seen[stateNr] = std::move(s);
	}
	return index < Super::getNrTransitions(stateNr) ? Super::seen[stateNr]->transitions[index]->nextState : NONE;
}

template <typename SPH, typename EPH> inline int
StateEventEnabledSystemGraph<SPH,EPH>::insertState(DagNode* stateDag)
{
	int nextState = StateDagContainer::insertDag(stateDag);
	if (nextState == Super::getNrStates())	// if a new state identified
		Super::seen.push_back(nullptr);
	return nextState;
}

} /* namespace modelChecker */
