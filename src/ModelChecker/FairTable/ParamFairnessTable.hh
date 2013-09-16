/*
 * ParamFairnessTable.hh
 *
 *  Created on: Sep 3, 2013
 *      Author: kquine
 */

#ifndef PARAMFAIRNESSTABLE_HH_
#define PARAMFAIRNESSTABLE_HH_
#include "ParamSubstitutionBuilder.hh"
#include "FairnessTable.hh"

namespace modelChecker {

template <typename Formula> class ParamFairnessTable;

using ParamWeakFairnessTable =		ParamFairnessTable<Bdd>;
using ParamStrongFairnessTable =	ParamFairnessTable<pair<Bdd,Bdd>>;

template <typename Formula>
class ParamFairnessTable: public FairnessTable<Formula>
{
	using Super = 			FairnessTable<Formula>;
	using Fairness =		typename Super::Fairness;
	using GroundFairness =	typename Super::GroundFairness;
	using RealizedSubst =	ParamSubstitutionBuilder::RealizedSubst;
public:
	ParamFairnessTable(PropositionTable& propTable);

	bool isParamFairness(unsigned int fairId) const override;
	const Formula& getFairFormula(unsigned int fairId) const override;
	bool isStateFairness(unsigned int fairId) const override;

	const ParamSubstitutionBuilder& getParamSubstBuilder(unsigned int fairId) const;
	unsigned int insertFairnessInstance(unsigned int paramFairId, const RealizedSubst& propSubst);		// propSubst = "paramPropId |-> a substitution id in ParamPropTable"

private:
	struct ParamFairness: public GroundFairness
	{
		ParamFairness(const GroundFairness& fi, DagNode* fDag, const set<unsigned int>& propIds, const ParamPropositionTable& pTable): GroundFairness(fi), builder(fDag,propIds,pTable) {}
		ParamSubstitutionBuilder builder;

		indexed_set<RealizedSubst> substs;	// subst id |-> a realized substitution
		vector<unsigned int> instanceId;	// subst id |-> fair id
	};

	struct InstanceFairnessInfo: public Fairness
	{
		InstanceFairnessInfo(unsigned int pfi, unsigned int si): paramFairId(pfi), substId(si) {}

		const unsigned int paramFairId;			// the base param fair id
		const unsigned int substId;				// the corresponding substitution id
		set<unsigned int> directBase;			// the set of immediate instance fair ids (in fairTable)
	};

	unique_ptr<GroundFairness> createFormulaFairness(unsigned int formulaId, const set<unsigned int>& propIds, DagNode* fairDag) const override;

	unsigned int getBaseFairId(unsigned int fairId) const;
	void updateInstanceBaseMap(const ParamFairness& pfi);	// update the dependency map when the last realized substitution is newly added

	bool subsumed(const RealizedSubst& s1, const RealizedSubst& s2) const;

	vector<unique_ptr<InstanceFairnessInfo>> instanceFairnessInfo;
	const ParamPropositionTable& paramPropTableRef;
};

} /* namespace modelChecker */

#include "ParamFairnessTable.cc"

#endif /* PARAMFAIRNESSTABLE_HH_ */
