/*
 * ParamWeakFairnessChecker.cc
 *
 *  Created on: Aug 20, 2013
 *      Author: kquine
 */

// utility stuff
#include "macros.hh"
#include "vector.hh"

// forward declarations
#include "interface.hh"
#include "core.hh"

// ltlr definitions
#include "FairSet/ParamWeakFairSet.hh"
#include "ParamWeakFairnessChecker.hh"

namespace modelChecker {

ParamWeakFairnessChecker::ParamWeakFairnessChecker(const vector<unsigned int>& weakFairIds, const vector<unsigned int>& paramWeakFairIds, ParamWeakFairnessTable& fTable):
	WeakFairnessChecker(weakFairIds,fTable), RealizedFairnessGenerator(paramWeakFairIds,fTable), fTableRef(fTable) {}

bool
ParamWeakFairnessChecker::empty() const
{
	return WeakFairnessChecker::empty() && RealizedFairnessGenerator::empty();
}

unique_ptr<FairSet>
ParamWeakFairnessChecker::computeAllFairness(const PropSet& trueProps)
{
	auto pws = new ParamWeakFairSet(move(static_cast<WeakFairSet&>(*WeakFairnessChecker::computeAllFairness(trueProps))));
	auto& pps = static_cast<const ParamPropSet&>(trueProps);

	for (auto& pt : this->generateRealizedFairness(pps))
	{
		pws->setRealized(pt.first);
		auto truth = [&] (unsigned int propId) { return pps.isParamProp(propId) ? pt.second.contains(propId) : pps.isTrue(propId);};
		auto& formula = fTableRef.getFairFormula(pt.first);

		if ( !BddUtil::satisfiesFormula(formula, truth))	pws->setFalsified(pt.first);
	}
	return unique_ptr<FairSet>(pws);
}

} /* namespace modelChecker */
