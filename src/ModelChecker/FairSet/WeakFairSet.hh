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

class WeakFairSet: public FairSet
{
public:
	struct Goal;
	struct Bad;

	virtual ~WeakFairSet() {}

	void setFalsified(int fairId);
	virtual void merge(const FairSet& f, const AbstractFairnessTable& table) override;
	void swapFalsified(NatSet& falsified);

	bool isSatisfied() const override;
	bool operator<(const FairSet& fs) const override;

	virtual unique_ptr<FairSet> clone() const override;
	unique_ptr<FairSet::Goal> makeFairGoal() const override;
	unique_ptr<FairSet::Bad> makeBadGoal() const override;

	void dump(ostream& o) const override;

protected:
	NatSet falsifiedWeakFair;
};

class WeakFairSet::Goal: public FairSet::Goal
{
public:
	Goal(const WeakFairSet& fs);
	virtual ~Goal() {}
	bool empty() const override;
	bool update(const FairSet& f) override;
	void dump(ostream& o) const override;
protected:
	NatSet weakFairGoal;	// falsified entities
};

struct WeakFairSet::Bad: public FairSet::Bad
{
	virtual ~Bad() {}
	bool isBad(const FairSet& f) const override	{ return false; }
	bool empty() const override					{ return true; }
	void merge(const FairSet::Bad& b) override	{ }
};

}

#endif /* WEAKFAIRSET_HH_ */
