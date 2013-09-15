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
public:
	ParamFairnessTable(PropositionTable& propTable);

	bool isParamFairness(int fairId) const override;
	const Formula& getFairFormula(int fairId) const override;
	bool isStateFairness(int fairId) const override;

	const ParamSubstitutionBuilder& getParamSubstBuilder(int fairId) const;
	int insertFairnessInstance(int paramFairId, const map<int,int>& propSubst);		// propSubst = "paramPropId |-> a substitution id in ParamPropTable"

private:
	struct ParamFairness: public GroundFairness
	{
		ParamFairness(const GroundFairness& fi, DagNode* fDag, const set<int>& propIds, const ParamPropositionTable& pTable): GroundFairness(fi), builder(fDag,propIds,pTable) {}
		ParamSubstitutionBuilder builder;

		indexed_set<map<int,int>> substs;	// subst id |-> a realized substitution
		vector<int> instanceId;				// subst id |-> fair id
	};

	struct InstanceFairnessInfo: public Fairness
	{
		InstanceFairnessInfo(int pfi, int si): paramFairId(pfi), substId(si) {}

		const int paramFairId;			// the base param fair id
		const int substId;				// the corresponding substitution id
		set<int> directBase;			// the set of immediate instance fair ids (in fairTable)
	};

	unique_ptr<GroundFairness> createFormulaFairness(int formulaId, const set<int>& propIds, DagNode* fairDag) const override;

	int getBaseFairId(int fairId) const;
	void updateInstanceBaseMap(const ParamFairness& pfi);	// update the dependency map when the last realized substitution is newly added

	bool subsumed(const map<int,int>& s1, const map<int,int>& s2) const;

	vector<unique_ptr<InstanceFairnessInfo>> instanceFairnessInfo;
	const ParamPropositionTable& paramPropTableRef;
};

} /* namespace modelChecker */

#include "ParamFairnessTable.cc"

#endif /* PARAMFAIRNESSTABLE_HH_ */
