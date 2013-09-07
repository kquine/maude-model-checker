/*
 * CompositeFairSet.hh
 *
 *  Created on: Aug 20, 2013
 *      Author: kquine
 */

#ifndef COMPOSITEFAIRSET_HH_
#define COMPOSITEFAIRSET_HH_
#include <memory>
#include "FairSet.hh"

namespace modelChecker {

class CompositeFairSet: public FairSet
{
public:
	struct Goal;
	struct Bad;

	FairSet::Goal* makeFairGoal() const;
	FairSet::Bad* makeBadGoal() const;
	void dump(ostream& o) const;
private:
	vector<unique_ptr<FairSet>> fairSets;
	friend class CompositedFairnessMap;
};

class CompositeFairSet::Goal: public FairSet::Goal
{
public:
	Goal(const CompositeFairSet* fs);
	bool empty() const;
	void dump(ostream& o) const;

private:
	vector<unique_ptr<FairSet::Goal>> fairGoals;
	friend class CompositedFairnessMap;
};

struct CompositeFairSet::Bad: public FairSet::Bad
{
public:
	Bad(const CompositeFairSet* fs);
	virtual ~Bad() {}
	bool isBad(const FairSet& f) const;
	bool empty() const;
	void merge(const FairSet::Bad& b);
private:
	vector<unique_ptr<FairSet::Bad>> fairBadSets;
};

} /* namespace modelChecker */
#endif /* COMPOSITEFAIRSET_HH_ */
