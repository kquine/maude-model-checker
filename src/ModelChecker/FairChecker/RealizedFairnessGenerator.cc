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

RealizedFairnessGenerator::RealizedFairnessGenerator(const vector<unsigned int>& paramFairIds, RealizedFairnessTable& fairTable):
	paramFairIds(paramFairIds), fairTable(fairTable) {}

deque<pair<unsigned int,NatSet>>
RealizedFairnessGenerator::generateRealizedFairness(const ParamPropSet& pps)
{
	deque<pair<unsigned int,NatSet>> result;
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
