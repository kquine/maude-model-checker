/*
 * FairSet.hh
 *
 *  Created on: Dec 10, 2010
 *      Author: kquine
 */

#ifndef FAIRSET_HH_
#define FAIRSET_HH_

namespace modelChecker {

struct FairSet
{
	struct Goal;
	struct Bad;

	virtual ~FairSet() {}
	virtual Goal* makeFairGoal() const = 0;
	virtual Bad* makeBadGoal() const = 0;
	virtual void dump(ostream& o) const = 0;
};

struct FairSet::Goal
{
	virtual ~Goal() {}
	virtual bool empty() const = 0;
	virtual void dump(ostream& o) const = 0;
};

struct FairSet::Bad
{
	virtual ~Bad() {}
	virtual bool isBad(const FairSet& f) const = 0;
	virtual bool empty() const = 0;
	virtual void merge(const Bad& b) = 0;
};

}

#endif /* FAIRSET_HH_ */
