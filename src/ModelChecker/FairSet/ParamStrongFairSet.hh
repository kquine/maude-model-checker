/*
 * ParamStrongFairSet.hh
 *
 *  Created on: Sep 11, 2013
 *      Author: kquine
 */

#ifndef PARAMSTRONGFAIRSET_HH_
#define PARAMSTRONGFAIRSET_HH_
#include "StrongFairSet.hh"
#include "ParamRealizedSet.hh"

namespace modelChecker {

class ParamStrongFairSet: public StrongFairSet, public ParamRealizedSet
{
public:
	struct Bad;

	ParamStrongFairSet(StrongFairSet&& f);

	void merge(const FairSet& f, const AbstractFairnessTable& table) override;

	unique_ptr<FairSet> clone() const override;

	virtual unique_ptr<FairSet::Bad> makeBadGoal() const override;

	void dump(ostream& o) const override;
};

struct ParamStrongFairSet::Bad: public StrongFairSet::Bad
{
public:
	Bad(const ParamStrongFairSet& fs);
	bool isBad(const FairSet& f, const AbstractFairnessTable& table) const override;
};


} /* namespace modelChecker */
#endif /* PARAMSTRONGFAIRSET_HH_ */
