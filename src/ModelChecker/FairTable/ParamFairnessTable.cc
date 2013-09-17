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
ParamFairnessTable<Formula>::isParamFairness(unsigned int fairId) const
{
	return dynamic_cast<ParamFairness*>(Super::fairTable[fairId].get());
}

template <typename Formula> const Formula&
ParamFairnessTable<Formula>::getFairFormula(unsigned int fairId) const
{
	return Super::getFairFormula(getBaseFairId(fairId));
}

template <typename Formula> bool
ParamFairnessTable<Formula>::isStateFairness(unsigned int fairId) const
{
	return Super::isStateFairness(getBaseFairId(fairId));
}

template <typename Formula> const ParamSubstitutionBuilder&
ParamFairnessTable<Formula>::getParamSubstBuilder(unsigned int fairId) const
{
	return static_cast<ParamFairness&>(*Super::fairTable[fairId]).builder;
}

template <typename Formula> unsigned int
ParamFairnessTable<Formula>::insertFairnessInstance(unsigned int paramFairId, const ParamSubstitution& propSubst)
{
	ParamFairness& pfi = static_cast<ParamFairness&>(*Super::fairTable[paramFairId]);
	auto oldSize = pfi.substs.size();
	auto si = pfi.substs.insert(propSubst);
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
ParamFairnessTable<Formula>::createFormulaFairness(unsigned int formulaId, const set<unsigned int>& propIds, DagNode* fairDag) const
{
	unique_ptr<GroundFairness> gfi = Super::createFormulaFairness(formulaId, propIds, fairDag);
	return fairDag->isGround() ? move(gfi) : unique_ptr<ParamFairness>(new ParamFairness(*gfi,fairDag,propIds,paramPropTableRef));
}


template <typename Formula> inline unsigned int
ParamFairnessTable<Formula>::getBaseFairId(unsigned int fairId) const
{
	if (InstanceFairnessInfo* ifi = dynamic_cast<InstanceFairnessInfo*>(Super::fairTable[fairId].get()))
		return ifi->paramFairId;
	else
		return fairId;
}

template <typename Formula> void
ParamFairnessTable<Formula>::updateInstanceBaseMap(const ParamFairness& /* pfi */)
{
	//unsigned int last = pfi.substs.size() - 1;

	//TODO:
}

template <typename Formula> bool
ParamFairnessTable<Formula>::subsumed(const vector<const ParamSubstitution*>& s1, const vector<const ParamSubstitution*>& s2) const
{
	//TODO:
	return false;
}

} /* namespace modelChecker */
