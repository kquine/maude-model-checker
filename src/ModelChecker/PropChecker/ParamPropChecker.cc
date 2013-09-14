/*
 * ParamPropChecker.cc
 *
 *  Created on: Aug 20, 2013
 *      Author: kquine
 */

// utility stuff
#include <memory>
#include "macros.hh"
#include "vector.hh"

// forward declarations
#include "interface.hh"
#include "core.hh"

// core class definitions
#include "rewritingContext.hh"
#include "symbolMap.hh"

// ltlr definitions
#include "Utility/ContainerUtil.hh"
#include "ParamPropChecker.hh"

namespace modelChecker {

ParamPropChecker::ParamPropChecker(const vector<int>& propIds, ParamPropositionTable& propTable, const PropEvaluator& pe, RewritingContext& context):
				PropChecker(ContainerUtil::filterVector(propIds, [&](int i) { return ! propTable.isParamProp(i); }), propTable, pe, context),
				RealizedPropGenerator(ContainerUtil::filterVector(propIds, [&](int i) { return propTable.isParamProp(i); }), propTable, pe, context) {}

unique_ptr<PropSet>
ParamPropChecker::computeCheckResult(DagNode* target)
{
	unique_ptr<PropSet> ors = PropChecker::computeCheckResult(target);
	ParamPropSet* pres = new ParamPropSet(RealizedPropGenerator::getPropTable(), std::move(*ors));
	RealizedPropGenerator::generateRealizedProps(target, *pres);
	return unique_ptr<PropSet>(pres);
}


} /* namespace modelChecker */
