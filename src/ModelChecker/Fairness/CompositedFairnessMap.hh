/*
 * CompositedFairnessMap.hh
 *
 *  Created on: Nov 30, 2010
 *      Author: kquine
 */

#ifndef COMPOSITEDFAIRNESSMAP_HH_
#define COMPOSITEDFAIRNESSMAP_HH_
#include "FairSet/CompositedFairSet.hh"
#include "DataStruct/PtrVector.hh"
#include "FairnessMap.hh"

namespace modelChecker {

class CompositedFairnessMap: public modelChecker::FairnessMap
{
	typedef CompositedFairSet		FairSet;
public:
	virtual ~CompositedFairnessMap() {}
	void addFairnessMap(FairnessMap* fairnessMap);

	int getNrFairness() const;
	bool satisfiedFairSet(const FairnessMap::FairSet* f) const;
	FairnessMap::FairSet* makeFairSet(const Transition& t);

	void mergeFairSet(FairnessMap::FairSet* f, const FairnessMap::FairSet* g);
	bool updateFairGoal(FairnessMap::FairSet::Goal* goal, const FairnessMap::FairSet* f);

private:
	Vector<FairnessMap*> fairMaps;
};

}


#endif /* COMPOSITEDFAIRNESSMAP_HH_ */
