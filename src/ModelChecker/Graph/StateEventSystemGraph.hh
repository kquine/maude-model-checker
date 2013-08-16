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
template <typename SELabelHandler>
class StateEventSystemGraph: public BaseSystemGraph<SELabelHandler,StateEventSystemGraph<SELabelHandler> >
{
	friend class BaseSystemGraph<SELabelHandler,StateEventSystemGraph<SELabelHandler> >;
	typedef BaseSystemGraph<SELabelHandler,StateEventSystemGraph<SELabelHandler> >				BaseSystemGraph;
	typedef typename BaseSystemGraphTraits<StateEventSystemGraph<SELabelHandler> >::StateLabel	StateLabel;
	typedef typename BaseSystemGraphTraits<StateEventSystemGraph<SELabelHandler> >::EventLabel	EventLabel;
	typedef typename BaseSystemGraphTraits<StateEventSystemGraph<SELabelHandler> >::State		State;
	typedef typename BaseSystemGraphTraits<StateEventSystemGraph<SELabelHandler> >::ActiveState	ActiveState;
	typedef typename BaseSystemGraphTraits<StateEventSystemGraph<SELabelHandler> >::Transition	Transition;

public:
	StateEventSystemGraph(RewritingContext* initial, SELabelHandler& selh, ProofTermGenerator& ptg);

private:
	bool insertTransition(int nextState, State* n);
};


} /* namespace modelChecker */

#include "StateEventSystemGraph.cc"	// to separate the implementation for the template

#endif /* STATEEVENTSYSTEMGRAPH_HH_ */
