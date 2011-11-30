/*
 * StateTransitionMetaGraph.hh
 *
 *  Created on: Nov 15, 2011
 *      Author: kquine
 */

#ifndef STATETRANSITIONMETAGRAPH_HH_
#define STATETRANSITIONMETAGRAPH_HH_
#include <memory>
#include "hashConsSet.hh"
#include "dagRoot.hh"
#include "Util/PtrVector.hh"
#include "RewriteTransitionState.hh"

/*
 * Generate a graph using Meta rules, given by:
 *   rl [State] => Transition' |->[State']
 * where "[_] : State -> Top" and "_|->[_] : Transition State -> Top".
 */
namespace modelChecker {

class StateTransitionMetaGraph
{
	NO_COPYING(StateTransitionMetaGraph);
public:
	StateTransitionMetaGraph(RewritingContext* initial, Symbol* stateSymbol, Symbol* transitionSymbol);
	virtual ~StateTransitionMetaGraph() {}

	int getNrStates() const;
	int getNrTransitions(int stateNr) const;
	int getNextState(int stateNr, int index);

	DagNode* getStateDag(int stateNr) const;
	DagNode* getTransitionDag(int stateNr, int index) const;
	int getStateParent(int stateNr) const;

private:
	struct State;
	struct Transition;
	struct ActiveState;

	int insertDag(DagNode* dag, int info);
	int insertState(int parentIndex, DagNode* stateDag);
	bool insertTransition(State* s, int nextState, DagNode* transitionDag);

	// symbols for meta-graph
	Symbol* stateSymbol;
	Symbol* transitionSymbol;

	// state information
	PtrVector<State> seen;
	RewritingContext* initial;

	// hash-cons information
	Vector<int> hashCons2dagIndex;		// partial map of hashCons indices to dag info
	HashConsSet hashConsSet;
	static const int transId = -11;
};

/*
 * Transition information
 */
struct StateTransitionMetaGraph::Transition
{
	int nextState;
	int hashConsIndex;

	Transition(int nextState, int hashConsIndex): nextState(nextState), hashConsIndex(hashConsIndex) {}
	bool operator<(const Transition& t) const	{ return nextState < t.nextState || hashConsIndex < t.hashConsIndex; }
};

/*
 * The information that is used only during graph search.
 */
struct StateTransitionMetaGraph::ActiveState: public RewriteTransitionState, private DagRoot
{
	typedef set<Transition*, bool (*)(const Transition*,const Transition*)> TransitionPtrSet;
	auto_ptr<TransitionPtrSet> transitionPtrSet;

	ActiveState(RewritingContext* parent, DagNode* activeDag):
		DagRoot(activeDag), RewriteTransitionState(parent,activeDag),
		transitionPtrSet(new TransitionPtrSet(trans_ptr_less)) {}
	static bool trans_ptr_less(const Transition* a, const Transition* b)	{ return (*a) < (*b); }
};

/*
 * State information
 */
struct StateTransitionMetaGraph::State
{
	int hashConsIndex;
	int parentIndex;
	auto_ptr<ActiveState> explore;
	PtrVector<Transition> transitions;

	State(int hashConsIndex, int parentIndex, RewritingContext* parentContext, DagNode* activeDag):
		hashConsIndex(hashConsIndex), parentIndex(parentIndex), explore(new ActiveState(parentContext, activeDag)) {}
};

inline int
StateTransitionMetaGraph::getNrStates() const
{
	return seen.size();
}

inline int
StateTransitionMetaGraph::getNrTransitions(int stateNr) const
{
	Assert(seen[stateNr] != NULL, "StateTransitionMetaGraph::getNrTransitions: Invalid state lookup");
	return seen[stateNr]->transitions.size();
}

inline DagNode*
StateTransitionMetaGraph::getStateDag(int stateNr) const
{
	Assert(stateNr < seen.size(), "StateTransitionMetaGraph::getStateDag: invalid state lookup (state dag)");
	return hashConsSet.getCanonical(seen[stateNr]->hashConsIndex);
}

inline DagNode*
StateTransitionMetaGraph::getTransitionDag(int stateNr, int index) const
{
	Assert(seen[stateNr] != NULL, "StateTransitionMetaGraph::getTransitionDag: Invalid state lookup");
	return hashConsSet.getCanonical(seen[stateNr]->transitions[index]->hashConsIndex);
}

inline int
StateTransitionMetaGraph::getStateParent(int stateNr) const
{
	Assert(seen[stateNr] != NULL, "StateTransitionMetaGraph::getStateParent: Invalid state lookup");
	return seen[stateNr]->parentIndex;
}

} /* namespace modelChecker */
#endif /* STATETRANSITIONMETAGRAPH_HH_ */
