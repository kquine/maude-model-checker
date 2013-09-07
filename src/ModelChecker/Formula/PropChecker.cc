/*
 * LabelSymbolChecker.cc
 *
 *  Created on: Dec 3, 2010
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
#include "Interface/PropEvaluator.hh"
#include "PropChecker.hh"


namespace modelChecker {


PropChecker::PropChecker(const vector<int>& propIds, const PropositionTable& propositions, const PropEvaluator& pe, RewritingContext& context):
		propIds(propIds), propositions(propositions), pEvaluator(pe), context(context)  {}

unique_ptr<PropSet>
PropChecker::computeCheckResult(DagNode* target)
{
	unique_ptr<PropSet> result(new PropSet);
	for (Uint i = propIds.size(); i > 0; --i)
	{
		int pId = propIds[i-1];
		if (pEvaluator.computeProp(target, propositions.index2DagNode(pId), context))
			result->setTrue(pId);
	}
	return result;
}



}
