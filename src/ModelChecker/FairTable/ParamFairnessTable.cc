/*
 * ParamFairnessTable.cc
 *
 *  Created on: Sep 3, 2013
 *      Author: kquine
 */

// utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "temporal.hh"
#include "interface.hh"
#include "core.hh"

// ltlr definitions
#include "ParamFairnessTable.hh"

namespace modelChecker {

template <typename Formula>
ParamFairnessTable<Formula>::ParamFairnessTable(PropositionTable& propTable):
	FairnessTable<Formula>(propTable), paramPropTableRef(static_cast<const ParamPropositionTable&>(propTable)) {}


template <typename Formula> bool
ParamFairnessTable<Formula>::isParamFairness(int fairId) const
{
	return dynamic_cast<ParamFairness*>(Super::fairTable[fairId].get());
}

template <typename Formula> const Formula&
ParamFairnessTable<Formula>::getFairFormula(int fairId) const
{
	return Super::getFairFormula(getBaseFairId(fairId));
}

template <typename Formula> bool
ParamFairnessTable<Formula>::isStateFairness(int fairId) const
{
	return Super::isStateFairness(getBaseFairId(fairId));
}

template <typename Formula> const ParamSubstitutionBuilder&
ParamFairnessTable<Formula>::getParamSubstBuilder(int fairId) const
{
	return static_cast<ParamFairness&>(*Super::fairTable[fairId]).builder;
}

template <typename Formula> int
ParamFairnessTable<Formula>::insertFairnessInstance(int paramFairId, const map<int,int>& propSubst)
{
	ParamFairness& pfi = static_cast<ParamFairness&>(*Super::fairTable[paramFairId]);
	int oldSize = pfi.substs.size();
	int si = pfi.substs.insert(propSubst);
	if (si >= oldSize)
	{
		pfi.instanceId.push_back(Super::fairTable.size());
		Super::fairTable.emplace_back(new InstanceFairnessInfo(paramFairId,si));
		updateInstanceBaseMap(pfi);
	}
	return pfi.instanceId[si];
}

template <typename Formula>
unique_ptr<typename ParamFairnessTable<Formula>::GroundFairness>
ParamFairnessTable<Formula>::createFormulaFairness(int formulaId, const set<int>& propIds, DagNode* fairDag) const
{
	unique_ptr<GroundFairness> gfi = Super::createFormulaFairness(formulaId, propIds, fairDag);
	return fairDag->isGround() ? move(gfi) : unique_ptr<ParamFairness>(new ParamFairness(*gfi,fairDag,propIds,paramPropTableRef));
}


template <typename Formula> inline int
ParamFairnessTable<Formula>::getBaseFairId(int fairId) const
{
	if (InstanceFairnessInfo* ifi = dynamic_cast<InstanceFairnessInfo*>(Super::fairTable[fairId].get()))
		return ifi->paramFairId;
	else
		return fairId;
}

template <typename Formula> void
ParamFairnessTable<Formula>::updateInstanceBaseMap(const ParamFairness& pfi)
{
	//int last = pfi.substs.size() - 1;

	//TODO:
}

template <typename Formula> bool
ParamFairnessTable<Formula>::subsumed(const map<int,int>& s1, const map<int,int>& s2) const
{
	//TODO:
	return false;
}

} /* namespace modelChecker */
