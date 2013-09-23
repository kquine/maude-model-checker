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
	using size_type = vector<int>::size_type;

	unsigned int getNrStates() const;
	DagNode* getStateDag(size_type index) const;	// return the dag for the index
	unsigned int insertDag(DagNode* dag);			// return the state index

private:
	vector<size_type> state2hashConsIndex;	// stateIndex |-> hashconse index
	vector<int> hashCons2dagIndex;			// partial map of hashCons indices to dag info
	HashConsSet hashConsSet;
};

inline unsigned int
StateDagContainer::getNrStates() const
{
	return state2hashConsIndex.size();
}

inline DagNode*
StateDagContainer::getStateDag(size_type index) const
{
	Assert(index < state2hashConsIndex.size(), "Invalid state lookup (state dag)");
	return hashConsSet.getCanonical(state2hashConsIndex[index]);
}

}

#endif /* STATEDAGGRAPH_HH_ */
