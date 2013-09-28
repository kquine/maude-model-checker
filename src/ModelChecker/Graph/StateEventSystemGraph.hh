/*
 * StateEventSystemGraph.hh
 *
 *  Created on: Aug 13, 2013
 *      Author: kquine
 */

#ifndef STATEEVENTSYSTEMGRAPH_HH_
#define STATEEVENTSYSTEMGRAPH_HH_
#include "BaseSystemGraphIter.hh"
#include "PropChecker/PropSet.hh"

namespace modelChecker {

/*
 * an ordinary state/event-based RW system graph; every state proposition should be defined by equations.
 */
template <typename PL>
class StateEventSystemGraph: public BaseSystemGraphIter<StateEventSystemGraph<PL>>, private SystemGraphTraits<StateEventSystemGraph<PL>>
{
	friend class BaseSystemGraph<StateEventSystemGraph>;
	friend class BaseSystemGraphIter<StateEventSystemGraph>;
	friend class SystemGraphTraits<StateEventSystemGraph<PL>>;
	using Super = BaseSystemGraphIter<StateEventSystemGraph<PL>>;

public:
	using typename SystemGraphTraits<StateEventSystemGraph<PL>>::State;
	using typename SystemGraphTraits<StateEventSystemGraph<PL>>::Transition;
	using typename SystemGraphTraits<StateEventSystemGraph<PL>>::ActiveState;

	StateEventSystemGraph(unique_ptr<PL>&& sepl, RewritingContext& initial, const ProofTermGenerator& ptg, const PropositionTable& propTable);

private:
	const unique_ptr<PL> propLabel;
};

template <typename PL>
class SystemGraphTraits<StateEventSystemGraph<PL>>
{
public:
	using State = typename SystemGraphTraits<StateSystemGraph<PL>>::State;
	struct Transition;
	struct ActiveState;

	bool insertTransition(unsigned int nextState, State& n);
	bool satisfiesStateProp(unsigned int propId, const State& s) const;
	bool satisfiesEventProp(unsigned int propId, const Transition& t) const;
	unique_ptr<PropSet> updateStateLabel(DagNode* stateDag, State& s) const;

private:
	using PreTransition = 	BaseSystemGraphIterTraits::Transition;
	using PreActiveState =	BaseSystemGraphIterTraits::ActiveState;
};

template <typename PL>
struct SystemGraphTraits<StateEventSystemGraph<PL>>::Transition: public PreTransition, public ProofDagGenerator, public PL::EventLabel
{
	Transition(unsigned int nextState, unsigned int transitionIndex): PreTransition{nextState}, ProofDagGenerator(transitionIndex) {}
	bool operator<(const Transition& t) const;
};

template <typename PL>
struct SystemGraphTraits<StateEventSystemGraph<PL>>::ActiveState: public PreActiveState
{
	ActiveState(RewritingContext& parent, DagNode* stateDag): PreActiveState(parent,stateDag) {}

	ptr_set<Transition> transitionPtrSet;	// to distinguish equivalent transitions
	unsigned int transitionCount = 0;		// to recover the corresponding proofterm
};



} /* namespace modelChecker */

#include "StateEventSystemGraph.cc"	// to separate the implementation for the template

#endif /* STATEEVENTSYSTEMGRAPH_HH_ */
