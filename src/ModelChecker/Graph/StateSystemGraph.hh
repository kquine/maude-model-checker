/*
 * StateSystemGraph.hh
 *
 *  Created on: Aug 12, 2013
 *      Author: kquine
 */

#ifndef STATESYSTEMGRAPH_HH_
#define STATESYSTEMGRAPH_HH_
#include "BaseSystemGraph.hh"

namespace modelChecker {

template <typename StateLabelHandler>
class StateSystemGraph: public BaseSystemGraph<StateLabelHandler,StateSystemGraph<StateLabelHandler> >
{
	friend class BaseSystemGraph<StateLabelHandler,StateSystemGraph<StateLabelHandler> >;
	typedef BaseSystemGraph<StateLabelHandler,StateSystemGraph<StateLabelHandler> >				BaseSystemGraph;
	typedef typename BaseSystemGraphTraits<StateSystemGraph<StateLabelHandler> >::StateLabel	StateLabel;
	typedef typename BaseSystemGraphTraits<StateSystemGraph<StateLabelHandler> >::State			State;
	typedef typename BaseSystemGraphTraits<StateSystemGraph<StateLabelHandler> >::Transition	Transition;
public:
	StateSystemGraph(RewritingContext* initial, StateLabelHandler& slh, ProofTermGenerator& ptg);

private:
	bool insertTransition(int nextState, State* n);
};

} /* namespace modelChecker */

#include "StateSystemGraph.cc"	// to separate the implementation for the template

#endif /* STATESYSTEMGRAPH_HH_ */
