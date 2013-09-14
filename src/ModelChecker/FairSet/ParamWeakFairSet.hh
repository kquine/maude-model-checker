/*
 * ParamWeakFairSet.hh
 *
 *  Created on: Sep 11, 2013
 *      Author: kquine
 */

#ifndef PARAMWEAKFAIRSET_HH_
#define PARAMWEAKFAIRSET_HH_
#include "WeakFairSet.hh"

namespace modelChecker {

class ParamWeakFairSet: public WeakFairSet
{
public:
	ParamWeakFairSet(const FairSet& f): WeakFairSet(static_cast<const WeakFairSet&>(f)) {}

	void merge(const FairSet& f, const AbstractFairnessTable& table) override;

	unique_ptr<FairSet> clone() const override;

};

} /* namespace modelChecker */
#endif /* PARAMWEAKFAIRSET_HH_ */
