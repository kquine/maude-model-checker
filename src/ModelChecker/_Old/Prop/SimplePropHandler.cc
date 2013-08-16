/*
 * SimplePropHandler.cc
 *
 *  Created on: Jul 7, 2011
 *      Author: kquine
 */

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "freeTheory.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "dagNodeSet.hh"

#include "Graph/RewriteTransitionState.hh"

#include "SimplePropHandler.hh"

namespace modelChecker {

SimplePropHandler::SimplePropHandler(const StatePropChecker* pc, const DagNodeSet& propDags,
		                             const NatSet& propSet, ProofTermSymbol* pfg):
	BasicPropHandler(pfg), pc(pc), propDags(propDags), propSet(propSet)
{
	Assert(propSet.max() < sizeof(unsigned long), "SimplePropHandler: discouraging usage");
}

bool
SimplePropHandler::isSatisfied(const PropHandler::PropSet* d, int propId) const
{
	return safeCast(const PropSet*,d)->label.contains(propId);
}

bool
SimplePropHandler::isRelated(int propId) const
{
	return propSet.contains(propId);
}

void
SimplePropHandler::update(PropSet* d, DagNode* t)
{
	FOR_EACH_CONST(i, NatSet, propSet)
		if (pc->computeLabel(t, propDags.index2DagNode(*i)))
			d->label.insert(*i);
}

}
