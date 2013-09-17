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
#include "FairSet/ParamWeakFairSet.hh"
#include "FairSet/ParamStrongFairSet.hh"
#include "RealizedFairnessGenerator.hh"

namespace modelChecker {

template <typename Formula>
RealizedFairnessGenerator<Formula>::RealizedFairnessGenerator(const vector<unsigned int>& paramFairIds, ParamFairnessTable<Formula>& fairTable):
	paramFairIds(paramFairIds), fairTable(fairTable) {}

template <typename Formula> bool
RealizedFairnessGenerator<Formula>::empty() const
{
	return paramFairIds.empty();
}

template <typename Formula> void
RealizedFairnessGenerator<Formula>::generateRealizedFairness(const ParamPropSet& pps, ParamFairSet& fs, indexed_set<unsigned int>& insIds)
{
	cout << "---------------" << endl;
	for (auto i : paramFairIds)
	{
		const ParamSubstitutionBuilder& builder = fairTable.getParamSubstBuilder(i);
		set<ParamSubstitution> rsubsts = builder.generateParamSubstitutions(pps);

		for (auto& rs : rsubsts)
		{
			auto fi =  fairTable.insertFairnessInstance(i, rs);
			auto tpps = builder.trueParamProps(pps,rs);
			auto truth = [&] (unsigned int propId) { return pps.isParamProp(propId) ? tpps.contains(propId) : pps.isTrue(propId);};
			this->setParamFairSet(fi, fs, fairTable.getFairFormula(fi), truth);
			insIds.insert(fi);
		}
	}
	cout << endl;
}


template <>
struct RealizedFairnessGeneratorTraits<Bdd>
{
	using ParamFairSet = 	ParamWeakFairSet;

	void setParamFairSet(unsigned int fairId, ParamFairSet& fairSet, const Bdd& formula, const function<bool(int)>& truth) const
	{
		if ( !BddUtil::satisfiesFormula(formula, truth) )	fairSet.setFalsified(fairId);
	}
};

template <>
struct RealizedFairnessGeneratorTraits<pair<Bdd,Bdd>>
{
	using ParamFairSet = 	ParamStrongFairSet;

	void setParamFairSet(unsigned int fairId, ParamFairSet& fairSet, const pair<Bdd,Bdd>& formula, const function<bool(int)>& truth) const
	{
		if ( !BddUtil::satisfiesFormula(formula.first, truth) )		fairSet.setSuppFalse(fairId);
		if ( !BddUtil::satisfiesFormula(formula.second, truth) )	fairSet.setConsFalse(fairId);
	}
};

} /* namespace modelChecker */
