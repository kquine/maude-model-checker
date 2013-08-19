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


#include "StateFoldingGraph.hh"

//#define TDEBUG

namespace modelChecker {

StateFoldingGraph::StateFoldingGraph(SystemGraph* graph, const FoldingRel* sfc):
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
	for (int i = 0; i < maximalStates.size() ; ++i)	// levelIndices[levelIndices.size() - 1]
	{
		if (sfc->fold(maximalStates[i], stateNr))	// forward folding
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
StateFoldingGraph::getNextState(int stateNr, int index)
{
	Assert(stateNr < foldGraph.size() && foldGraph[stateNr] != NULL,
				"StateFoldingGraph::getNextState: unknown source state folding");

	const MaximalState* ms = safeCast(const MaximalState*,foldGraph[stateNr]);
	return ms->trans.get() == NULL ? NONE :
			(index < ms->trans->size() ? (*ms->trans)[index].first : NONE);
}

//FIXME: there should be a graph mapping from StateFoldingGraph to the underlying graph...
//
//
//inline DagNode*
//StateFoldingGraph::getStateDag(int stateNr) const
//{
//	return graph->getStateDag(stateNr);
//}
//
//inline DagNode*
//StateFoldingGraph::getTransitionDag(int stateNr, int index) const
//{
//	Assert(stateNr < foldGraph.size(), "StateFoldingGraph::getTransitionDag: invalid state lookup");
//	const MaximalState* ms = safeCast(const MaximalState*,foldGraph[stateNr]);
//	Assert(ms->trans.get() != NULL, "StateFoldingGraph::getTransitionDag: no transition");
//	Assert((index == 0 && (*ms->trans).size() == 0) || index < (*ms->trans).size(),
//			"StateFoldingGraph::getTransitionDag: invalid folding transition lookup");
//	return (index == 0 && (*ms->trans).size() == 0) ? NULL
//			: graph->getTransitionDag(stateNr, (*ms->trans)[index].second);
//}


}
