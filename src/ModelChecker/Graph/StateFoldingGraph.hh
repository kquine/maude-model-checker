/*
 * StateFoldingGraph.hh
 *
 *  Created on: Aug 7, 2011
 *      Author: kquine
 */

#ifndef STATEFOLDINGGRAPH_HH_
#define STATEFOLDINGGRAPH_HH_
#include <set>
#include "GraphInterface/SystemGraph.hh"
#include "IO/PrettyPrinter.hh"
#include "DataStructure/PtrVector.hh"
#include "protectedDagNodeSet.hh"

namespace modelChecker {

//
// NOTE1: currently, inefficient due to using Full-Maude functions..
// NOTE2:  we do not maintain the entire partial order structure, but a only "collapsed" structure..
// NOTE3: we will not implement backward folding until its logical meaning is clear..
//
class StateFoldingGraph: public SystemGraph
{
public:
	struct FoldingRel
	{
		virtual bool fold(int s, int t) const = 0;
	};

	StateFoldingGraph(SystemGraph* graph, const FoldingRel* sfc);
	StateFoldingGraph(const StateFoldingGraph&) = delete;
	StateFoldingGraph& operator=(const StateFoldingGraph&) = delete;

	virtual ~StateFoldingGraph() = default;

	int getNrStates() const;
	int getNrTransitions(int stateNr) const;
	int getNextState(int stateNr, int index);

	SystemGraph& getUnderlyingGraph();

	bool reachFixpoint() const;
	int getCurrLevel() const;
	bool boundState(int stateNr) const;
	void incrementLevel();

private:
	struct State
	{
		virtual ~State() = default;
	};
	struct MaximalState: public State
	{
		unique_ptr<Vector<pair<int,int> > > trans;	// (next state, orig trIndex)
	};
	struct FoldedState: public State
	{
		set<int> foldRel;	// folded States (in original graph)
	};

	void insertFoldedState(int stateNr);

	SystemGraph* graph;							// underlying graph

	vector<unique_ptr<State> > foldGraph;		// folding graph (the same stateIndex with the underlying graph)
	const FoldingRel* sfc;						// state folding checker

	Vector<int> maximalStates;		// the maximal state indices
	Vector<int> levelIndices;		// the vector of the least indices with each level (in maximalStates)

};

inline int
StateFoldingGraph::getNrStates() const
{
	return maximalStates.size();
}

inline int
StateFoldingGraph::getNrTransitions(int stateNr) const
{
	Assert(stateNr < foldGraph.size(), "StateFoldingGraph::getNrTransitions: invalid state lookup");
	const MaximalState* ms = safeCast(const MaximalState*,foldGraph[stateNr]);
	return ms->trans ? NONE: ms->trans->size();
}

inline SystemGraph&
StateFoldingGraph::getUnderlyingGraph()
{
	return *graph;
}

inline bool
StateFoldingGraph::reachFixpoint() const
{
	return levelIndices[levelIndices.size() - 1] >= maximalStates.size();
}

inline int
StateFoldingGraph::getCurrLevel() const
{
	return reachFixpoint() ? levelIndices.size() - 1 : levelIndices.size();
}

inline bool
StateFoldingGraph::boundState(int stateNr) const
{
	return safeCast(const MaximalState*,foldGraph[stateNr])->trans;
}


}

#endif /* STATEFOLDINGGRAPH_HH_ */
