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

ParamSubstitutionBuilder::ParamSubstitutionBuilder(DagNode* fairnessDag, const set<unsigned int>& propIds, const ParamPropositionTable& propTable):
		ParamVarInfo(fairnessDag), propTable(propTable),
		fairDag(fairnessDag)	// TODO;
{
	init(propIds);
}

vector<ParamSubstitutionBuilder::RealizedSubst>
ParamSubstitutionBuilder::generateRealizedSubstitutions(const ParamPropSet& pps) const
{
	vector<RealizedSubst> result;
	RealizedSubst propIdMap;
	ParamSubstitution subst(ParamVarInfo::getNrVariables());

	cout << "Generating for " << fairDag << endl;
	computeParamSubsts(pidInfo.cbegin(), subst, propIdMap, pps, result);
	return result;
}

void
ParamSubstitutionBuilder::computeParamSubsts(ParamList::const_iterator pos, ParamSubstitution& subst, RealizedSubst& rsubst, const ParamPropSet& pps, vector<RealizedSubst>& result) const
{
	if (pos != pidInfo.cend())
	{
		const ParamSubstitution orig(subst, (*pos)->varMap);
		const bool total = orig.isTotal();

		for (unsigned int sid : pps.getTrueParamSubst((*pos)->propId))
		{
			const ParamSubstitution& target = propTable.getParamSubst((*pos)->propId,sid);

			if ( target.isConsistent(orig) )
			{
				subst.setSubst((*pos)->varMap, target);
				rsubst[(*pos)->propId] = sid;
				computeParamSubsts(pos + 1, subst, rsubst, pps, result);

				if (total) return; 	// no need to compute more if old is total (i.e., no 'bot')
			}
		}
		if (!total)
		{
			subst.setSubst((*pos)->varMap, orig);	// the "bottom" case
			rsubst.erase((*pos)->propId);
			computeParamSubsts(pos + 1, subst, rsubst, pps, result);
		}
	}
	else
	{
		for (auto j : rsubst)
		{
			cout <<  "   " << propTable.index2DagNode(j.first) << " = { ";
			const auto& sub = propTable.getParamSubst(j.first, j.second).getSubst();
			for (unsigned int i = 0; i < sub.size(); ++i)
			{
				cout << "[" << static_cast<DagNode*>(this->index2Variable(i)) << " = " << sub[i] << "] ";
			}
			cout << "}";
		}
		if (!rsubst.empty()) cout << endl;
		result.push_back(rsubst);	//  collect the result (with copy)
	}
}

void
ParamSubstitutionBuilder::init(const set<unsigned int>& propIds)
{
	auto compare = [&] (unsigned int a,unsigned int b) { return propTable.getParamNrVars(a) < propTable.getParamNrVars(b) || a < b; };
	set<unsigned int,decltype(compare)> targetPropIds(compare);
	for (auto i : propIds)
		if (propTable.isParamProp(i)) targetPropIds.insert(i);

	set<unsigned int> cvd;						// a set of "covered" variable ids
	list<unique_ptr<PropVarInfo>> pidLists;		// a list of props not in postponed.
	list<unique_ptr<PropVarInfo>> postponed;	// a list of "covered" param props by the preceded ones (NOTE: all props in pidLists can be bots)

	for (auto j = targetPropIds.rbegin(); j != targetPropIds.rend(); ++j)	// try a prop with a greater number of vars first
	{
		bool not_covered = false;
		unique_ptr<PropVarInfo> pvi(new PropVarInfo(*j, propTable, *this));

		for (auto k : pvi->varMap)
			not_covered |= cvd.insert(k).second;	// true if there is a new vid

		(not_covered ? pidLists : postponed).push_back(std::move(pvi));
	}
	pidLists.splice(pidLists.end(), postponed);
	pidInfo.resize(pidLists.size());
	std::move(pidLists.begin(), pidLists.end(), pidInfo.begin());
}


ParamSubstitutionBuilder::PropVarInfo::PropVarInfo(unsigned int propId, const ParamPropositionTable& propTable, const ParamVarInfo& varInfo): propId(propId)
{
	varMap.resize(propTable.getParamNrVars(propId));
	for (unsigned int i = 0; i < varMap.size(); ++i)
		varMap[i] = varInfo.variable2Index(propTable.getParamVariable(propId, i));
}



} /* namespace modelChecker */
