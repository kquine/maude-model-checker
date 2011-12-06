/*
 * NDFSModelChecker.hh
 *
 *  Created on: Nov 26, 2010
 *      Author: kquine
 */

#ifndef NDFSMODELCHECKER_HH_
#define NDFSMODELCHECKER_HH_
#include "buchiAutomaton2.hh"
#include "ModelChecker.hh"
#include "DataStruct/PtrVector.hh"
#include "Automaton/ProductAutomaton.hh"

namespace modelChecker {

class NDFSModelChecker: public modelChecker::ModelChecker
{
	NO_COPYING(NDFSModelChecker);
	typedef ProductAutomaton<BuchiAutomaton2>	Automaton;
	typedef Automaton::State					State;
	typedef Automaton::Transition				Transition;
	typedef Automaton::TransitionIterator		TransitionIterator;

public:
	NDFSModelChecker(Automaton& prod);

	bool findCounterExample();

private:
	struct StateSet
	{
		NatSet dfs1Seen;
	    NatSet onDfs1Stack;		// flags for states on dfs1 stack
	    NatSet dfs2Seen;
	};
	class PrefixBFSGraph;					// for generating prefix counter example

	bool dfs1(const State& initial);		// outer dfs
	bool dfs2(const State& initial);		// inner dfs
	bool trap(int propertyIndex) const;		// return true if a single self loop with transition "true"

	State cycleState;						// intersection of cycle and prefix after nested dfs
	PtrVector<StateSet> intersectionStates;
	Automaton& prod;
};

inline bool
NDFSModelChecker::trap(int propertyIndex) const
{
	if (prod.getPropertyAutomaton().isAccepting(propertyIndex))
	{
		const BuchiAutomaton2::TransitionMap& cm = prod.getPropertyAutomaton().getTransitions(propertyIndex);
		if (cm.size() == 1 && cm.begin()->first == propertyIndex && cm.begin()->second == bdd_true())
			return true;
	}
	return false;
}

}

#endif /* NDFSMODELCHECKER_HH_ */
