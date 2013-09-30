/*
 * StateEventEnabledSystemGraph.hh
 *
 *  Created on: Aug 14, 2013
 *      Author: kquine
 */

#ifndef STATEEVENTENABLEDSYSTEMGRAPH_HH_
#define STATEEVENTENABLEDSYSTEMGRAPH_HH_
#include "BaseSystemGraphOnce.hh"
#include "PropChecker/EnabledPropHandler.hh"

namespace modelChecker {

template <typename PL> class StateEventEnabledSystemGraphLabelTraits;

/*
 * a state/event-based RW system graph with "operational" enabled propositions.
 */
template <typename PL>
class StateEventEnabledSystemGraph: public BaseSystemGraphOnce<StateEventEnabledSystemGraph<PL>>, private SystemGraphTraits<StateEventEnabledSystemGraph<PL>>
{
	friend class BaseSystemGraph<StateEventEnabledSystemGraph<PL>>;
	friend class BaseSystemGraphOnce<StateEventEnabledSystemGraph<PL>>;
	friend class SystemGraphTraits<StateEventEnabledSystemGraph<PL>>;
	using Super = BaseSystemGraphOnce<StateEventEnabledSystemGraph<PL>>;

public:
	using typename SystemGraphTraits<StateEventEnabledSystemGraph<PL>>::State;
	using typename SystemGraphTraits<StateEventEnabledSystemGraph<PL>>::Transition;

	StateEventEnabledSystemGraph(unique_ptr<PL>&& sepl, unique_ptr<EnabledPropHandler>&& enph,
			RewritingContext& initial, const ProofTermGenerator& ptg, const PropositionTable& propTable);
};


template <typename PL>
class SystemGraphTraits<StateEventEnabledSystemGraph<PL>>
{
public:
	struct State;
	struct Transition;

	SystemGraphTraits(unique_ptr<PL>&& pl, unique_ptr<EnabledPropHandler>&& enph): propLabel(move(pl)), enabledHandler(move(enph)) {}

	template <typename STATE>
	bool satisfiesStateProp(unsigned int propId, const STATE& s) const;

	template <typename TRANSITION>
	bool satisfiesEventProp(unsigned int propId, const TRANSITION& t) const;

	void updateAllLabels(DagNode* stateDag, const vector<DagNode*>& proofDags, State& s, vector<unique_ptr<Transition>>& trs) const;

protected:
	const unique_ptr<PL> propLabel;
	const unique_ptr<EnabledPropHandler> enabledHandler;
};

template <typename PL>
struct SystemGraphTraits<StateEventEnabledSystemGraph<PL>>::State: public BaseSystemGraphOnceTraits::State<Transition>, public PL::StateLabel {};

template <typename PL>
struct SystemGraphTraits<StateEventEnabledSystemGraph<PL>>::Transition: public BaseSystemGraphOnceTraits::Transition, public ProofDagGenerator, public PL::EventLabel
{
	Transition(unsigned int nextState, unsigned int transitionIndex): BaseSystemGraphOnceTraits::Transition{nextState}, ProofDagGenerator(transitionIndex) {}
	bool operator<(const Transition& t) const;
};

} /* namespace modelChecker */

#include "StateEventEnabledSystemGraph.cc"	// to separate the implementation for the template

#endif /* STATEEVENTENABLEDSYSTEMGRAPH_HH_ */
