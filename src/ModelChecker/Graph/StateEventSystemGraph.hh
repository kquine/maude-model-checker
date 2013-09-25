/*
 * StateEventSystemGraph.hh
 *
 *  Created on: Aug 13, 2013
 *      Author: kquine
 */

#ifndef STATEEVENTSYSTEMGRAPH_HH_
#define STATEEVENTSYSTEMGRAPH_HH_
#include "BaseSystemGraphIter.hh"
#include "PropSet/PropSet.hh"

namespace modelChecker {

/*
 * an ordinary state/event-based RW system graph; every state proposition should be defined by equations.
 */
template <typename PL>
class StateEventSystemGraph: public BaseSystemGraphIter<StateEventSystemGraph<PL> >
{
	friend class BaseSystemGraph<StateEventSystemGraph<PL>>;
	friend class BaseSystemGraphIter<StateEventSystemGraph<PL>>;

public:
	StateEventSystemGraph(unique_ptr<PL>&& sepl, RewritingContext& initial, const ProofTermGenerator& ptg, const PropositionTable& propTable);

	bool satisfiesStateProp(unsigned int propId, unsigned int stateNr) const;
	bool satisfiesEventProp(unsigned int propId, unsigned int stateNr, unsigned int transitionNr) const;

private:
	using Super = 		BaseSystemGraphIter<StateEventSystemGraph<PL> >	;
	using State = 		typename BaseSystemGraphTraits<StateEventSystemGraph<PL>>::State;
	using ActiveState =	typename BaseSystemGraphTraits<StateEventSystemGraph<PL>>::ActiveState;
	using Transition =	typename BaseSystemGraphTraits<StateEventSystemGraph<PL>>::Transition;

	void updateStateLabel(DagNode* stateDag, State& s);		 /* implements */
	void updateTransitionLabel(RewriteTransitionState& rts, Transition& t, State& s);

	unique_ptr<State> createState(DagNode* stateDag) const;	 /* implements */
	unique_ptr<Transition> createTransition(unsigned int nextState, unsigned int transitionIndex) const;

	bool insertTransition(unsigned int nextState, State& n); /* implements */

	const unique_ptr<PL> propLabel;
};

template <typename PL>
struct BaseSystemGraphTraits<StateEventSystemGraph<PL> >
{
	struct Transition: public PL::EventLabel
	{
		Transition(unsigned int nextState, unsigned int transitionIndex): nextState(nextState), transitionIndex(transitionIndex) {}
		virtual ~Transition() = default;

		virtual bool operator<(const Transition& t) const;
		DagNode* makeDag(RewritingContext& parent, DagNode* stateDag, const ProofTermGenerator& ptg) const;

		const unsigned int nextState;
		const unsigned int transitionIndex;	// to construct a proofterm
	};

	struct trans_ptr_compare
	{
		bool operator() (const Transition* a, const Transition* b) const;
	};

	struct ActiveState: public RewriteTransitionState
	{
		ActiveState(RewritingContext& parent, DagNode* stateDag): RewriteTransitionState(parent,stateDag) {}
		set<Transition*, trans_ptr_compare> transitionPtrSet;	// to distinguish equivalent transitions
		unsigned int transitionCount = 0;						// to recover the corresponding proofterm
	};

	struct State: public PL::StateLabel
	{
		State(RewritingContext& parent, DagNode* stateDag): explore(new ActiveState(parent,stateDag)) {}
		virtual ~State() = default;

		vector<unique_ptr<Transition> > transitions;
		unique_ptr<ActiveState> explore;
	};
};

} /* namespace modelChecker */

#include "StateEventSystemGraph.cc"	// to separate the implementation for the template

#endif /* STATEEVENTSYSTEMGRAPH_HH_ */
