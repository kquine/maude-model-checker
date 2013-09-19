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
ParamFairnessTable<Formula>::ParamFairnessTable(ParamPropositionTable& propTable):
	FairnessTable<Formula>(propTable), paramPropTableRef(propTable) {}


template <typename Formula> inline bool
ParamFairnessTable<Formula>::isParamFairness(index_type fairId) const
{
	return dynamic_cast<ParamFairness*>(Super::fairTable[fairId].get());
}

template <typename Formula> inline bool
ParamFairnessTable<Formula>::isStateFairness(index_type fairId) const
{
	return Super::isStateFairness(getBaseFairId(fairId));
}

template <typename Formula> inline const Formula&
ParamFairnessTable<Formula>::getFairFormula(index_type fairId) const
{
	return Super::getFairFormula(getBaseFairId(fairId));
}

template <typename Formula>
unique_ptr<typename ParamFairnessTable<Formula>::GroundFairness>
ParamFairnessTable<Formula>::createFormulaFairness(const Formula& f, const vector<index_type>& propIds, DagNode* fairDag) const
{
	unique_ptr<GroundFairness> gfi = Super::createFormulaFairness(f, propIds, fairDag);
	return fairDag->isGround() ? move(gfi) : unique_ptr<ParamFairness>(new ParamFairness(*gfi,fairDag,propIds,paramPropTableRef));
}

template <typename Formula>
typename ParamFairnessTable<Formula>::index_type
ParamFairnessTable<Formula>::getNextFairIndex() const
{
	return Super::fairTable.size();
}

template <typename Formula>
typename ParamFairnessTable<Formula>::ParamInfo&
ParamFairnessTable<Formula>::getParamInfo(index_type fairId) const
{
	return static_cast<ParamFairness&>(*Super::fairTable[fairId]);
}

template <typename Formula>
typename ParamFairnessTable<Formula>::InstanceInfo&
ParamFairnessTable<Formula>::getInstanceInfo(index_type fairId) const
{
	return static_cast<InstanceFairnessInfo&>(*Super::fairTable[fairId]);
}

template <typename Formula> void
ParamFairnessTable<Formula>::insertInstance(index_type pfi, const ParamSubstitution* s)
{
	Super::fairTable.emplace_back(new InstanceFairnessInfo(pfi, s));
}

template <typename Formula>
inline typename ParamFairnessTable<Formula>::index_type
ParamFairnessTable<Formula>::getBaseFairId(index_type fairId) const
{
	if (InstanceFairnessInfo* ifi = dynamic_cast<InstanceFairnessInfo*>(Super::fairTable[fairId].get()))
		return ifi->paramFairId;
	else
		return fairId;
}



} /* namespace modelChecker */
