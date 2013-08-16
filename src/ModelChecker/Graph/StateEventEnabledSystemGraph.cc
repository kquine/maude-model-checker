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

template <typename SEELabelHandler>
struct BaseSystemGraphTraits<StateEventEnabledSystemGraph<SEELabelHandler> >: public BaseSystemGraphTraits<StateEventSystemGraph<SEELabelHandler> >
{
	typedef typename SEELabelHandler::StateLabel		StateLabel;
	typedef typename SEELabelHandler::EventLabel		EventLabel;

	typedef BaseSystemGraphTraits<StateEventSystemGraph<SEELabelHandler> >		StateEventSystemGraphTraits;
	typedef typename StateEventSystemGraphTraits::Transition  					Transition;
	typedef typename StateEventSystemGraphTraits::trans_ptr_compare				trans_ptr_compare;

	struct State: public StateLabel
	{
		PtrVector<Transition> transitions;
	};
};


template <typename SEELH>
StateEventEnabledSystemGraph<SEELH>::StateEventEnabledSystemGraph(RewritingContext* initial, SEELH& seelh, ProofTermGenerator& ptg):
	BaseSystemGraph(initial, seelh, ptg) {}

template <typename SEELH> int
StateEventEnabledSystemGraph<SEELH>::insertState(DagNode* stateDag)
{
	int nextState = StateDagContainer::insertDag(stateDag);
	if (nextState == BaseSystemGraph::seen.size())	// if a new state identified, add an "unexplored" state
		BaseSystemGraph::seen.append(NULL);
	return nextState;
}

template <typename SEELH> int
StateEventEnabledSystemGraph<SEELH>::computeNextState(int stateNr, int index)
{
	if (BaseSystemGraph::seen[stateNr] == NULL)	// if not explored yet
	{
		DagNode* stateDag = BaseSystemGraph::getStateDag(stateNr);
		RewriteTransitionState rts(BaseSystemGraph::initial, stateDag);

		PtrVector<Transition> transitions;
		ProtectedDagNodeSet tdags;		// to protect proofDags from the garbage collection.
		Vector<DagNode*> proofDags;

		// first compute each transition
		for( struct {DagNode* ns; int tc;} i = {NULL,0}; (i.ns = rts.getNextStateDag(BaseSystemGraph::initial)); ++ i.tc)
		{
			int nextState = insertState(i.ns);
			transitions.append(new Transition(nextState, i.tc));
			proofDags.append(computeTransitionDag(rts, tdags));
			MemoryCell::okToCollectGarbage();
		}
		State* s = createState(proofDags, transitions, stateDag);
		BaseSystemGraph::seen.replace(stateNr, s);
	}
	return index < BaseSystemGraph::getNrTransitions(stateNr) ? BaseSystemGraph::seen[stateNr]->transitions[index]->nextState : NONE;
}

template <typename SEELH> DagNode*
StateEventEnabledSystemGraph<SEELH>::computeTransitionDag(const RewriteTransitionState& rts, ProtectedDagNodeSet& tdags) const
{
	DagNode* pd = BaseSystemGraph::ptGenerator.makeProofDag(rts.getPosition(),*rts.getRule(),rts.getSubstitution());
	int tindex = tdags.dagNode2Index(pd);
	if (tindex == NONE)
	{
		tindex = tdags.cardinality();
		tdags.insert(pd);
	}
	return tdags.index2DagNode(tindex);
}

template<typename SEELH> typename StateEventEnabledSystemGraph<SEELH>::State*
StateEventEnabledSystemGraph<SEELH>::createState(const Vector<DagNode*>& proofDags, PtrVector<Transition>& transitions, DagNode* stateDag) const
{
	typedef typename BaseSystemGraphTraits<StateEventEnabledSystemGraph<SEELH> >::trans_ptr_compare trans_ptr_compare;
	set<Transition*, trans_ptr_compare> transitionPtrSet;

	State* s = new State;
	BaseSystemGraph::lHandler.updateStateEventLabel(stateDag, s, proofDags, transitions);	// compute the state/event labels at once

	for (unsigned int i = 0; i < transitions.size(); ++i)
	{
		if (transitionPtrSet.insert(transitions[i]).second)		// if a new transition identified
			s->transitions.append(transitions.move(i));	// NOTE: transfer ownership..
	}
	return s;
}

} /* namespace modelChecker */
