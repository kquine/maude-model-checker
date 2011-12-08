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
// Wrapper with folding relation.. maintaining POR data structure
// NOTE1: currently, inefficient due to using Full-Maude functions..)
// NOTE2: unfolding state will only consider "concrete" narrowing states,
//		so that we do not need to maintain the entire partial order structure,
//		but a only "collapsed" structure..
// TODO: iterator-based implementation will be required for efficiency..
// TODO: folding information should be modified when (i) backward folding,
//       and (ii) folding refinement.
// TODO: equivalent state (modulo renaming) should be treated differently.
//
class StateFoldingGraph: public CounterExampleGenerator::DagGraph
{
	NO_COPYING(StateFoldingGraph);
	typedef CounterExampleGenerator::Edge	Edge;
public:
	StateFoldingGraph(SystemGraph2* graph, const FoldingChecker* sfc, const FoldingChecker* tfc);
	virtual ~StateFoldingGraph() {}

	int getNrStates() const;
	int getNrTransitions(int stateNr) const;
	DagNode* getStateDag(int stateNr) const;
	DagNode* getTransitionDag(int stateNr, int index) const;

	int getNextState(int stateNr, int index);

	// folding stuff
	void collapseFolding();			// collapse folding structure (backward folding)

	// construct a concrete path without folding, and returns true if succeeded.
	bool constructConcretePath(const list<Edge>& path, const list<Edge>& cycle,
							   list<Edge>& resP, list<Edge>& resCy);

	// state dump
	void dump(ostream& o, int stateNr, PrettyPrinter* stateP, PrettyPrinter* transP) const;

private:
	struct State
	{
		virtual ~State() {}
		auto_ptr<Vector<pair<int,int> > > trans;	// (next state, orig trIndex)
	};

	struct MaximalState: public State
	{
		set<int> parents;		// parent (maximal/refined) state indices in folded graph
		Vector<int> backFold;	// for backward folding
	};

	struct FoldedState: public State
	{
		set<int> foldRel;	// folded States (in original graph)
	};

	struct EquivState: public State
	{
		int rep;	//
	};

	bool constConcrPath(const list<Edge>& path, const list<Edge>& cycle,
						list<Edge>::const_iterator pos, bool inCycle,
						int statePos, list<Edge>& resP, list<Edge>& resCy);

	void insertFoldedState(int stateNr);

	bool foldState(int s, int t) const;

	void dump_fold(ostream& o, int stateNr) const;

	SystemGraph2* graph;			// underlying graph
	PtrVector<State> foldGraph;		// folding graph (the same stateIndex with the underlying graph)
	set<int> maximalStates;			// set of maximal states
	set<int> refinedStates;			// set of refined states
	const FoldingChecker* sfc;		// state folding checker
	const FoldingChecker* tfc;		// transition folding checker
};


inline int
StateFoldingGraph::getNrStates() const
{
	return maximalStates.size() + refinedStates.size();
}

inline int
StateFoldingGraph::getNrTransitions(int stateNr) const
{
	Assert(stateNr < foldGraph.size(), "StateFoldingGraph::getNrTransitions: invalid state lookup");
	return foldGraph[stateNr]->trans.get() == NULL? NONE: foldGraph[stateNr]->trans->size();
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
	Assert(foldGraph[stateNr]->trans.get() != NULL,
			"StateFoldingGraph::getTransitionDag: no transition");
	Assert(index < (*foldGraph[stateNr]->trans).size(),
			"StateFoldingGraph::getTransitionDag: invalid folding transition lookup");
	return graph->getTransitionDag(stateNr,
			(*foldGraph[stateNr]->trans)[index].second);
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
