/*
 * SystemParamWeakFairnessMap.hh
 *
 *  Created on: Dec 15, 2010
 *      Author: kquine
 */

#ifndef SYSTEMPARAMWEAKFAIRNESSMAP_HH_
#define SYSTEMPARAMWEAKFAIRNESSMAP_HH_
#include "FairSet/ParamWeakFairSet.hh"
#include "StateRealizedFairGenerator.hh"
#include "FairnessMap.hh"

namespace modelChecker {

class SystemParamWeakFairnessMap: public modelChecker::FairnessMap
{
	typedef ParamWeakFairSet		FairSet;
public:
	SystemParamWeakFairnessMap(SystemGraph& sysGraph, StateRealizedFairGenerator& fGenerator);
	bool satisfiedFairSet(const FairnessMap::FairSet* f) const;
	int getNrFairness() const;
	FairnessMap::FairSet* makeFairSet(const Transition& t);

	void mergeFairSet(FairnessMap::FairSet* f, const FairnessMap::FairSet* g);
	bool updateFairGoal(FairnessMap::FairSet::Goal* goal, const FairnessMap::FairSet* f);

private:
	StateRealizedFairGenerator& fGenerator;
	SystemGraph& sysGraph;
};

}

#endif /* SYSTEMPARAMWEAKFAIRNESSMAP_HH_ */
