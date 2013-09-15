/*
 * RealizedFairnessGenerator.hh
 *
 *  Created on: Sep 8, 2013
 *      Author: kquine
 */

#ifndef REALIZEDFAIRNESSGENERATOR_HH_
#define REALIZEDFAIRNESSGENERATOR_HH_
#include "Utility/BddUtil.hh"
#include "FairTable/ParamFairnessTable.hh"

namespace modelChecker {

template <typename Formula> struct RealizedFairnessGeneratorTraits;

template <typename Formula>
class RealizedFairnessGenerator: private RealizedFairnessGeneratorTraits<Formula>
{
	typedef typename RealizedFairnessGeneratorTraits<Formula>::ParamFairSet	ParamFairSet;
public:
	RealizedFairnessGenerator(const vector<int>& paramFairIds, ParamFairnessTable<Formula>& fairTable);
	virtual ~RealizedFairnessGenerator() {}

	void generateRealizedFairness(const ParamPropSet& pps, ParamFairSet& fs, indexed_set<int>& insIds);

private:
	bool satisfiesParamFormula(const ParamPropSet& pps, const map<int,int>& subst, Bdd formula) const;

	const vector<int> paramFairIds;
	ParamFairnessTable<Formula>& fairTable;
};

template <typename Formula> inline bool
RealizedFairnessGenerator<Formula>::satisfiesParamFormula(const ParamPropSet& pps, const map<int,int>& subst, Bdd formula) const
{
	return BddUtil::satisfiesFormula(formula, [&] (int propId) { return pps.getPropTable().isParamProp(propId) ? (subst.find(propId) != subst.end()) : (pps.isTrue(propId));});
}

} /* namespace modelChecker */

#include "RealizedFairnessGenerator.cc"

#endif /* REALIZEDFAIRNESSGENERATOR_HH_ */
