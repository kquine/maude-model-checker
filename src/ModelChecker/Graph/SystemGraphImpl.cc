/*
 * RewritePreGraph.cc
 *
 *  Created on: Jun 23, 2011
 *      Author: kquine
 */
//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "freeTheory.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

#include "SystemGraphImpl.hh"

namespace modelChecker {

template <typename TH,typename SPH,typename EPH>
SystemGraphImpl<TH,SPH,EPH>::SystemGraphImpl(RewritingContext* initial, TH* th, SPH* sph,  EPH* eph):
		initial(initial), th(th), sph(sph), eph(eph)
{
	initial->reduce();

	// simplified version of the insertState function
	DagNode* cannStateDag = getStateDag(insertDag(initial->root()));
	State* s = new State(initial, cannStateDag);
	this->sph->updateProp(s, cannStateDag);
	seen.append(s);

}

template <typename TH,typename SPH,typename EPH> inline int
SystemGraphImpl<TH,SPH,EPH>::getNrStates() const
{
	return seen.size();
}

template <typename TH,typename SPH,typename EPH> inline int
SystemGraphImpl<TH,SPH,EPH>::getNrTransitions(int stateNr) const
{
	Assert(seen[stateNr] != NULL, "StateTransitionGraph::getNrTransitions: Invalid state lookup");
	return seen[stateNr]->transitions.size();
}


template <typename TH,typename SPH,typename EPH> int
SystemGraphImpl<TH,SPH,EPH>::getNextState(int stateNr, int index)
{
	int nrTransitions = getNrTransitions(stateNr);
	State* n = seen[stateNr];
	if (index < nrTransitions)
		return n->transitions[index]->nextState;
	if (n->explore.get() == NULL)	// fully explored
		return NONE;
	while (nrTransitions <= index)
	{
		DagNode* ns = n->explore->getNextStateDag(initial);
		if (ns != NULL)		// if there is a next state
		{
			if (insertTransition(n, insertState(ns, n->explore.get())))		// new transition identified
				++ nrTransitions;
			MemoryCell::okToCollectGarbage();
		}
		else
		{
			n->explore.reset();	// remove active state
			return NONE;		// no more transition,, (fully explored transitions)
		}
    }
	return n->transitions[index]->nextState;
}

template <typename TH,typename SPH,typename EPH> inline DagNode*
SystemGraphImpl<TH,SPH,EPH>::getStateDag(int stateNr) const
{
	return StateDagContainer::getStateDag(stateNr);
}

template <typename TH,typename SPH,typename EPH> inline DagNode*
SystemGraphImpl<TH,SPH,EPH>::getTransitionDag(int stateNr, int index) const
{
	Assert(seen[stateNr] != NULL, "StateTransitionGraph::getTransitionDag: Invalid state lookup");
	return th->getDag(seen[stateNr]->transitions[index], getStateDag(stateNr));
}

template <typename TH,typename SPH,typename EPH> inline bool
SystemGraphImpl<TH,SPH,EPH>::satisfiesStateProp(int propId, int stateNr) const
{
	Assert(stateNr < getNrStates(), "StateTransitionGraph::satisfiesStateProp: Invalid state lookup");
	return sph->isSatisfied(seen[stateNr], propId);
}

template <typename TH,typename SPH,typename EPH> inline bool
SystemGraphImpl<TH,SPH,EPH>::satisfiesEventProp(int propId, int stateNr, int transitionNr) const
{
	Assert(stateNr < getNrStates() && transitionNr < getNrTransitions(stateNr),
			"StateTransitionGraph::satisfiesEventProp: Invalid transition lookup");
	return eph->isSatisfied(seen[stateNr]->transitions[transitionNr], propId);
}

template <typename TH,typename SPH,typename EPH> inline int
SystemGraphImpl<TH,SPH,EPH>::insertState(DagNode* stateDag, ActiveState* previous)
{
	int nextState = insertDag(stateDag);
	th->updateStateData(previous);
	if (nextState == seen.size())	// if a new state identified
	{
		DagNode* cannStateDag = getStateDag(nextState);
		State* s = new State(initial, cannStateDag);
		sph->updateProp(s, cannStateDag);	// including state props for (parameterized) fairness
		seen.append(s);
	}
	return nextState;
}

template <typename TH,typename SPH,typename EPH> inline bool
SystemGraphImpl<TH,SPH,EPH>::insertTransition(State* s, int nextState)
{
	RewriteTransitionState* rewriteState = s->explore.get();
	auto_ptr<Transition> t(new Transition(nextState));
	eph->updateProp(t.get(), rewriteState);	// including event props for (parameterized) fairness

	if (s->explore->transitionPtrSet->insert(t.get()).second)	// if a new transition identified
	{
		Transition* nt = t.release();	// NOTE: transfer ownership..
		s->transitions.append(nt);
		th->setTransitionData(nt, s->explore.get(), rewriteState);
		return true;
	}
	return false;
}

template <typename TH,typename SPH,typename EPH> inline
SystemGraphImpl<TH,SPH,EPH>::State::State(RewritingContext* parent, DagNode* stateDag):
	explore(new ActiveState(parent, stateDag)) {}

template <typename TH,typename SPH,typename EPH> inline bool
SystemGraphImpl<TH,SPH,EPH>::Transition::operator<(const Transition& t) const
{
	return nextState < t.nextState || EventPropSet::operator<(t);	// TH:TransitionData is not used for comparing..
}

template <typename TH,typename SPH,typename EPH> inline
SystemGraphImpl<TH,SPH,EPH>::ActiveState::ActiveState(RewritingContext* parent, DagNode* stateDag):
	RewriteTransitionState(parent, stateDag), transitionPtrSet(new TransitionPtrSet(trans_ptr_less)) {}

template <typename TH,typename SPH,typename EPH> inline bool
SystemGraphImpl<TH,SPH,EPH>::ActiveState::trans_ptr_less(const Transition* a, const Transition* b)
{
	return (*a) < (*b);
}

}
