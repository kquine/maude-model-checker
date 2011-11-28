/*
 * NarrowingStateGraph.cc
 *
 *  Created on: Jul 24, 2011
 *      Author: kquine
 */

//	utility stuff
#include <set>
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "mixfixModule.hh"

//	variable class definitions
#include "variableDagNode.hh"

//	core class definitions
#include "rewritingContext.hh"

//	higher class definitions
#include "freshVariableSource.hh"
#include "matchSearchState.hh"

#include "Util/TermUtil.hh"
#include "NarrowingStateGraph.hh"

namespace modelChecker {

NarrowingStateGraph::NarrowingStateGraph(RewritingContext* initial)
	: initial(initial)
{
	initial->reduce();
	Vector<DagNode*> initialDags(0,1);
	nsgen->getVariantStates(initial, initial->root(), initialDags);

	// TODO: check subsumption here?
	for (int i = 0; i < initialDags.size(); ++i)
	{
		if (stateDags.dagNode2Index(initialDags[i]) == NONE)
		{
			stateDags.insert(initialDags[i]);
			seen.append(new State(initialDags[i]));
			initialStates.append(i);
		}
	}
}

int
NarrowingStateGraph::getNrStates() const
{
	return seen.size();
}

int
NarrowingStateGraph::getNrTransitions(int stateNr) const
{
	Assert(seen[stateNr] != NULL, "NarrowingStateGraph::getNrTransitions: Invalid state lookup");
	return seen[stateNr]->nextStates.get() == NULL ? NONE : seen[stateNr]->nextStates->size();
}

DagNode*
NarrowingStateGraph::getStateDag(int stateNr) const
{
	return stateDags.index2DagNode(stateNr);
}

int
NarrowingStateGraph::getNextState(int stateNr, int index)
{
	int nrTransitions = getNrTransitions(stateNr);
	if (nrTransitions == NONE)
		openCurrentState(stateNr);
	const Vector<int>& nextStates = *seen[stateNr]->nextStates;
	return index < nextStates.size() ? nextStates[index] : NONE;
}

void
NarrowingStateGraph::openCurrentState(int stateNr)
{
	Assert(seen[stateNr]->nextStates.get() == NULL, "NarrowingStateGraph::expandState: already opened");

	State* n = seen[stateNr];
	n->nextStates.reset(new Vector<int>());
	set<int> transitionSet;

	Vector<DagNode*> nextStates;
	nsgen->getNextStates(initial, getStateDag(stateNr), nextStates);

	FOR_EACH_CONST(ns, Vector<DagNode*>, nextStates)
	{
		insertWithFolding(*ns, *n->nextStates.get(), transitionSet);
	}
}

void
NarrowingStateGraph::insertWithFolding(DagNode* stateDag, Vector<int>& nextStates, set<int>& transitionSet)
{
	if (int index = stateDags.dagNode2Index(stateDag) != NONE) // if existing state
	{
		addTransition(index, nextStates, transitionSet);
	}
	else if (! forwardFolding(stateDag, nextStates, transitionSet))	// new state (not folded)
	{
		int index = stateDags.insert(stateDag);
		Assert(index == seen.size(), "NarrowingStateGraph::insertWithFolding: existing states, but not folded");
		cout << "State " << index << " : " << stateDag << endl;
		seen.append(new State(stateDag));
		addTransition(index, nextStates, transitionSet);
	}
}

bool
NarrowingStateGraph::forwardFolding(DagNode* stateDag, Vector<int>& nextStates, set<int>& transitionSet) const
{
	bool everFolded = false;
	auto_ptr<RewritingContext> stateContext(initial->makeSubcontext(stateDag));
	for (int i = 0; i < seen.size(); ++i)
	{
		MatchSearchState ff(stateContext.get(), seen[i]->ffPattern.get(), MatchSearchState::GC_SUBSTITUTION);
		if (ff.findNextMatch())	// matches with the existing state (including itself)
		{
			everFolded = true;
			addTransition(i, nextStates, transitionSet);
		}
	}
	return everFolded;
}

NarrowingStateGraph::State::State(DagNode* stateDag):
		ffPattern(new Pattern(TermUtil::constructTerm(stateDag), true)) {}


}
