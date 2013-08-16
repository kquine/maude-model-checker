/*
 * ModelCheckerFactory.cc
 *
 *  Created on: Aug 6, 2013
 *      Author: kquine
 */

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "temporal.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//	ltlr definitions
#include "ModelCheckerFactory.hh"

namespace modelChecker {

ModelCheckerFactory::ModelCheckerFactory()
{
	// TODO Auto-generated constructor stub
}


ModelChecker*
ModelCheckerFactory::getModelChecker()
{
	return NULL;
}

const DagGraphMap*
ModelCheckerFactory::getDagGraphMap() const
{
	return NULL;
}

} /* namespace modelChecker */
