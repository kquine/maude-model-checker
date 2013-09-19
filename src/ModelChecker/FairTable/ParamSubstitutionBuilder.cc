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

ParamSubstitutionBuilder::ParamSubstitutionBuilder(DagNode* fairnessDag, const vector<unsigned int>& propIds, const ParamPropositionTable& propTable):
		ParamVarInfo(fairnessDag), propTable(propTable)
{
	for (auto p : propIds)
		if (propTable.isParamProp(p))
			pidInfo.emplace_back(new PropVarInfo(p, propTable, *this));

	sort(pidInfo.begin(), pidInfo.end(),
			[](const unique_ptr<PropVarInfo>& a, decltype(a) b) { return a->varMap.size() > b->varMap.size(); });	// sorted by a number of vars
}


NatSet
ParamSubstitutionBuilder::trueParamProps(const ParamPropSet& pps, const ParamSubstitution& subst) const
{
	NatSet result;
	for (auto& pi : pidInfo)
	{
		const ParamSubstitution propSubst(subst, pi->varMap);
		if (propSubst.isTotal())
		{
			for (auto i : pps.getTrueParamSubst(pi->propId))	//FIXME: the size of getTrueParamSubst should be small.
				if (propSubst == *i)
				{
					//dumpPropSubst(pi->propId, propSubst);
					result.insert(pi->propId);
				}
		}
	}
	return result;
}

set<ParamSubstitution>
ParamSubstitutionBuilder::generateParamSubstitutions(const ParamPropSet& pps) const
{
	set<ParamSubstitution> result;
	ParamSubstitution t(ParamVarInfo::getNrVariables());
	computeParamSubstitutions(pidInfo.cbegin(), t, pps, false, result);

	return result;
}

void
ParamSubstitutionBuilder::computeParamSubstitutions(PropInfoPos i, ParamSubstitution& t, const ParamPropSet& pps, bool ever, set<ParamSubstitution>& result) const
{
	if (i != pidInfo.cend())
	{
		const ParamSubstitution orig(t, (*i)->varMap);
		for (auto target : pps.getTrueParamSubst((*i)->propId))
		{
			if ( target->isConsistent(orig) )
			{
				t.setSubst((*i)->varMap, *target);
				computeParamSubstitutions(i + 1, t, pps, true, result);

				if (orig.isTotal()) break; 	// no need to compute more if old is total (i.e., no 'bot')
			}
		}
		t.setSubst((*i)->varMap, orig);	// the "bottom" case
		computeParamSubstitutions(i + 1, t, pps, ever, result);
	}
	else
	{
		if (ever)
			result.insert(t);	//  collect the result (with copy) except for the empty case, since it's the param formula itself.
	}
}

void
ParamSubstitutionBuilder::dumpParamSubst(const ParamSubstitution& s) const
{
	cout << "{ ";
	for (unsigned int i = 0; i < s.getSubst().size(); ++i)
	{
		cout << "[" << static_cast<DagNode*>(this->index2Variable(i)) << " = ";
		if (s.getSubst()[i])
			cout << s.getSubst()[i];
		else
			cout << ".";
		cout << "] ";
	}
	cout << "}" << endl;
}

void
ParamSubstitutionBuilder::dumpPropSubst(unsigned int propId, const ParamSubstitution& s) const
{
	cout <<  " " << propTable.index2DagNode(propId) << " = ";
	cout << "{ ";
	for (unsigned int i = 0; i < s.getSubst().size(); ++i)
		cout << "[" << static_cast<Term*>(propTable.getParamVariable(propId,i)) << " = " << s.getSubst()[i] << "] ";
	cout << "} ";
}

ParamSubstitutionBuilder::PropVarInfo::PropVarInfo(unsigned int propId, const ParamPropositionTable& propTable, const ParamVarInfo& varInfo): propId(propId)
{
	varMap.resize(propTable.getParamNrVars(propId));
	for (unsigned int i = 0; i < varMap.size(); ++i)
		varMap[i] = varInfo.variable2Index(propTable.getParamVariable(propId, i));
}


} /* namespace modelChecker */
