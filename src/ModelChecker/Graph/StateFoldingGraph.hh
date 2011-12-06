/*
 * StateFoldingGraph.hh
 *
 *  Created on: Aug 7, 2011
 *      Author: kquine
 */

#ifndef STATEFOLDINGGRAPH_HH_
#define STATEFOLDINGGRAPH_HH_
#include "Interface/FoldingChecker.hh"
#include "Interface/PrettyPrinter.hh"
#include "Interface/CounterExampleGenerator.hh"
#include "DataStruct/PtrVector.hh"
#include "protectedDagNodeSet.hh"
#include "Graph/SystemGraph2.hh"

namespace modelChecker {

//
// Wrapper with folding relation.. maintaining POR data structure
// (currently, inefficient due to using Full-Maude functions..)
//
class StateFoldingGraph: public CounterExampleGenerator::DagGraph
{
	NO_COPYING(StateFoldingGraph);
	typedef CounterExampleGenerator::Edge	Edge;
public:
	StateFoldingGraph(SystemGraph2* graph,
			const FoldingChecker* sfc, const FoldingChecker* tfc);
	virtual ~StateFoldingGraph() {}

	int getNrStates() const;
	int getNrTransitions(int stateNr) const;
	DagNode* getStateDag(int stateNr) const;
	DagNode* getTransitionDag(int stateNr, int index) const;

	int getNextState(int stateNr, int index);

	// folding stuff
	bool notFolded(int stateNr) const;	// if stateNr is a folded state or not

	// construct a concrete path without folding, and returns true if succeeded.
	bool constructConcretePath(const list<Edge>& path, const list<Edge>& cycle,
							   list<Edge>& resP, list<Edge>& resCy);

	// state dump
	void dump(int stateNr, PrettyPrinter* stateP, PrettyPrinter* transP);

private:
	struct FoldedState
	{
		Vector<int> foldStates;				// folded state indices
		PtrVector<Vector<int> > foldTrans;	// folded transitions
		auto_ptr<Vector<int> > nextStates;	// index |-> folded states. NULL if not open yet.
	};

	bool constConcrPath(const list<Edge>& path, const list<Edge>& cycle,
						list<Edge>::const_iterator pos, bool inCycle,
						int statePos, list<Edge>& resP, list<Edge>& resCy);

	void insertNewFoldedState(int stateNr);

	int foldedStateSize;

	// NOTE: folding graph and the underlying graph shares the same state index.
	PtrVector<FoldedState> states;		// folding graph
	SystemGraph2* graph;				// underlying graph

	const FoldingChecker* sfc;		// state folding
	const FoldingChecker* tfc;		// transition folding
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

inline bool
StateFoldingGraph::notFolded(int stateNr) const
{
	return states[stateNr] != NULL && states[stateNr]->foldStates.empty();
}

}

#endif /* STATEFOLDINGGRAPH_HH_ */
