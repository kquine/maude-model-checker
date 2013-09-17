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
	for (auto p : propIds)
		if (propTable.isParamProp(p))
			pidInfo.emplace_back(new PropVarInfo(p, propTable, *this));

	sort(pidInfo.begin(), pidInfo.end(),
			[](const unique_ptr<PropVarInfo>& a, decltype(a) b) { return a->varMap.size() > b->varMap.size(); });	// sorted by a number of vars
}


NatSet
ParamSubstitutionBuilder::trueParamProps(const ParamPropSet& pps, const ParamSubstitution& subst) const
{
	cout << "subst = ";
	dumpParamSubst(subst);
	cout << "true  = ";
	NatSet result;
	for (auto& pi : pidInfo)
	{
		const unique_ptr<ParamSubstitution> propSubst(new ParamSubstitution(subst, pi->varMap));
		if (propSubst->isTotal())
		{
			auto& psubsts = propTable.getParamSubsts(pi->propId);	// propTable maintains a "unique" pointer for a ParamSubstitution
			auto i = psubsts.find(propSubst);
			if (i != psubsts.end())
			{
				auto& tsubst = pps.getTrueParamSubst(pi->propId);	// ParamPropSet has a set of "true" ParamSubstitution refs (i.e., pointers)
				if (tsubst.find(i->get()) != tsubst.end())			// TODO: ParamPropSet should be relatively small!
				{
					dumpPropSubst(pi->propId, **i);
					result.insert(pi->propId);
				}
			}
		}
	}
	cout << endl;
	return result;
}

set<ParamSubstitution>
ParamSubstitutionBuilder::generateParamSubstitutions(const ParamPropSet& pps) const
{
	set<ParamSubstitution> result;
	ParamSubstitution t(ParamVarInfo::getNrVariables());
	computeParamSubstitutions(pidInfo.cbegin(), t, pps, result);

	cout << "Generating for " << fairDag << endl;
	return result;
}

void
ParamSubstitutionBuilder::computeParamSubstitutions(vector<unique_ptr<PropVarInfo>>::const_iterator i, ParamSubstitution& t, const ParamPropSet& pps, set<ParamSubstitution>& result) const
{
	if (i != pidInfo.cend())
	{
		const ParamSubstitution orig(t, (*i)->varMap);
		for (auto target : pps.getTrueParamSubst((*i)->propId))
		{
			if ( target->isConsistent(orig) )
			{
				t.setSubst((*i)->varMap, *target);
				computeParamSubstitutions(i + 1, t, pps, result);

				if (orig.isTotal()) break; 	// no need to compute more if old is total (i.e., no 'bot')
			}
		}
		t.setSubst((*i)->varMap, orig);	// the "bottom" case
		computeParamSubstitutions(i + 1, t, pps, result);
	}
	else
	{
		result.insert(t);	//  collect the result (with copy)
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
