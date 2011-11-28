/*
 * StateTransitionMetaGraph.cc
 *
 *  Created on: Nov 15, 2011
 *      Author: kquine
 */
//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "freeTheory.hh"

//      free theory class definitions
#include "freeDagNode.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

#include "StateTransitionMetaGraph.hh"

namespace modelChecker {

StateTransitionMetaGraph::StateTransitionMetaGraph(RewritingContext* initial, Symbol* stateSymbol, Symbol* transitionSymbol):
		initial(initial), stateSymbol(stateSymbol), transitionSymbol(transitionSymbol)
{
	initial->reduce();
	insertState(NONE, initial->root());
}

int
StateTransitionMetaGraph::getNextState(int stateNr, int index)
{
	int nrTransitions = getNrTransitions(stateNr);
	State* n = seen[stateNr];
	if (index < nrTransitions)
		return n->transitions[index]->nextState;
	if (n->explore.get() == NULL)	// fully explored
		return NULL;
	while (nrTransitions <= index)
	{
		DagNode* ns = n->explore->getNextStateDag(initial);
		if (ns != NULL && ns->symbol() == transitionSymbol)		// if there is a next transition
		{
			FreeDagNode* mtDag = safeCast(FreeDagNode*, ns);
			DagNode* transDag = mtDag->getArgument(0);
			DagNode* stateDag = mtDag->getArgument(1);

			if (insertTransition(n, insertState(stateNr, stateDag), transDag))		// new transition identified
				++ nrTransitions;
			MemoryCell::okToCollectGarbage();
		}
		else
		{
			n->explore.reset();	// remove active state
			return NONE;		// no more transition,, (fully explored transitions)
		}
    }
	return n->transitions[index]->nextState;
}

int
StateTransitionMetaGraph::insertState(int parentIndex, DagNode* stateDag)
{
	int currHashConsIndex = insertDag(stateDag, getNrStates());
	int nextState = hashCons2dagIndex[currHashConsIndex];
	if (nextState == getNrStates())	// if a new state identified
	{
		static Vector<DagNode*> arg(1);
		arg[0] = hashConsSet.getCanonical(currHashConsIndex);
		DagNode* activeDag = stateSymbol->makeDagNode(arg);		// make META transitions from [STATE] ...

		State* s = new State(currHashConsIndex, parentIndex, initial, activeDag);
		seen.append(s);
	}
	return nextState;
}

bool
StateTransitionMetaGraph::insertTransition(State* s, int nextState, DagNode* transitionDag)
{
	RewriteTransitionState* rewriteState = s->explore.get();
	auto_ptr<Transition> t(new Transition(nextState, insertDag(transitionDag, transId)));

	if (s->explore->transitionPtrSet->insert(t.get()).second)	// if a new transition identified
	{
		Transition* nt = t.release();	// NOTE: transfer ownership..
		s->transitions.append(nt);
		return true;
	}
	return false;
}

int
StateTransitionMetaGraph::insertDag(DagNode* dag, int info)
{
	int hashConsIndex = hashConsSet.insert(dag);
	int mapSize = hashCons2dagIndex.size();

	//DebugAdvisory("replacement dag = " << dag << "hashConsIndex = " << hashConsIndex);
	if (hashConsIndex < mapSize)	// Seen before
	{
		int oldinfo = hashCons2dagIndex[hashConsIndex];
		if (oldinfo != NONE)
			return hashConsIndex;
		// But only as a subdag, not as a state dag, so it counts as a new dag.
	}
	else	//	Definitely a new dag.
	{
		hashCons2dagIndex.resize(hashConsIndex + 1);
		for (int i = mapSize; i <= hashConsIndex; ++i)
			hashCons2dagIndex[i] = NONE;
	}

	Assert(hashCons2dagIndex[hashConsIndex] == NONE, "StateTransitionMetaGraph::insertDag: try to overwrite");
	hashCons2dagIndex[hashConsIndex] = info;
	return hashConsIndex;
}


} /* namespace modelChecker */
