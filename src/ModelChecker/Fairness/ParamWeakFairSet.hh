/*
 * ParamWeakFairSet.hh
 *
 *  Created on: Dec 12, 2010
 *      Author: kquine
 */

#ifndef PARAMWEAKFAIRSET_HH_
#define PARAMWEAKFAIRSET_HH_
#include "ParamRealizedSet.hh"
#include "WeakFairSet.hh"

namespace modelChecker {

class ParamWeakFairSet: public WeakFairSet, private ParamRealizedSet
{
public:
	struct Goal;

	ParamWeakFairSet(const NatSet& realized, const NatSet& satisfied);
	void merge(const ParamWeakFairSet* pwf, const NatSet& unrealizedTrue);

	bool isSatisfied() const;
	void dump(ostream& o) const;
	FairSet::Goal* makeFairGoal() const;
};

class ParamWeakFairSet::Goal: public WeakFairSet::Goal
{
public:
	Goal(const ParamWeakFairSet* fs);
	virtual ~Goal() {}
	bool update(const FairSet* f, const NatSet& unrealizedTrue);
};


}

#endif /* PARAMWEAKFAIRSET_HH_ */
