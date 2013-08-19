/*
 * StateEventEnabledSystemGraph.hh
 *
 *  Created on: Aug 14, 2013
 *      Author: kquine
 */

#ifndef STATEEVENTENABLEDSYSTEMGRAPH_HH_
#define STATEEVENTENABLEDSYSTEMGRAPH_HH_
#include "BaseSystemGraph.hh"

namespace modelChecker {

template <typename StatePropHandler, typename EventPropHandler>
class StateEventEnabledSystemGraph: public BaseSystemGraph<StateEventEnabledSystemGraph<StatePropHandler,EventPropHandler> >
{
	friend class BaseSystemGraph<StateEventEnabledSystemGraph<StatePropHandler,EventPropHandler> >;
	typedef BaseSystemGraph<StateEventEnabledSystemGraph<StatePropHandler,EventPropHandler> >		Super;
	typedef BaseSystemGraphTraits<StateEventEnabledSystemGraph<StatePropHandler,EventPropHandler> >	StateEventSystemGraphTraits;
	typedef typename StateEventSystemGraphTraits::StateLabel										StateLabel;
	typedef typename StateEventSystemGraphTraits::EventLabel										EventLabel;
	typedef typename StateEventSystemGraphTraits::State												State;
	typedef typename StateEventSystemGraphTraits::ActiveState										ActiveState;
	typedef typename StateEventSystemGraphTraits::Transition										Transition;

public:
	StateEventEnabledSystemGraph(RewritingContext* initial, const StatePropHandler& sph, const EventPropHandler& eph, ProofTermGenerator& ptg);

private:
	DagNode* makeTransitionDag(int stateNr, int transitionNr) const;
	int computeNextState(int stateNr, int index);
	int insertState(DagNode* stateDag);

	const StatePropHandler& spHandler;
	const StatePropHandler& epHandler;
};

template <typename SPH, typename EPH>
struct BaseSystemGraphTraits<StateEventEnabledSystemGraph<SPH,EPH> >: public BaseSystemGraphTraits<StateEventSystemGraph<SPH,EPH> >
{
	typedef typename SPH::Label		StateLabel;
	typedef typename EPH::Label		EventLabel;

	typedef BaseSystemGraphTraits<StateEventSystemGraph<SPH,EPH> >		StateEventSystemGraphTraits;
	typedef typename StateEventSystemGraphTraits::ActiveState  			ActiveState;
	typedef typename StateEventSystemGraphTraits::Transition  			Transition;
	typedef typename StateEventSystemGraphTraits::trans_ptr_compare		trans_ptr_compare;

	struct State: public StateLabel
	{
		State(): explored(false) {}
	    bool explored;
		PtrVector<Transition> transitions;
	};
};


} /* namespace modelChecker */

#include "StateEventEnabledSystemGraph.cc"	// to separate the implementation for the template

#endif /* STATEEVENTENABLEDSYSTEMGRAPH_HH_ */
