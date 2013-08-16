/*
 * CompositedFairSet.hh
 *
 *  Created on: Dec 10, 2010
 *      Author: kquine
 */

#ifndef COMPOSITEDFAIRSET_HH_
#define COMPOSITEDFAIRSET_HH_
#include "DataStructure/PtrVector.hh"
#include "FairSet.hh"

namespace modelChecker {

class CompositedFairSet: public modelChecker::FairSet
{
public:
	struct Goal;
	struct Bad;

	FairSet::Goal* makeFairGoal() const;
	FairSet::Bad* makeBadGoal() const;
	void dump(ostream& o) const;
private:
	PtrVector<FairSet> fairSets;
	friend class CompositedFairnessMap;
};

class CompositedFairSet::Goal: public FairSet::Goal
{
public:
	Goal(const CompositedFairSet* fs);
	bool empty() const;
	void dump(ostream& o) const;

private:
	PtrVector<FairSet::Goal> fairGoals;
	friend class CompositedFairnessMap;
};

struct CompositedFairSet::Bad: public FairSet::Bad
{
public:
	Bad(const CompositedFairSet* fs);
	virtual ~Bad() {}
	bool isBad(const FairSet* f) const;
	bool empty() const;
	void merge(const FairSet::Bad* b);
private:
	PtrVector<FairSet::Bad> fairBadSets;
};

}

#endif /* COMPOSITEDFAIRSET_H_ */
