/*
 * SystemParamStrongFairnessMap.hh
 *
 *  Created on: Dec 15, 2010
 *      Author: kquine
 */

#ifndef SYSTEMPARAMSTRONGFAIRNESSMAP_HH_
#define SYSTEMPARAMSTRONGFAIRNESSMAP_HH_
#include "StateRealizedFairGenerator.hh"
#include "FairSet/ParamStrongFairSet.hh"
#include "FairnessMap.hh"

namespace modelChecker {

class SystemParamStrongFairnessMap: public modelChecker::FairnessMap
{
	typedef ParamStrongFairSet		FairSet;
public:
	SystemParamStrongFairnessMap(SystemGraph& sysGraph, StateRealizedFairGenerator& fGenerator);
	virtual ~SystemParamStrongFairnessMap() {}
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

#endif /* SYSTEMPARAMSTRONGFAIRNESSMAP_HH_ */
