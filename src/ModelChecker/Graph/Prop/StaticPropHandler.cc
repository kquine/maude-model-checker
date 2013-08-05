/*
 * StaticPropHandler.cc
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

#include "StaticPropHandler.hh"

namespace modelChecker {

StaticPropHandler::StaticPropHandler(const PropChecker* pc, const DagNodeSet& propDags,
        							 const NatSet& propSet, ProofTermSymbol* pfg):
	BasicPropHandler(pfg), pc(pc), propDags(propDags)
{
	global2local.expandTo(propSet.max() + 1);
	local2global.expandTo(propSet.size());
	for (int k = 0, i = 0; i < global2local.size(); ++i)
		if (propSet.contains(i))
			global2local[i] = k, local2global[k++] = i;
		else
			global2local[i] = NONE;
}
bool
StaticPropHandler::isSatisfied(const PropHandler::PropSet* d, int propId) const
{
	Assert(isRelated(propId), "StaticPropHandler::isSatisfied: invalid Prop Id..");
	return safeCast(const PropSet*,d)->label.contains(global2local[propId]);
}

bool
StaticPropHandler::isRelated(int propId) const
{
	return propId < global2local.size() && global2local[propId] >= 0;
}

void
StaticPropHandler::update(PropSet* d, DagNode* t)
{
	for(int i = local2global.length() - 1; i >= 0; --i)
		if (pc->computeLabel(t, propDags.index2DagNode(local2global[i])))
			d->label.insert(i);
}

}
