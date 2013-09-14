/*
 * ParamStrongFairSet.cc
 *
 *  Created on: Sep 11, 2013
 *      Author: kquine
 */

#include "macros.hh"
#include "vector.hh"

#include "natSet.hh"
#include "ParamStrongFairSet.hh"

namespace modelChecker {

void
ParamStrongFairSet::merge(const FairSet& f, const AbstractFairnessTable& table)
{
	//TODO
	CantHappen("NOT IMPLEMENTED");
}

unique_ptr<FairSet>
ParamStrongFairSet::clone() const
{
	return unique_ptr<ParamStrongFairSet>(new ParamStrongFairSet(*this));
}

} /* namespace modelChecker */
