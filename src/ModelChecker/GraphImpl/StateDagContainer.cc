/*
 * StateDagGraph.cc
 *
 *  Created on: May 24, 2011
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

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

#include "StateDagContainer.hh"

namespace modelChecker {

int
StateDagContainer::insertDag(DagNode* dag)
{
	int nextState;
	int hashConsIndex = hashConsSet.insert(dag);
	int mapSize = hashCons2dagIndex.size();

	//DebugAdvisory("replacement dag = " << dag << "hashConsIndex = " << hashConsIndex);
	if (hashConsIndex < mapSize)	// Seen before
	{
		nextState = hashCons2dagIndex[hashConsIndex];
		if (nextState != NONE)
			return nextState;
		// But only as a subdag, not as a state dag, so it counts as a new dag.
	}
	else	//	Definitely a new dag.
	{
		hashCons2dagIndex.resize(hashConsIndex + 1);
		for (int i = mapSize; i < hashConsIndex; ++i)
			hashCons2dagIndex[i] = NONE;
	}
	nextState = state2hashConsIndex.size();
	hashCons2dagIndex[hashConsIndex] = nextState;
	state2hashConsIndex.append(hashConsIndex);

	return nextState;
}

}
