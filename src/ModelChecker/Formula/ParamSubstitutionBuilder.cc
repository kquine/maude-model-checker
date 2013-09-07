/*
 * ParamSubstitutionBuilder.cc
 *
 *  Created on: Aug 29, 2013
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
#include "ParamSubstitutionBuilder.hh"

namespace modelChecker {

ParamSubstitutionBuilder::ParamSubstitutionBuilder(DagNode* fairnessDag, const set<int>& propIds, const ParamPropositionTable& propTable):
		ParamVarInfo(fairnessDag), propTable(propTable)
{
	init(propIds);
}

void
ParamSubstitutionBuilder::genRealizedSubstitutions(const ParamPropSet& pps) const
{
	map<int,int> propIdMap;
	ParamSubstitution subst(ParamVarInfo::getNrVariables());

	computeParamSubstitutions(pidLists.cbegin(), subst, propIdMap, pps);
}

void
ParamSubstitutionBuilder::computeParamSubstitutions(
		list<unique_ptr<PropVarInfo>>::const_iterator pos, ParamSubstitution& subst, map<int,int>& propIdMap, const ParamPropSet& pps) const
{
	if (pos != pidLists.cend())
	{
		ParamSubstitution orig(subst, (*pos)->varMap);
		bool total = orig.isTotal();

		for (int sid : pps.getTrueParamSubst((*pos)->propId))
		{
			const ParamSubstitution& target = propTable.getParamSubst((*pos)->propId,sid);

			if ( target.isConsistent(orig) )
			{
				subst.setSubst((*pos)->varMap, target);
				propIdMap[(*pos)->propId] = sid;
				computeParamSubstitutions(++pos, subst, propIdMap, pps);
				--pos;

				if (total) return; 	// no need to compute more if old is total (i.e., no 'bot')
			}
		}
		if (!total)
		{
			subst.setSubst((*pos)->varMap, orig);	// the "bottom" case
			propIdMap[(*pos)->propId] = NONE;
			computeParamSubstitutions(++pos, subst, propIdMap, pps);
		}
	}
	else
	{
		// FOUND!
		// insert it to the fairness table, and get an id
		// the truth value can be computed later using the info in the table..
	}
}

void
ParamSubstitutionBuilder::init(const set<int>& propIds)
{
	set<int,function<bool(int,int)>> targetPropIds([&] (int a,int b) { return propTable.getParamNrVars(a) < propTable.getParamNrVars(b); });
	for (int i : propIds)
		if (propTable.isParamProp(i)) targetPropIds.insert(i);

	set<int> cvd;								// a set of "covered" variable ids
	list<unique_ptr<PropVarInfo>> postponed;	// a set of "completely covered" param props by the preceded ones

	for (auto j = targetPropIds.rbegin(); j != targetPropIds.rend(); ++j)	// try a prop with a greater number of vars first
	{
		bool not_covered = false;
		unique_ptr<PropVarInfo> pvi(new PropVarInfo(*j, propTable, *this));

		for (int k : pvi->varMap)
			not_covered |= cvd.insert(k).second;	// true if there is a new vid

		(not_covered ? pidLists : postponed).push_back(std::move(pvi));
	}
	pidLists.splice(pidLists.end(), postponed);
}


ParamSubstitutionBuilder::PropVarInfo::PropVarInfo(int propId, const ParamPropositionTable& propTable, const ParamVarInfo& varInfo): propId(propId)
{
	varMap.resize(propTable.getParamNrVars(propId));
	for (unsigned int i = 0; i < varMap.size(); ++i)
		varMap[i] = varInfo.variable2Index(propTable.getParamVariable(propId, i));
}



} /* namespace modelChecker */
