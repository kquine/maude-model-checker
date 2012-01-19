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

bool
StateFoldingGraph::concretePath(
		const list<Edge>& path, const list<Edge>& cycle,
		list<Edge>& resP, list<Edge>& resCy)
{
	// currently, supposed to be called only once.
	Assert (!path.empty() || !cycle.empty(), "ModelChecker: empty counterexample");
	bool result = false;
	resP.clear();
	resCy.clear();
	result = path.empty()?
			constConcrPath(path, cycle, cycle.begin(), true,
					cycle.front().first, resP, resCy):
			constConcrPath(path, cycle, path.begin(), false,
					path.front().first, resP, resCy);

#ifdef TDEBUG
	if (result)
	{
		cout << "\nCounterexample found in Narrowing Graph:" << endl;
		FOR_EACH_CONST(i1, list<Edge>, path)
			cout << i1->first << " =[" << i1->second << "]=> ";
		cout << "| ";
		FOR_EACH_CONST(j1, list<Edge>, cycle)
			cout << j1->first << " =[" << j1->second << "]=> ";
		cout << "$ " << endl;
		cout << "~~~ >= ~~~~~~~~~~~~~~~~" << endl;

		FOR_EACH_CONST(i2, list<Edge>, resP)
			cout << i2->first << " =[" << i2->second << "]=> ";
		cout << "| ";
		FOR_EACH_CONST(j2, list<Edge>, resCy)
			cout << j2->first << " =[" << j2->second << "]=> ";
		cout << "$\n" << endl;
	}
#endif
	return result;
}

bool
StateFoldingGraph::constConcrPath(
		const list<Edge>& path, const list<Edge>& cycle,
		list<Edge>::const_iterator pos, bool inCycle,
		int spos, list<Edge>& resP, list<Edge>& resCy)
{
	if (!inCycle && pos == path.end())
	{
		if (cycle.empty())
			return true;
		else
			return constConcrPath(path,cycle, cycle.begin(),true, spos,resP,resCy);
	}
	if (inCycle && pos == cycle.end())
	{
		//TODO: how to construct a concrete infinite path?
		//return sfc->fold(getStateDag(cycle.front().first), graph->getStateDag(spos));
		return cycle.front().first == spos;
	}

	if (foldState(pos->first, spos))
	{
		int index = 0;
		int next = NONE;
		pos++;

		while ((next = graph->getNextState(spos, index)) != NONE)
		{
			insertFoldedState(next);
			if (constConcrPath(path, cycle, pos, inCycle, next, resP, resCy))
			{
				if (inCycle)
					resCy.push_front(make_pair(spos,index));
				else
					resP.push_front(make_pair(spos,index));
				return true;
			}
			++index;
		}
	}
	return false;
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
