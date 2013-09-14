/*
 * ParamStrongFairSet.hh
 *
 *  Created on: Sep 11, 2013
 *      Author: kquine
 */

#ifndef PARAMSTRONGFAIRSET_HH_
#define PARAMSTRONGFAIRSET_HH_
#include "StrongFairSet.hh"

namespace modelChecker {

class ParamStrongFairSet: public StrongFairSet
{
public:
	ParamStrongFairSet(const FairSet& f): StrongFairSet(static_cast<const StrongFairSet&>(f)) {}

	void merge(const FairSet& f, const AbstractFairnessTable& table) override;

	unique_ptr<FairSet> clone() const override;
};

} /* namespace modelChecker */
#endif /* PARAMSTRONGFAIRSET_HH_ */
