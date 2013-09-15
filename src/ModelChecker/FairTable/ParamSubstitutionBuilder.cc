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
#include "interface.hh"
#include "core.hh"

// ltlr definitions
#include "ParamSubstitutionBuilder.hh"

namespace modelChecker {

ParamSubstitutionBuilder::ParamSubstitutionBuilder(DagNode* fairnessDag, const set<int>& propIds, const ParamPropositionTable& propTable):
		ParamVarInfo(fairnessDag), propTable(propTable)
{
	init(propIds);
}

vector<map<int,int>>
ParamSubstitutionBuilder::generateRealizedSubstitutions(const ParamPropSet& pps) const
{
	vector<map<int,int>> result;
	map<int,int> propIdMap;
	ParamSubstitution subst(ParamVarInfo::getNrVariables());

	computeParamSubstitutions(pidInfo.cbegin(), subst, propIdMap, pps, result);
	return result;
}

void
ParamSubstitutionBuilder::computeParamSubstitutions(
		ParamPropTable::const_iterator pos, ParamSubstitution& subst, map<int,int>& propSubstMap, const ParamPropSet& pps, vector<map<int,int>>& result) const
{
	if (pos != pidInfo.cend())
	{
		const ParamSubstitution orig(subst, (*pos)->varMap);
		const bool total = orig.isTotal();

		for (int sid : pps.getTrueParamSubst((*pos)->propId))
		{
			const ParamSubstitution& target = propTable.getParamSubst((*pos)->propId,sid);

			if ( target.isConsistent(orig) )
			{
				subst.setSubst((*pos)->varMap, target);
				propSubstMap[(*pos)->propId] = sid;
				computeParamSubstitutions(pos + 1, subst, propSubstMap, pps, result);

				if (total) return; 	// no need to compute more if old is total (i.e., no 'bot')
			}
		}
		if (!total)
		{
			subst.setSubst((*pos)->varMap, orig);	// the "bottom" case
			propSubstMap[(*pos)->propId] = NONE;
			computeParamSubstitutions(pos + 1, subst, propSubstMap, pps, result);
		}
	}
	else
	{
		result.push_back(propSubstMap);	//  collect the result (with copy)
	}
}

void
ParamSubstitutionBuilder::init(const set<int>& propIds)
{
	set<int,function<bool(int,int)>> targetPropIds([&] (int a,int b) { return propTable.getParamNrVars(a) < propTable.getParamNrVars(b); });
	for (int i : propIds)
		if (propTable.isParamProp(i)) targetPropIds.insert(i);

	set<int> cvd;								// a set of "covered" variable ids
	list<unique_ptr<PropVarInfo>> pidLists;		// a list of props not in postponed.
	list<unique_ptr<PropVarInfo>> postponed;	// a list of "completely covered" param props by the preceded ones

	for (auto j = targetPropIds.rbegin(); j != targetPropIds.rend(); ++j)	// try a prop with a greater number of vars first
	{
		bool not_covered = false;
		unique_ptr<PropVarInfo> pvi(new PropVarInfo(*j, propTable, *this));

		for (int k : pvi->varMap)
			not_covered |= cvd.insert(k).second;	// true if there is a new vid

		(not_covered ? pidLists : postponed).push_back(std::move(pvi));
	}

	pidLists.splice(pidLists.end(), postponed);
	pidInfo.resize(pidLists.size());
	std::move(pidLists.begin(), pidLists.end(), pidInfo.begin());
}


ParamSubstitutionBuilder::PropVarInfo::PropVarInfo(int propId, const ParamPropositionTable& propTable, const ParamVarInfo& varInfo): propId(propId)
{
	varMap.resize(propTable.getParamNrVars(propId));
	for (unsigned int i = 0; i < varMap.size(); ++i)
		varMap[i] = varInfo.variable2Index(propTable.getParamVariable(propId, i));
}



} /* namespace modelChecker */
