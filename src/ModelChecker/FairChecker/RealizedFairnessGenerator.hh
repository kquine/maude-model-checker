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
	using ParamFairSet = 	typename RealizedFairnessGeneratorTraits<Formula>::ParamFairSet;
	using RealizedSubst =	ParamSubstitutionBuilder::RealizedSubst;
public:
	RealizedFairnessGenerator(const vector<unsigned int>& paramFairIds, ParamFairnessTable<Formula>& fairTable);
	virtual ~RealizedFairnessGenerator() {}

	bool empty() const;
	void generateRealizedFairness(const ParamPropSet& pps, ParamFairSet& fs, indexed_set<unsigned int>& insIds);

private:
	const vector<unsigned int> paramFairIds;
	ParamFairnessTable<Formula>& fairTable;
};

} /* namespace modelChecker */

#include "RealizedFairnessGenerator.cc"

#endif /* REALIZEDFAIRNESSGENERATOR_HH_ */
