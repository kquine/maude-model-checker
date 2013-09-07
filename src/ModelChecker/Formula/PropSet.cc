/*
 * PropSet.cc
 *
 *  Created on: Aug 26, 2013
 *      Author: kquine
 */

// utility stuff
#include <memory>
#include "macros.hh"
#include "vector.hh"

// forward declarations
#include "interface.hh"
#include "core.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNodeSet.hh"

// core class definitions
#include "rewritingContext.hh"
#include "symbolMap.hh"

// ltlr definitions
#include "PropSet.hh"

namespace modelChecker {

bool
PropSet::isTrue(int propId) const
{
	return truePropIds.contains(propId);
}

void
PropSet::setTrue(int propId)
{
	truePropIds.insert(propId);
}

void
PropSet::setTrue(const PropSet& ps)
{
	truePropIds.insert(ps.truePropIds);
}

void
PropSet::dump(ostream& s)
{
	s << truePropIds;
}



} /* namespace modelChecker */
