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

//      interface class definitions
#include "symbol.hh"
#include "dagNodeSet.hh"
#include "term.hh"

// core class definitions
#include "rewritingContext.hh"
#include "equation.hh"
#include "dagArgumentIterator.hh"

// ltlr definitions
#include "FairSet/ParamWeakFairSet.hh"
#include "FairSet/ParamStrongFairSet.hh"
#include "RealizedFairnessGenerator.hh"

namespace modelChecker {

template <typename Formula>
RealizedFairnessGenerator<Formula>::RealizedFairnessGenerator(const vector<int>& paramFairIds, ParamFairnessTable<Formula>& fairTable):
	paramFairIds(paramFairIds), fairTable(fairTable) {}

template <typename Formula> void
RealizedFairnessGenerator<Formula>::generateRealizedFairness(const ParamPropSet& pps, ParamFairSet& fs, indexed_set<int>& insIds)
{
	for (int i : paramFairIds)
	{
		const ParamSubstitutionBuilder& builder = fairTable.getParamSubstBuilder(i);
		vector<map<int,int>> rsubsts = builder.generateRealizedSubstitutions(pps);

		for (auto& rs : rsubsts)
		{
			int fi =  fairTable.insertFairnessInstance(i, rs);
			this->setParamFairSet(fi, fs, fairTable.getFairFormula(fi), [&](Bdd f){ return this->satisfiesParamFormula(pps,rs,f); });
			insIds.insert(fi);
		}
	}
}


template <typename Formula> bool
RealizedFairnessGenerator<Formula>::satisfiesParamFormula(const ParamPropSet& pps, const map<int,int>& subst, Bdd formula) const
{
	for(;;)
	{
		if (formula == bdd_true())
			return true;
		else if (formula == bdd_false())
			return false;
		else
		{
			int propId = bdd_var(formula);
			if (pps.getPropTable().isParamProp(propId))
				formula = (subst.find(propId) != subst.end()) ? bdd_high(formula) : bdd_low(formula);
			else
				formula = pps.isTrue(propId) ? bdd_high(formula) : bdd_low(formula);
		}
	}
}


template <>
struct RealizedFairnessGeneratorTraits<Bdd>
{
	typedef ParamWeakFairSet	ParamFairSet;
	void setParamFairSet(int fairId, ParamFairSet& fairSet, const Bdd& formula, function<bool(Bdd)> satisfiesFormula) const
	{
		if ( ! satisfiesFormula(formula) )
			fairSet.setFalsified(fairId);
	}
};

template <>
struct RealizedFairnessGeneratorTraits<pair<Bdd,Bdd>>
{
	typedef ParamStrongFairSet	ParamFairSet;
	void setParamFairSet(int fairId, ParamFairSet& fairSet, const pair<Bdd,Bdd>& formula, function<bool(Bdd)> satisfiesFormula) const
	{
		if ( ! satisfiesFormula(formula.first) )
			fairSet.setSuppFalse(fairId);
		if ( ! satisfiesFormula(formula.second) )
			fairSet.setConsFalse(fairId);
	}
};

} /* namespace modelChecker */
