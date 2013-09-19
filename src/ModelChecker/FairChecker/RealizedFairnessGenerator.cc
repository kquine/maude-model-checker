/*
 * RealizedFairnessGenerator.cc
 *
 *  Created on: Sep 8, 2013
 *      Author: kquine
 */

//      utility stuff
#include <memory>
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "temporal.hh"
#include "interface.hh"
#include "core.hh"

// ltlr definitions
#include "RealizedFairnessGenerator.hh"

namespace modelChecker {

RealizedFairnessGenerator::RealizedFairnessGenerator(const vector<index_type>& paramFairIds, RealizedFairnessTable& fairTable):
	paramFairIds(paramFairIds), fairTable(fairTable) {}

bool
RealizedFairnessGenerator::empty() const
{
	return paramFairIds.empty();
}

deque<pair<RealizedFairnessGenerator::index_type,NatSet>>
RealizedFairnessGenerator::generateRealizedFairness(const ParamPropSet& pps)
{
	deque<pair<index_type,NatSet>> result;
	for (auto i : paramFairIds)
	{
		const ParamSubstitutionBuilder& builder = fairTable.getSubstBuilder(i);
		set<ParamSubstitution> rsubsts = builder.generateParamSubstitutions(pps);

		for (auto& rs : rsubsts)
		{
			auto fi =  fairTable.insertFairnessInstance(i, rs);
			result.push_back(make_pair(fi,builder.trueParamProps(pps,rs)));
		}
	}
	return result;
}



} /* namespace modelChecker */
