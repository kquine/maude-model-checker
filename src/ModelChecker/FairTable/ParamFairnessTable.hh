/*
 * ParamFairnessTable.hh
 *
 *  Created on: Sep 3, 2013
 *      Author: kquine
 */

#ifndef PARAMFAIRNESSTABLE_HH_
#define PARAMFAIRNESSTABLE_HH_
#include "FairnessTable.hh"
#include "RealizedFairnessTable.hh"

namespace modelChecker {

template <typename Formula> class ParamFairnessTable;

using ParamWeakFairnessTable =		ParamFairnessTable<Bdd>;
using ParamStrongFairnessTable =	ParamFairnessTable<pair<Bdd,Bdd>>;

template <typename Formula>
class ParamFairnessTable: public FairnessTable<Formula>, public RealizedFairnessTable
{
	using Super 			= FairnessTable<Formula>;
	using Fairness 			= typename Super::Fairness;
	using GroundFairness 	= typename Super::GroundFairness;
public:
	using index_type 		= AbstractFairnessTable::index_type;

	ParamFairnessTable(ParamPropositionTable& propTable);

	bool isParamFairness(index_type fairId) const override;
	bool isStateFairness(index_type fairId) const override;

	const Formula& getFairFormula(index_type fairId) const override;

private:
	struct ParamFairness: public GroundFairness, public RealizedFairnessTable::ParamInfo
	{
		ParamFairness(const GroundFairness& fi, DagNode* fDag, const vector<index_type>& propIds, const ParamPropositionTable& pTable):
			GroundFairness(fi), ParamInfo(fDag,propIds,pTable) {}
	};
	struct InstanceFairnessInfo: public Fairness, public RealizedFairnessTable::InstanceInfo
	{
		InstanceFairnessInfo(index_type pfi, const ParamSubstitution* s): InstanceInfo(pfi, s) {}
	};

	unique_ptr<GroundFairness> createFormulaFairness(const Formula& f, const vector<index_type>& propIds, DagNode* fairDag) const override;

	index_type getNextFairIndex() const;
	ParamInfo& getParamInfo(index_type fairId) const override;
	InstanceInfo& getInstanceInfo(index_type fairId) const override;
	void insertInstance(index_type pfi, const ParamSubstitution* s);

	//TEST
	DagNode* getDag(index_type fi) const { return Super::dags[fi]; }

	index_type getBaseFairId(index_type fairId) const;

	const ParamPropositionTable& paramPropTableRef;
};

} /* namespace modelChecker */

#include "ParamFairnessTable.cc"

#endif /* PARAMFAIRNESSTABLE_HH_ */
