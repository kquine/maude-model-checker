/*
 * LabelFairHandler.cc
 *
 *  Created on: Aug 27, 2013
 *      Author: kquine
 */

// utility stuff
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
#include "LabelFairHandler.hh"

namespace modelChecker {


bool
LabelFairHandler::Label::operator<(const FairHandler::Label& l) const
{
	// TODO
	return false;
}


void
LabelFairHandler::updateLabel(const FairSet& f, FairHandler::Label& l) const
{
	// TODO
}

} /* namespace modelChecker */
