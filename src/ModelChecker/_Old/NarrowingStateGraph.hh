/*
 * NarrowingStateGraph.hh
 *
 *  Created on: Jul 24, 2011
 *      Author: kquine
 */

#ifndef NARROWINGSTATEGRAPH_HH_
#define NARROWINGSTATEGRAPH_HH_
#include "pattern.hh"
#include "protectedDagNodeSet.hh"
#include "Interface/NarrowingStateGenerator.hh"
#include "Util/PtrVector.hh"

namespace modelChecker {

class NarrowingStateGraph
{
	NO_COPYING(NarrowingStateGraph);
	struct State
	{
		State(DagNode* stateDag);
		auto_ptr<Vector<int> > nextStates;	// nextState pointer is NULL if not yet explored..
		auto_ptr<Pattern> ffPattern;		// forward folding pattern
	};

public:
	NarrowingStateGraph(RewritingContext* initial);

	int getNrStates() const;
	int getNrTransitions(int stateNr) const;
	int getNextState(int stateNr, int index);
	DagNode* getStateDag(int stateNr) const;
	const Vector<int>& getInitialStates() const;

private:
	void openCurrentState(int stateNr);
	void insertWithFolding(DagNode* stateDag, Vector<int>& nextStates, set<int>& transitionSet);
	bool forwardFolding(DagNode* stateDag, Vector<int>& nextStates, set<int>& transitionSet) const;
	void addTransition(int nextState, Vector<int>& nextStates, set<int>& transitionSet) const;

	ProtectedDagNodeSet stateDags;	// state dag container
	PtrVector<State> seen;			// state information
	Vector<int> initialStates;		// initial state indices

	RewritingContext* initial;
	NarrowingStateGenerator* nsgen;
};

inline const Vector<int>&
NarrowingStateGraph::getInitialStates() const
{
	return initialStates;
}

inline void
NarrowingStateGraph::addTransition(int nextState, Vector<int>& nextStates, set<int>& transitionSet) const
{
	if (transitionSet.insert(nextState).second)
		nextStates.append(nextState);
}


}

#endif /* NARROWINGSTATEGRAPH_HH_ */
