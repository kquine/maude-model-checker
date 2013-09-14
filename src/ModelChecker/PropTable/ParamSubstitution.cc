/*
 * ParamSubstitution.cc
 *
 *  Created on: Aug 30, 2013
 *      Author: kquine
 */

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNodeSet.hh"
#include "term.hh"

// core class definitions
#include "rewritingContext.hh"
#include "equation.hh"
#include "rule.hh"

// ltlr definitions
#include "ParamSubstitution.hh"

namespace modelChecker {

ParamSubstitution::ParamSubstitution(const VariableInfo& vi, const Substitution& sb): subst(vi.getNrRealVariables())
{
	for (int j = 0; j < vi.getNrRealVariables(); ++j)
		subst[j] = sb.value(j);
}

ParamSubstitution::ParamSubstitution(const ParamSubstitution& src, const vector<int>& varMap): subst(varMap.size())
{
	setSubst(src, varMap);
}

bool
ParamSubstitution::isTotal() const
{
	for (DagNode* d : subst)
		if (d == nullptr)
			return false;
	return true;
}

bool
ParamSubstitution::isConsistent(const ParamSubstitution& other) const
{
	for (unsigned int j = 0; j < subst.size(); ++j)
		if (subst[j] != other.subst[j] && subst[j] != nullptr && other.subst[j] != nullptr)
			return false;
	return true;
}

const vector<DagNode*>&
ParamSubstitution::getSubst() const
{
	return subst;
}

void
ParamSubstitution::setSubst(const ParamSubstitution& src, const vector<int>& varMap)
{
	for (unsigned int j = 0; j < varMap.size(); ++j)
		subst[j] = src.subst[varMap[j]];
}

void
ParamSubstitution::setSubst(const vector<int>& varMap, const ParamSubstitution& src)
{
	for (unsigned int j = 0; j < varMap.size(); ++j)
		subst[varMap[j]] = src.subst[j];
}


} /* namespace modelChecker */
