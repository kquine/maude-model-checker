/*
 * WeakFairSet.hh
 *
 *  Created on: Dec 10, 2010
 *      Author: kquine
 */

#ifndef WEAKFAIRSET_HH_
#define WEAKFAIRSET_HH_

#include "FairSet.hh"

namespace modelChecker {

class WeakFairSet: public modelChecker::FairSet
{
public:
	struct Goal;
	struct Bad;

	WeakFairSet(const NatSet& satisfied);
	void merge(const WeakFairSet* wf);

	bool isSatisfied(const NatSet& all) const;
	void dump(ostream& o) const;
	FairSet::Goal* makeFairGoal() const;
	FairSet::Bad* makeBadGoal() const;

protected:
	NatSet satisfiedWeakFair;
};

class WeakFairSet::Goal: public FairSet::Goal
{
public:
	Goal(const WeakFairSet* fs);
	bool empty() const;
	bool update(const FairSet* f);
	void dump(ostream& o) const;
protected:
	NatSet weakFairGoal;	// falsified entities
};

struct WeakFairSet::Bad: public FairSet::Bad
{
	bool isBad(const FairSet* f) const	{ return false; }
	bool empty() const					{ return true; }
	void merge(const FairSet::Bad* b)		{}
};

}

#endif /* WEAKFAIRSET_HH_ */
