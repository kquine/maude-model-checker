/*
 * SystemStrongFairnessMap.hh
 *
 *  Created on: Dec 12, 2010
 *      Author: kquine
 */

#ifndef SYSTEMSTRONGFAIRNESSMAP_HH_
#define SYSTEMSTRONGFAIRNESSMAP_HH_
#include "FairSet/StrongFairSet.hh"
#include "FairnessMap.hh"

namespace modelChecker {

class SystemStrongFairnessMap: public modelChecker::FairnessMap
{
	typedef StrongFairSet		FairSet;
public:
	SystemStrongFairnessMap(KripkeStructure& sysGraph, const Vector<pair<Bdd,Bdd> >& fTable);
	virtual ~SystemStrongFairnessMap() {}
	bool satisfiedFairSet(const FairnessMap::FairSet* f) const;
	int getNrFairness() const;
	FairnessMap::FairSet* makeFairSet(const Transition& t);

	void mergeFairSet(FairnessMap::FairSet* f, const FairnessMap::FairSet* g);
	bool updateFairGoal(FairnessMap::FairSet::Goal* goal, const FairnessMap::FairSet* f);
private:
	const Vector<pair<Bdd,Bdd> >& fairConditions;
	KripkeStructure& sysGraph;
};

}

#endif /* SYSTEMSTRONGFAIRNESSMAP_HH_ */
