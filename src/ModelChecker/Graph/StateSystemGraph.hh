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

private:
	const unique_ptr<PL> propLabel;
};

template <typename PL>
class SystemGraphTraits<StateSystemGraph<PL>>
{
public:
	struct State;
	struct Transition;
	struct ActiveState;

	bool insertTransition(unsigned int nextState, State& n) const;
	bool satisfiesStateProp(unsigned int propId, const State& s) const;
	bool satisfiesEventProp(unsigned int propId, const Transition& t) const;
	unique_ptr<PropSet> updateStateLabel(DagNode* stateDag, State& s) const;

private:
	using PreState = 		BaseSystemGraphIterTraits::State;
	using PreTransition = 	BaseSystemGraphIterTraits::Transition;
	using PreActiveState =	BaseSystemGraphIterTraits::ActiveState;
};


template <typename PL>
struct SystemGraphTraits<StateSystemGraph<PL>>::State: public PreState, public PL::StateLabel {};

template <typename PL>
struct SystemGraphTraits<StateSystemGraph<PL>>::Transition: public PreTransition, public RuleDagGenerator
{
	Transition(unsigned int nextState, const Rule* rule): PreTransition{nextState}, RuleDagGenerator(rule) {}
};

template <typename PL>
struct SystemGraphTraits<StateSystemGraph<PL>>::ActiveState: public PreActiveState
{
	ActiveState(RewritingContext& parent, DagNode* stateDag): PreActiveState(parent,stateDag) {}
	set<unsigned int> nextStateSet;
};


} /* namespace modelChecker */

#include "StateSystemGraph.cc"	// to separate the implementation for the template

#endif /* STATESYSTEMGRAPH_HH_ */
