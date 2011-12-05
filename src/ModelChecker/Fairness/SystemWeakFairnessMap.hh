/*
 * SystemWeakFairnessMap.hh
 *
 *  Created on: Dec 12, 2010
 *      Author: kquine
 */

#ifndef SYSTEMWEAKFAIRNESSMAP_HH_
#define SYSTEMWEAKFAIRNESSMAP_HH_
#include "FairSet/WeakFairSet.hh"
#include "FairnessMap.hh"

namespace modelChecker {

class SystemWeakFairnessMap: public modelChecker::FairnessMap
{
	typedef WeakFairSet		FairSet;
public:
	SystemWeakFairnessMap(KripkeStructure& sysGraph, const Vector<Bdd>& fTable);
	virtual ~SystemWeakFairnessMap() {}
	int getNrFairness() const;
	bool satisfiedFairSet(const FairnessMap::FairSet* f) const;
	FairnessMap::FairSet* makeFairSet(const Transition& t);

	void mergeFairSet(FairnessMap::FairSet* f, const FairnessMap::FairSet* g);
	bool updateFairGoal(FairnessMap::FairSet::Goal* goal, const FairnessMap::FairSet* f);

private:
	NatSet allWeakFair;
	const Vector<Bdd>& fairConditions;
	KripkeStructure& sysGraph;
};

}

#endif /* SYSTEMWEAKFAIRNESSMAP_HH_ */
