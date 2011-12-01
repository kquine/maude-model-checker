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

namespace modelChecker {

StateFoldingGraph::StateFoldingGraph(RewritingContext* parent,
		StateTransitionMetaGraph* graph, const FoldingChecker* sfc, const FoldingChecker* tfc):
		parentContext(parent), graph(graph), foldedStateSize(0), sfc(sfc), tfc(tfc),
		searchBound(NONE), hitBoundFlag(false)
{
	insertNewFoldedState(0, -1);
}

int
StateFoldingGraph::getNextState(int stateNr, int index)
{
	if (hitStateBound(stateNr))	// bounded search
	{
		hitBoundFlag = true;
		return NONE;
	}
	else
	{
		FoldedState* n = states[stateNr];
		if (n->nextStates.get() == NULL)
			openState(stateNr);
		return index < n->nextStates->size() ? (*n->nextStates)[index] : NONE;
	}
}

int
StateFoldingGraph::spuriousState(list<int> path, list<int>::const_iterator pos, int curr)
{
	if (pos != path.end() && sfc->fold(getStateDag(*pos), getStateDag(curr), parentContext))
	{
		cout << " --> " << getStateDag(curr) << endl;
		pos++;
		int sp = NONE;
		for (int i = 0;; ++i)
		{
			int n = graph->getNextState(curr, i);
			if (n == NONE)
				break;
			else
			{
				// TODO: we need to actually compare the depths, but it's OK because of incremental searching.
				int nsp = spuriousState(path, pos, n);
				if (nsp != NONE && nsp > sp)
					sp = nsp;
			}
		}
		return sp;
	}
	else
		return NONE;
}

void
StateFoldingGraph::dump(PrettyPrinter* stateP, PrettyPrinter* transP)
{
	for (int i = 0; i < states.size(); ++i)
	{
		if (i > 0 && states[i]->depth > states[i-1]->depth)
			cout << "-------------------------------------- BOUND: " << states[i]->depth << endl;
		cout << " " << i;
		if (graph->getStateParent(i) > 0)
			cout << "(parent " << graph->getStateParent(i) << ")" ;
		if (!notFolded(i))
		{
			cout << "[folded";
			FOR_EACH_CONST(j, Vector<int>, states[i]->foldingStates)
			{
				cout << " " << *j;
			}
			cout << "]";
		}
		cout << ": ";
		stateP->print(cout, getStateDag(i), parentContext);
		cout << endl;

		// print transitions
		if (states[i]->nextStates.get() != NULL)
		{
			for (int j = 0; j < graph->getNrTransitions(i); ++j)
			{
				int nx = graph->getNextState(i,j);
				cout << "    " << (notFolded(nx)?'#':'.') << "-[ ";
				transP->print(cout, graph->getTransitionDag(i,j), parentContext);
				cout << " ]-> " << nx;
				if (!notFolded(nx))
				{
					cout << " [FOLD: ";
					FOR_EACH_CONST(k, Vector<int>, states[nx]->foldingStates)
						cout << " " << *k;
					cout << "]";
				}
				cout << endl;
			}
		}
	}
}

void
StateFoldingGraph::openState(int stateNr)
{
	//cout << "   --- State " << stateNr << " : ";
	FoldedState* n = states[stateNr];
	n->nextStates.reset(new Vector<int>());

	int ns, i = 0;
	while ((ns = graph->getNextState(stateNr, i++)) != NONE)
	{
		//cout << "  #->(" << ns << ")";
		if (ns >= states.size() || states[ns] == NULL)
			insertNewFoldedState(ns, n->depth);

		const Vector<int>& ffs = states[ns]->foldingStates;
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

void
StateFoldingGraph::insertNewFoldedState(int stateNr, int parentDepth)
{
	FoldedState* n = new FoldedState(parentDepth + 1);

	while (states.size() <= stateNr)
		states.append(NULL);
	states.replace(stateNr, n);
	DagNode* stateDag = getStateDag(stateNr);

	//FIXME: we can use the maximum state index for the depth "parentDepth"
	for (int i = 0; i < states.size(); ++i)
	{
		if (notFolded(i)
				&& states[i]->depth < n->depth	// i != stateNr
				&& sfc->fold(getStateDag(i), stateDag, parentContext))
		{
			n->foldingStates.append(i);
		}
	}

	if (n->foldingStates.empty())
		++ foldedStateSize;

	// TODO: backward folding,, (we may use the state depth for this)
}

}
