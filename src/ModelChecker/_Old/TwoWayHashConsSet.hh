/*
 * TwoWayHashConsSet.hh
 *
 *  Created on: Dec 1, 2010
 *      Author: kquine
 */

#ifndef TWOWAYHASHCONSSET_HH_
#define TWOWAYHASHCONSSET_HH_
#include "hashConsSet.hh"

namespace modelChecker {

class TwoWayHashConsSet: private HashConsSet
{
public:
	HashConsSet::getCanonical;
	int getOppositeIndex(int hashConsIndex) const;
	pair<int,bool> insertDag(DagNode* dag, int value);	// return (hashConsIndex, isNew)

private:
	Vector<int> hashCons2dagIndex;		// partial map of hashCons indices to dag info
};


inline int
TwoWayHashConsSet::getOppositeIndex(int hashConsIndex) const
{
	return hashCons2dagIndex[hashConsIndex];
}

}

#endif /* TWOWAYHASHCONSSET_HH_ */
