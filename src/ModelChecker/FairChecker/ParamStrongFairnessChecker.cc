/*
 * ParamStrongFairnessChecker.cc
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
#include "FairSet/ParamStrongFairSet.hh"
#include "ParamStrongFairnessChecker.hh"

namespace modelChecker {

ParamStrongFairnessChecker::ParamStrongFairnessChecker(const vector<unsigned int>& strongFairIds, const vector<unsigned int>& prStrongFairIds, ParamStrongFairnessTable& fTable):
	StrongFairnessChecker(strongFairIds, fTable), RealizedFairnessGenerator(prStrongFairIds, fTable), fTableRef(fTable) {}

bool
ParamStrongFairnessChecker::empty() const
{
	return StrongFairnessChecker::empty() && RealizedFairnessGenerator::empty();
}

unique_ptr<FairSet>
ParamStrongFairnessChecker::computeAllFairness(const PropSet& trueProps)
{
	auto pss = new ParamStrongFairSet(move(static_cast<StrongFairSet&>(*StrongFairnessChecker::computeAllFairness(trueProps))));
	auto& pps = static_cast<const ParamPropSet&>(trueProps);

	for (auto& pt : this->generateRealizedFairness(pps))
	{
		pss->setRealized(pt.first);
		auto truth = [&] (unsigned int propId) { return pps.isParamProp(propId) ? pt.second.contains(propId) : pps.isTrue(propId);};
		auto& formula = fTableRef.getFairFormula(pt.first);

		if ( !BddUtil::satisfiesFormula(formula.first, truth))	pss->setSuppFalse(pt.first);
		if ( !BddUtil::satisfiesFormula(formula.second, truth))	pss->setConsFalse(pt.first);

	}
	return unique_ptr<FairSet>(pss);
}

} /* namespace modelChecker */
