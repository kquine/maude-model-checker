/*
 * ParamWeakFairSet.hh
 *
 *  Created on: Sep 11, 2013
 *      Author: kquine
 */

#ifndef PARAMWEAKFAIRSET_HH_
#define PARAMWEAKFAIRSET_HH_
#include "WeakFairSet.hh"
#include "ParamRealizedSet.hh"

namespace modelChecker {

class ParamWeakFairSet: public WeakFairSet, public ParamRealizedSet
{
public:
	ParamWeakFairSet(WeakFairSet&& f);

	void merge(const FairSet& f, const AbstractFairnessTable& table) override;

	unique_ptr<FairSet> clone() const override;

	void dump(ostream& o) const override;
};

} /* namespace modelChecker */
#endif /* PARAMWEAKFAIRSET_HH_ */
