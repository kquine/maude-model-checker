/*
 * StateSystemGraph.hh
 *
 *  Created on: Sep 11, 2013
 *      Author: kquine
 */

#ifndef STATESYSTEMGRAPH_HH_
#define STATESYSTEMGRAPH_HH_
#include "BaseSystemGraphIter.hh"
#include "PropChecker/PropSet.hh"

namespace modelChecker {

//
// Only state propositions
//
template <typename PL>
class StateSystemGraph: public BaseSystemGraphIter<StateSystemGraph<PL>>, private SystemGraphTraits<StateSystemGraph<PL>>
{
	friend class BaseSystemGraph<StateSystemGraph>;
	friend class BaseSystemGraphIter<StateSystemGraph>;
	friend class SystemGraphTraits<StateSystemGraph<PL>>;
	using Super = BaseSystemGraphIter<StateSystemGraph<PL>>;

public:
	using typename SystemGraphTraits<StateSystemGraph<PL>>::State;
	using typename SystemGraphTraits<StateSystemGraph<PL>>::Transition;

	StateSystemGraph(unique_ptr<PL>&& spl, RewritingContext& initial, const ProofTermGenerator& ptg, const PropositionTable& propTable);
};

template <typename PL>
class SystemGraphTraits<StateSystemGraph<PL>>
{
public:
	struct State;
	struct Transition;
	struct ActiveState;

	SystemGraphTraits(unique_ptr<PL>&& pl): propLabel(move(pl)) {}

	template <typename STATE>
	bool insertTransition(unsigned int nextState, STATE& n) const;

	template <typename STATE>
	void insertDeadlockTransition(unsigned int stateNr, STATE& n) const;

	template <typename STATE>
	bool satisfiesStateProp(unsigned int propId, const STATE& s) const;

	template <typename TRANSITION>
	bool satisfiesEventProp(unsigned int propId, const TRANSITION& t) const;

	unique_ptr<PropSet> updateStateLabel(DagNode* stateDag, State& s) const;

protected:
	const unique_ptr<PL> propLabel;
};


template <typename PL>
struct SystemGraphTraits<StateSystemGraph<PL>>::State: public BaseSystemGraphIterTraits::State<ActiveState,Transition>, public PL::StateLabel {};

template <typename PL>
struct SystemGraphTraits<StateSystemGraph<PL>>::Transition: public BaseSystemGraphIterTraits::Transition, public RuleDagGenerator
{
	Transition(unsigned int nextState, const Rule* rule): BaseSystemGraphIterTraits::Transition{nextState}, RuleDagGenerator(rule) {}
};

template <typename PL>
struct SystemGraphTraits<StateSystemGraph<PL>>::ActiveState: public BaseSystemGraphIterTraits::ActiveState
{
	ActiveState(RewritingContext& parent, DagNode* stateDag): BaseSystemGraphIterTraits::ActiveState(parent,stateDag) {}
	set<unsigned int> nextStateSet;
};


} /* namespace modelChecker */

#include "StateSystemGraph.cc"	// to separate the implementation for the template

#endif /* STATESYSTEMGRAPH_HH_ */
