/*
 * StateEventSystemGraph.hh
 *
 *  Created on: Aug 13, 2013
 *      Author: kquine
 */

#ifndef STATEEVENTSYSTEMGRAPH_HH_
#define STATEEVENTSYSTEMGRAPH_HH_
#include "BaseSystemGraph.hh"

namespace modelChecker {

/*
 * an ordinary state/event-based RW system graph.
 * every state proposition should be defined by equations (no special treatment for enabled props here)
 */
template <typename StatePropHandler, typename EventPropHandler>
class StateEventSystemGraph: public BaseSystemGraph<StateEventSystemGraph<StatePropHandler,EventPropHandler> >
{
	friend class BaseSystemGraph<StateEventSystemGraph<StatePropHandler,EventPropHandler> >;
	typedef BaseSystemGraph<StateEventSystemGraph<StatePropHandler,EventPropHandler> >			Super;
	typedef BaseSystemGraphTraits<StateEventSystemGraph<StatePropHandler,EventPropHandler> >	StateEventSystemGraphTraits;
	typedef typename StateEventSystemGraphTraits::StateLabel									StateLabel;
	typedef typename StateEventSystemGraphTraits::EventLabel									EventLabel;
	typedef typename StateEventSystemGraphTraits::State											State;
	typedef typename StateEventSystemGraphTraits::ActiveState									ActiveState;
	typedef typename StateEventSystemGraphTraits::Transition									Transition;

public:
	StateEventSystemGraph(RewritingContext* initial, const StatePropHandler& sph, const EventPropHandler& eph, ProofTermGenerator& ptg);

private:
	DagNode* makeTransitionDag(int stateNr, int transitionNr) const;
	int computeNextState(int stateNr, int index);
	int insertState(DagNode* stateDag);
	bool insertTransition(int nextState, State* n);

	const StatePropHandler& spHandler;
	const StatePropHandler& epHandler;
};

template <typename StatePropHandler, typename EventPropHandler>
struct BaseSystemGraphTraits<StateEventSystemGraph<StatePropHandler,EventPropHandler> >
{
	typedef typename StatePropHandler::Label		StateLabel;
	typedef typename EventPropHandler::Label		EventLabel;

	struct Transition: public EventLabel
	{
		Transition(int nextState, int transitionIndex): nextState(nextState), transitionIndex(transitionIndex) {}
		bool operator<(const Transition& t) const { return nextState < t.nextState || EventLabel::operator<(t); }
		int nextState;
		int transitionIndex;	// to construct a proofterm
	};

	struct trans_ptr_compare
	{
		bool operator() (const Transition* a, const Transition* b) const { return (*a) < (*b); }
	};

	struct ActiveState: public RewriteTransitionState
	{
		ActiveState(RewritingContext* parent, DagNode* stateDag): RewriteTransitionState(parent,stateDag), transitionCount(-1) {}
		set<Transition*, trans_ptr_compare> transitionPtrSet;	// to distinguish equivalent transitions
		int transitionCount;									// to recover the corresponding proofterm
	};

	struct State: public StateLabel
	{
		State(RewritingContext* parent, DagNode* stateDag): explore(new ActiveState(parent,stateDag)) {}
		PtrVector<Transition> transitions;
		auto_ptr<ActiveState> explore;
	};
};


} /* namespace modelChecker */

#include "StateEventSystemGraph.cc"	// to separate the implementation for the template

#endif /* STATEEVENTSYSTEMGRAPH_HH_ */
