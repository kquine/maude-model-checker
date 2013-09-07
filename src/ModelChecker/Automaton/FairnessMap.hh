/*
 * FairGraph.h
 *
 *  Created on: Nov 18, 2010
 *      Author: kquine
 */

#ifndef FAIRNESSMAP_HH_
#define FAIRNESSMAP_HH_
#include "Fairness/FairSet.hh"
#include "ProductAutomaton.hh"

namespace modelChecker {

struct FairnessMap
{
	typedef product::Transition<GenBuchiAutomaton>	Transition;

	virtual ~FairnessMap() {}
	virtual bool satisfiedFairSet(const FairSet* f) const = 0;
	virtual FairSet* makeFairSet(const Transition& t) = 0;
	virtual int getNrFairness() const = 0;

	virtual void mergeFairSet(FairSet* f, const FairSet* g) = 0;
	virtual bool updateFairGoal(FairSet::Goal* goal, const FairSet* f) = 0;
};

}

#endif /* FAIRNESSMAP_HH_ */
