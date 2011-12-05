/*
 * FormulaFairnessMap.hh
 *
 *  Created on: Dec 12, 2010
 *      Author: kquine
 */

#ifndef FORMULAFAIRNESSMAP_HH_
#define FORMULAFAIRNESSMAP_HH_
#include "FairSet/WeakFairSet.hh"
#include "FairnessMap.hh"

namespace modelChecker {

class FormulaFairnessMap: public modelChecker::FairnessMap
{
	typedef WeakFairSet		FairSet;
public:
	FormulaFairnessMap(const GenBuchiAutomaton& property);
	virtual ~FormulaFairnessMap() {}
	int getNrFairness() const;
	bool satisfiedFairSet(const FairnessMap::FairSet* f) const;
	FairnessMap::FairSet* makeFairSet(const Transition& t);

	void mergeFairSet(FairnessMap::FairSet* f, const FairnessMap::FairSet* g);
	bool updateFairGoal(FairnessMap::FairSet::Goal* goal, const FairnessMap::FairSet* f);


private:
	const GenBuchiAutomaton& property;
	NatSet allFormulaFair;
};

}

#endif /* FORMULAFAIRNESSMAP_HH_ */
