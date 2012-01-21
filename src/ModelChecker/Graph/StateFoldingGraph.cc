/*
 * StateFoldingGraph.cc
 *
 *  Created on: Aug 7, 2011
 *      Author: kquine
 */

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//	core class definitions
#include "dagArgumentIterator.hh"
#include "rewritingContext.hh"

#include "StateFoldingGraph.hh"

//#define TDEBUG

namespace modelChecker {

StateFoldingGraph::StateFoldingGraph(SystemGraph2* graph, const FoldingChecker* sfc):
		graph(graph), sfc(sfc)
{
	levelIndices.append(0);
	insertFoldedState(0);
}

void
StateFoldingGraph::incrementLevel()
{
	if (reachFixpoint())	// do nothing if already fixpoint reached
		return;
	int oldSize = maximalStates.size();
	for(int i = levelIndices[levelIndices.size() - 1]; i < oldSize; ++i)
	{
		int mStateNr = maximalStates[i];
		MaximalState* ms = safeCast(MaximalState*,foldGraph[mStateNr]);
		Assert(ms->trans.get() == NULL, "the maximal level, but already explored");
		ms->trans.reset(new Vector<pair<int,int> >());
		for (int j = 0, n = NONE; (n = graph->getNextState(mStateNr, j)) != NONE; ++j)
		{
			insertFoldedState(n);	//	state folding
			//
			//	folding graph
			//
			if (FoldedState* fns = dynamic_cast<FoldedState*>(foldGraph[n]))
			{
				// if the target state is folded
				FOR_EACH_CONST(k, set<int>, fns->foldRel)
					ms->trans->append(make_pair(*k,j));
			}
			else
			{
				// if the target not folded
				MaximalState* mns = safeCast(MaximalState*,foldGraph[n]);
				ms->trans->append(make_pair(n,j));
			}
		}
	}
	levelIndices.append(oldSize);
}

void
StateFoldingGraph::insertFoldedState(int stateNr)
{
	if (stateNr >= foldGraph.size())
		foldGraph.expandTo(stateNr + 1, false);
	if (foldGraph[stateNr] != NULL)	// do NOTHING if stateNr have already been created
		return;

	Vector<int> foldingStates;
	DagNode* stateDag = getStateDag(stateNr);
	for (int i = 0; i < levelIndices[levelIndices.size() - 1]; ++i)
	{
		if (sfc->fold(getStateDag(maximalStates[i]), stateDag))	// forward folding
			foldingStates.append(maximalStates[i]);
	}

	if (foldingStates.empty())	// maximal cases
	{
		MaximalState* ms = new MaximalState();
		foldGraph.replace(stateNr, ms);
		maximalStates.append(stateNr);
	}
	else	// folding cases
	{
		FoldedState* fs = new FoldedState();
		foldGraph.replace(stateNr, fs);
		FOR_EACH_CONST(j, Vector<int>, foldingStates)
			fs->foldRel.insert(*j);
	}
}


int
StateFoldingGraph::getNextState(int stateNr, int index) const
{
	Assert(stateNr < foldGraph.size() && foldGraph[stateNr] != NULL,
				"StateFoldingGraph::getNextState: unknown source state folding");

	const MaximalState* ms = safeCast(const MaximalState*,foldGraph[stateNr]);
	return ms->trans.get() == NULL ? NONE :
			(index < ms->trans->size() ? (*ms->trans)[index].first : NONE);
}

void
StateFoldingGraph::dump(ostream& o, int stateNr, PrettyPrinter* stateP, PrettyPrinter* transP) const
{
	o << " " << stateNr;
	if (graph->getStateParent(stateNr) > 0)
		o << "(parent " << graph->getStateParent(stateNr) << ")" ;
	dump_fold(o, stateNr);
	o << ": ";
	stateP->print(cout, getStateDag(stateNr));
	o << endl;

	// print transitions
	for (int j = 0; j < graph->getNrTransitions(stateNr); ++j)
	{
		int nx = graph->getNextState(stateNr,j);
		o << "    " << "-[ ";
		transP->print(cout, graph->getTransitionDag(stateNr,j));
		o << " ]-> " << nx;
		dump_fold(o, nx);
		o << endl;
	}
}

void
StateFoldingGraph::dump_fold(ostream& o, int stateNr) const
{
	if (const FoldedState* fs = dynamic_cast<const FoldedState*>(foldGraph[stateNr]))
	{
		o << "[folded";
		FOR_EACH_CONST(j, set<int>, fs->foldRel)
		{
			o << " " << *j;
		}
		o << "]";
	}
}

}
