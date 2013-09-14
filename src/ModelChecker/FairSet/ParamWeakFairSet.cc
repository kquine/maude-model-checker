/*
 * ParamWeakFairSet.cc
 *
 *  Created on: Sep 11, 2013
 *      Author: kquine
 */

#include "macros.hh"
#include "vector.hh"

#include "natSet.hh"
#include "ParamWeakFairSet.hh"

namespace modelChecker {

void
ParamWeakFairSet::merge(const FairSet& f, const AbstractFairnessTable& table)
{
	//TODO
	CantHappen("NOT IMPLEMENTED");
}

unique_ptr<FairSet>
ParamWeakFairSet::clone() const
{
	return unique_ptr<ParamWeakFairSet>(new ParamWeakFairSet(*this));
}

} /* namespace modelChecker */
