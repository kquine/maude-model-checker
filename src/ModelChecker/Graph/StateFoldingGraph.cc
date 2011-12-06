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

#define TDEBUG

namespace modelChecker {

StateFoldingGraph::StateFoldingGraph(SystemGraph2* graph,
		const FoldingChecker* sfc, const FoldingChecker* tfc):
		graph(graph), foldedStateSize(0), sfc(sfc), tfc(tfc)
{
	insertNewFoldedState(0);
}

int
StateFoldingGraph::getNextState(int stateNr, int index)
{
	FoldedState* n = states[stateNr];
	if (n->nextStates.get() == NULL)	// OPEN state
	{
		//cout << "   --- State " << stateNr << " : ";
		FoldedState* n = states[stateNr];
		n->nextStates.reset(new Vector<int>());

		int ns, i = 0;
		while ((ns = graph->getNextState(stateNr, i++)) != NONE)
		{
			//cout << "  #->(" << ns << ")";
			if (ns >= states.size() || states[ns] == NULL)
				insertNewFoldedState(ns);

			const Vector<int>& ffs = states[ns]->foldStates;
			if (ffs.empty())	// if not folded
				n->nextStates->append(ns);
			else
			{
				//cout << "->[folded:";
				FOR_EACH_CONST(j, Vector<int>, ffs)
				{
					//cout << " " << *j;
					n->nextStates->append(*j);
				}
				//cout << "]";
			}
		}
		//cout << endl;
	}
	return index < n->nextStates->size() ? (*n->nextStates)[index] : NONE;
}

void
StateFoldingGraph::insertNewFoldedState(int stateNr)
{
	FoldedState* n = new FoldedState();
	states.expandTo(stateNr + 1, false);
	states.replace(stateNr, n);
	DagNode* stateDag = getStateDag(stateNr);

	//FIXME: we can use the maximum state index for the depth "parentDepth"
	for (int i = 0; i < states.size(); ++i)
	{
		if (notFolded(i)
				&& i != stateNr
				&& sfc->fold(getStateDag(i), stateDag))
		{
			n->foldStates.append(i);
		}
	}

	if (n->foldStates.empty())
		++ foldedStateSize;

	// TODO: backward folding,, (we may use the state depth for this)
}

bool
StateFoldingGraph::constructConcretePath(
		const list<Edge>& path, const list<Edge>& cycle,
		list<Edge>& resP, list<Edge>& resCy)
{
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
		return sfc->fold(getStateDag(cycle.front().first),
						 graph->getStateDag(spos));
	}
	if (sfc->fold(getStateDag(pos->first),graph->getStateDag(spos)))
	{
		int index = 0;
		int next = NONE;
		list<Edge>::const_iterator oldPos = pos;
		pos++;
		while ((next = graph->getNextState(spos, index)) != NONE)
		{
			if (tfc->fold(
					getTransitionDag(oldPos->first,oldPos->second),
					graph->getTransitionDag(spos,index)))
			{
				if (constConcrPath(path, cycle, pos, inCycle, next, resP, resCy))
				{
					if (inCycle)
						resCy.push_front(make_pair(spos,index));
					else
						resP.push_front(make_pair(spos,index));
					return true;
				}
			}
			++index;
		}
	}
	return false;
}

void
StateFoldingGraph::dump(int stateNr, PrettyPrinter* stateP, PrettyPrinter* transP)
{
	cout << " " << stateNr;
	if (graph->getStateParent(stateNr) > 0)
		cout << "(parent " << graph->getStateParent(stateNr) << ")" ;
	if (!notFolded(stateNr))
	{
		cout << "[folded";
		FOR_EACH_CONST(j, Vector<int>, states[stateNr]->foldStates)
		{
			cout << " " << *j;
		}
		cout << "]";
	}
	cout << ": ";
	stateP->print(cout, getStateDag(stateNr));
	cout << endl;

	// print transitions
	if (states[stateNr]->nextStates.get() != NULL)
	{
		for (int j = 0; j < graph->getNrTransitions(stateNr); ++j)
		{
			int nx = graph->getNextState(stateNr,j);
			cout << "    " << (notFolded(nx)?'#':'.') << "-[ ";
			transP->print(cout, graph->getTransitionDag(stateNr,j));
			cout << " ]-> " << nx;
			if (!notFolded(nx))
			{
				cout << " [FOLD: ";
				FOR_EACH_CONST(k, Vector<int>, states[nx]->foldStates)
					cout << " " << *k;
				cout << "]";
			}
			cout << endl;
		}
	}
}


}
