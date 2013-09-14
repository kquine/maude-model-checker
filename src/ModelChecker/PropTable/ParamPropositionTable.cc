/*
 * ParamPropositionTable.cc
 *
 *  Created on: Aug 23, 2013
 *      Author: kquine
 */

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "temporal.hh"
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "higher.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNodeSet.hh"
#include "term.hh"
#include "dagArgumentIterator.hh"
#include "argumentIterator.hh"
#include "subproblem.hh"
#include "extensionInfo.hh"

//		higher class definitions
#include "stateTransitionGraph.hh"
#include "searchState.hh"
#include "matchSearchState.hh"

// core class definitions
#include "rewritingContext.hh"
#include "lhsAutomaton.hh"
#include "conditionState.hh"
#include "equation.hh"
#include "rule.hh"
#include "termBag.hh"
#include "rhsBuilder.hh"

// ltlr definitions
#include "Utility/TermUtil.hh"
#include "ParamPropositionTable.hh"

namespace modelChecker {

ParamSubstitution::ParamSubstitution(vector<DagNode*>::size_type size): subst(size,nullptr) {}

ParamPropositionTable::ParamPropositionTable(const PropInterpreter& pi): PropositionTable(pi)  {}

bool
ParamPropositionTable::hasParamProp() const
{
	return ! paramInfoTable.empty();
}

bool
ParamPropositionTable::isParamProp(int propId) const
{
	return getParamPropInfo(propId) != nullptr;
}

int
ParamPropositionTable::getParamNrVars(int propId) const
{
	const ParamPropInfo* ppi = getParamPropInfo(propId);
	Assert(ppi != nullptr, "ParamPropositionTable::getParamNrVars: not a param substitution");
	return paramInfoTable[ppi->paramInfoId]->pattern->getNrRealVariables();
}

Term*
ParamPropositionTable::getParamVariable(int propId, int varId) const
{
	const ParamPropInfo* ppi = getParamPropInfo(propId);
	Assert(ppi != nullptr, "ParamPropositionTable::getParamVariable: not a param substitution");
	return paramInfoTable[ppi->paramInfoId]->pattern->index2Variable(varId);
}


const ParamSubstitution&
ParamPropositionTable::getParamSubst(int propId, int substId) const
{
	const ParamPropInfo* ppi = getParamPropInfo(propId);
	Assert(ppi != nullptr, "ParamPropositionTable::getParamSubst: not a param substitution");
	return *paramInfoTable[ppi->paramInfoId]->substitutions[substId];
}

const map<int,set<int> >*
ParamPropositionTable::getParamMatches(int propId) const
{
	if (const InstancePropInfo* ipi = getInstancePropInfo(propId))
		return & ipi->matchingPropNSubstIds;
	else
		return nullptr;
}

int
ParamPropositionTable::insertInstanceAndUpdate(DagNode* propDag, RewritingContext& parentContext)
{
	int propId = PropositionTable::dagNode2Index(propDag);
	if (propId != NONE)	// if the dag has stored
	{
		if (! getInstancePropInfo(propId))	// if the instance relation has NOT been computed
		{
			unique_ptr<InstancePropInfo> ipi(new InstancePropInfo(*propInfoTable[propId]));
			computeMatchingProps(propDag, parentContext, ipi->matchingPropNSubstIds);
			propInfoTable[propId] = std::move(ipi);
		}
		return getInstancePropInfo(propId)->matchingPropNSubstIds.empty() ? NONE : propId;
	}
	else // if the dag has not been stored (but it could have already been computed while not stored)
	{
		map<int,set<int> > temp;
		computeMatchingProps(propDag, parentContext, temp);

		if (! temp.empty())	// if it is an instance
		{
			int newPropId = PropositionTable::cardinality();
			PropositionTable::insert(propDag);
			updatePropTable();

			unique_ptr<InstancePropInfo> ipi(new InstancePropInfo(*propInfoTable[newPropId]));
			ipi->matchingPropNSubstIds = std::move(temp);
			propInfoTable[newPropId] = std::move(ipi);
			return newPropId;
		}
		else
			return NONE;
	}
}


void
ParamPropositionTable::computeMatchingProps(DagNode* propDag, RewritingContext& parentContext, map<int,set<int> >& match)
{
	const map<const Symbol*, Vector<int> >::iterator it = paramPropSymbolMap.find(propDag->symbol());
	if (it != paramPropSymbolMap.end())	// if no corresponding param props
	{
		unique_ptr<RewritingContext> dagCxt(parentContext.makeSubcontext(propDag));
		for (int k : it->second)
		{
			if (const ParamPropInfo* ppi = getParamPropInfo(k))
			{
				ParamInfo& pi = *paramInfoTable[ppi->paramInfoId];
				MatchSearchState mss(dagCxt.get(), pi.pattern.get());

				while (mss.findNextMatch())
				{
					int sIndex = pi.substitutions.size();
					pi.substitutions.emplace_back(new ParamSubstitution(*pi.pattern, *mss.getContext()));
					match[k].insert(sIndex);
				}
			}
		}
	}
}

void
ParamPropositionTable::updatePropInfo(int propId)
{
	PropositionTable::updatePropInfo(propId);	// first fill the basic info

	DagNode* propDag = PropositionTable::index2DagNode(propId);
	if (! propDag->isGround())	// check if ground (variable info must be filled before)
	{
		unsigned int pInfoId;
		if (PropositionTable::isEnabledProp(propId))
		{
			pInfoId = getParamPropInfo(getEnabledEventId(propId))->paramInfoId;
		}
		else
		{
			paramPropSymbolMap[propDag->symbol()].append(propId);	// register a related symbol for the prop (except for enalbed props)
			pInfoId = paramInfoTable.size();
			paramInfoTable.emplace_back(new ParamInfo(TermUtil::constructTerm(propDag)));
		}
		propInfoTable[propId].reset(new ParamPropInfo(*propInfoTable[propId], pInfoId));
	}
}



} /* namespace modelChecker */
