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
	for (auto pid : propIds)
	{
		if (propTable.isParamProp(pid))
		{
			varMaps[pid] = initPropVarInfo(pid, propTable);
			paramPids.push_back(pid);
		}
	}
	sort(paramPids.begin(), paramPids.end(), [&](unsigned int a, unsigned int b) { return varMaps.at(a).size() > varMaps.at(b).size(); });	// sorted by a number of vars
}


NatSet
ParamSubstitutionBuilder::trueParamProps(const ParamPropSet& pps, const ParamSubstitution& subst) const
{
	cout << "subst = ";
	dumpParamSubst(subst);
	cout << "true  = ";
	NatSet result;
	for (auto propId : paramPids)
	{
		const unique_ptr<ParamSubstitution> propSubst(new ParamSubstitution(subst, varMaps.at(propId)));
		if (propSubst->isTotal())
		{
			auto& psubsts = propTable.getParamSubsts(propId);	// propTable maintains a "unique" pointer for a ParamSubstitution
			auto i = psubsts.find(propSubst);
			if (i != psubsts.end())
			{
				auto& tsubst = pps.getTrueParamSubst(propId);	// ParamPropSet has a set of "true" ParamSubstitution refs (i.e., pointers)
				if (tsubst.find(i->get()) != tsubst.end())
				{
					dumpPropSubst(propId, **i);
					result.insert(propId);
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
	computeParamSubstitutions(paramPids.cbegin(), t, pps, result);

	cout << "Generating for " << fairDag << endl;
	return result;
}

void
ParamSubstitutionBuilder::computeParamSubstitutions(vector<unsigned int>::const_iterator i, ParamSubstitution& t, const ParamPropSet& pps, set<ParamSubstitution>& result) const
{
	if (i != paramPids.cend())
	{
		auto vm = varMaps.at(*i);
		const ParamSubstitution orig(t, vm);
		for (auto target : pps.getTrueParamSubst(*i))
		{
			if ( target->isConsistent(orig) )
			{
				t.setSubst(vm, *target);
				computeParamSubstitutions(i + 1, t, pps, result);

				if (orig.isTotal()) break; 	// no need to compute more if old is total (i.e., no 'bot')
			}
		}
		t.setSubst(vm, orig);	// the "bottom" case
		computeParamSubstitutions(i + 1, t, pps, result);
	}
	else
	{
		result.insert(t);	//  collect the result (with copy)
	}
}

vector<unsigned int>
ParamSubstitutionBuilder::initPropVarInfo(unsigned int propId, const ParamPropositionTable& propTable) const
{
	vector<unsigned int> vm(propTable.getParamNrVars(propId));
	for (unsigned int i = 0; i < vm.size(); ++i)
		vm[i] = this->variable2Index(propTable.getParamVariable(propId, i));
	return vm;
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


} /* namespace modelChecker */
