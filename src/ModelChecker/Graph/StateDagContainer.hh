/*
 * StateDagGraph.hh
 *
 *  Created on: May 24, 2011
 *      Author: kquine
 */

#ifndef STATEDAGGRAPH_HH_
#define STATEDAGGRAPH_HH_
#include "hashConsSet.hh"
#include "dagNode.hh"

namespace modelChecker {

class StateDagContainer
{
public:
	int getNrStates() const;
	DagNode* getStateDag(int index) const;	// return the dag for the index
	int insertDag(DagNode* dag);			// return the state index

private:

	Vector<int> state2hashConsIndex;	// stateIndex |-> hashconse index
	Vector<int> hashCons2dagIndex;		// partial map of hashCons indices to dag info
	HashConsSet hashConsSet;
};

inline int
StateDagContainer::getNrStates() const
{
	return state2hashConsIndex.size();
}

inline DagNode*
StateDagContainer::getStateDag(int index) const
{
	Assert(index < state2hashConsIndex.length(), "Invalid state lookup (state dag)");
	return hashConsSet.getCanonical(state2hashConsIndex[index]);
}

}

#endif /* STATEDAGGRAPH_HH_ */
