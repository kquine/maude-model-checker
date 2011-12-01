/*
 * StateFoldingGraph.hh
 *
 *  Created on: Aug 7, 2011
 *      Author: kquine
 */

#ifndef STATEFOLDINGGRAPH_HH_
#define STATEFOLDINGGRAPH_HH_
#include "Graph/StateTransitionMetaGraph.hh"
#include "Interface/CounterExampleGenerator.hh"
#include "Interface/StateFoldingChecker.hh"
#include "Interface/PrettyPrinter.hh"
#include "Util/PtrVector.hh"
#include "protectedDagNodeSet.hh"

namespace modelChecker {

//
// Wrapper with folding relation.. (currently, inefficient due to using Full-Maude functions..)
//
class StateFoldingGraph: public CounterExampleGenerator::DagGraph
{
	NO_COPYING(StateFoldingGraph);

public:
	StateFoldingGraph(RewritingContext* parent, StateTransitionMetaGraph* graph, const StateFoldingChecker* sfc);
	virtual ~StateFoldingGraph() {}

	int getNrStates() const;
	int getNrTransitions(int stateNr) const;
	DagNode* getStateDag(int stateNr) const;
	DagNode* getTransitionDag(int stateNr, int index) const;

	int getNextState(int stateNr, int index);

	// bounded search stuff
	int getBound() const;
	void setBound(int bound = NONE);
	bool hitBound() const;
	bool hitStateBound(int stateNr) const;

	// folding stuff
	bool notFolded(int stateNr) const;				// if stateNr is a folded state or not

	// return spurious folding state (that is the closest one to the last one.)
	// Check for concrete paths that have the exact same length, if there is indeed a cycle in middle.
	int spuriousState(list<int> path, list<int>::const_iterator pos, int curr);

	// state dump
	void dump(PrettyPrinter* stateP, PrettyPrinter* transP);

private:
	struct FoldedState
	{
		FoldedState(int depth): depth(depth) {}
		const int depth;					// search depth.. (not actual BFS depth)
		Vector<int> foldingStates;			// folded state indices
		auto_ptr<Vector<int> > nextStates;	// index |-> folded states. NULL if not open yet.
	};

	void openState(int stateNr);
	void insertNewFoldedState(int stateNr, int parentDepth);

	int foldedStateSize;
	int searchBound;
	bool hitBoundFlag;

	RewritingContext* parentContext;
	PtrVector<FoldedState> states;		// folding graph
	StateTransitionMetaGraph* graph;	// underlying graph
	const StateFoldingChecker* sfc;
};


inline int
StateFoldingGraph::getNrStates() const
{
	return foldedStateSize;
}

inline int
StateFoldingGraph::getNrTransitions(int stateNr) const
{
	Assert(stateNr < states.size() && states[stateNr]->nextStates.get() != NULL,
			"StateFoldingGraph::getNrTransitions: invalid state lookup");
	return states[stateNr]->nextStates->size();
}

inline DagNode*
StateFoldingGraph::getStateDag(int stateNr) const
{
	return graph->getStateDag(stateNr);
}

inline DagNode*
StateFoldingGraph::getTransitionDag(int stateNr, int index) const
{
	//FIXME: should return the corresponding dag
	return graph->getTransitionDag(stateNr, index);
}

inline int
StateFoldingGraph::getBound() const
{
	return searchBound;
}

inline void
StateFoldingGraph::setBound(int bound)
{
	hitBoundFlag = false;
	searchBound = bound;
}

inline bool
StateFoldingGraph::hitBound() const
{
	return hitBoundFlag;
}

inline bool
StateFoldingGraph::hitStateBound(int stateNr) const
{
	return searchBound >= 0 && states[stateNr]->depth > searchBound;
}

inline bool
StateFoldingGraph::notFolded(int stateNr) const
{
	return states[stateNr] != NULL && states[stateNr]->foldingStates.empty();
}

}

#endif /* STATEFOLDINGGRAPH_HH_ */
