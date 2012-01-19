/*
 * StateFoldingGraph.hh
 *
 *  Created on: Aug 7, 2011
 *      Author: kquine
 */

#ifndef STATEFOLDINGGRAPH_HH_
#define STATEFOLDINGGRAPH_HH_
#include <set>
#include "Interface/FoldingChecker.hh"
#include "Interface/PrettyPrinter.hh"
#include "Interface/CounterExampleGenerator.hh"
#include "DataStruct/PtrVector.hh"
#include "protectedDagNodeSet.hh"
#include "Graph/SystemGraph2.hh"

namespace modelChecker {

//
// NOTE1: currently, inefficient due to using Full-Maude functions..
// NOTE2:  we do not maintain the entire partial order structure, but a only "collapsed" structure..
// NOTE3: we will not implement backward folding until its logical meaning is clear..
//
class StateFoldingGraph: public CounterExampleGenerator::DagGraph
{
	NO_COPYING(StateFoldingGraph);
	typedef CounterExampleGenerator::Edge	Edge;
public:
	StateFoldingGraph(SystemGraph2* graph, const FoldingChecker* sfc);
	virtual ~StateFoldingGraph() {}

	int getNrStates() const;
	int getNrTransitions(int stateNr) const;
	DagNode* getStateDag(int stateNr) const;
	DagNode* getTransitionDag(int stateNr, int index) const;

	bool reachFixpoint() const;
	int getCurrLevel() const;
	bool boundState(int stateNr) const;
	void incrementLevel();
	int getNextState(int stateNr, int index) const;

	// construct a concrete path without folding, and returns true if succeeded.
	bool concretePath(const list<Edge>& path, const list<Edge>& cycle, list<Edge>& resP, list<Edge>& resCy);
	const SystemGraph2& getUnderlyingGraph() const;

	void dump(ostream& o, int stateNr, PrettyPrinter* stateP, PrettyPrinter* transP) const;	// state dump

private:
	struct State
	{
		virtual ~State() {}
	};
	struct MaximalState: public State
	{
		auto_ptr<Vector<pair<int,int> > > trans;	// (next state, orig trIndex)
	};
	struct FoldedState: public State
	{
		set<int> foldRel;	// folded States (in original graph)
	};

	void insertFoldedState(int stateNr);
	bool foldState(int s, int t) const;

	bool constConcrPath(const list<Edge>& path, const list<Edge>& cycle,
						list<Edge>::const_iterator pos, bool inCycle,
						int statePos, list<Edge>& resP, list<Edge>& resCy);

	void dump_fold(ostream& o, int stateNr) const;

	SystemGraph2* graph;			// underlying graph
	PtrVector<State> foldGraph;		// folding graph (the same stateIndex with the underlying graph)
	const FoldingChecker* sfc;		// state folding checker

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
	return ms->trans.get() == NULL? NONE: ms->trans->size();
}

inline DagNode*
StateFoldingGraph::getStateDag(int stateNr) const
{
	return graph->getStateDag(stateNr);
}

inline DagNode*
StateFoldingGraph::getTransitionDag(int stateNr, int index) const
{
	Assert(stateNr < foldGraph.size(), "StateFoldingGraph::getTransitionDag: invalid state lookup");
	const MaximalState* ms = safeCast(const MaximalState*,foldGraph[stateNr]);
	Assert(ms->trans.get() != NULL, "StateFoldingGraph::getTransitionDag: no transition");
	Assert(index < (*ms->trans).size(), "StateFoldingGraph::getTransitionDag: invalid folding transition lookup");
	return graph->getTransitionDag(stateNr, (*ms->trans)[index].second);
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
	return safeCast(const MaximalState*,foldGraph[stateNr])->trans.get() == NULL;
}

inline const SystemGraph2&
StateFoldingGraph::getUnderlyingGraph() const
{
	return *graph;
}

inline bool
StateFoldingGraph::foldState(int s, int t) const
{
	if (t == s)
		return true;
	if (const FoldedState* ft = dynamic_cast<const FoldedState*>(foldGraph[t]))
	{
		return ft->foldRel.find(s) != ft->foldRel.end();
	}
	return false;
}


}

#endif /* STATEFOLDINGGRAPH_HH_ */
