/*
 * SystemStrongFairnessMap.cc
 *
 *  Created on: Dec 12, 2010
 *      Author: kquine
 */
#include "macros.hh"
#include "vector.hh"
#include "natSet.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

#include "SystemStrongFairnessMap.hh"

namespace modelChecker {

SystemStrongFairnessMap::SystemStrongFairnessMap(KripkeStructure& sysGraph, const Vector<pair<Bdd,Bdd> >& fTable):
		sysGraph(sysGraph), fairConditions(fTable) {}

bool
SystemStrongFairnessMap::satisfiedFairSet(const FairnessMap::FairSet* f) const
{
	const FairSet* nf = safeCast(const FairSet*,f);
	return nf->isSatisfied();
}

int
SystemStrongFairnessMap::getNrFairness() const
{
	return fairConditions.size();
}

FairnessMap::FairSet*
SystemStrongFairnessMap::makeFairSet(const Transition& t)
{
	NatSet supp, concl;
	for (int i = fairConditions.size() - 1; i >= 0 ; --i)
	{
		/*
		if (sysGraph.satisfiesFormula(fairConditions[i].first, t.source.system, t.systemIndex))
			supp.insert(i);
		if (sysGraph.satisfiesFormula(fairConditions[i].second, t.source.system, t.systemIndex))
			concl.insert(i);
			*/
	}
	return new FairSet(supp, concl);
}

void
SystemStrongFairnessMap::mergeFairSet(FairnessMap::FairSet* f, const FairnessMap::FairSet* g)
{
	FairSet* nf = safeCast(FairSet*,f);
	const FairSet* ng = safeCast(const FairSet*,g);
	nf->merge(ng);
}

bool
SystemStrongFairnessMap::updateFairGoal(FairnessMap::FairSet::Goal* goal, const FairnessMap::FairSet* f)
{
	return safeCast(FairSet::Goal*,goal)->update(f);
}


}
