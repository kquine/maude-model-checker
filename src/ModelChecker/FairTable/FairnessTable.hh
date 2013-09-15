/*
 * FairnessTable.hh
 *
 *  Created on: Aug 22, 2013
 *      Author: kquine
 */

#ifndef FAIRNESSTABLE_HH_
#define FAIRNESSTABLE_HH_
#include <set>
#include "bdd.h"
#include "AbstractFairnessTable.hh"
#include "Utility/indexedSet.hh"
#include "PropTable/PropositionTable.hh"

namespace modelChecker {

template <typename Formula> class FairnessTable;

using WeakFairnessTable	= 	FairnessTable<Bdd>;
using StrongFairnessTable = FairnessTable<pair<Bdd,Bdd>>;

template <typename Formula>
class FairnessTable: public AbstractFairnessTable
{
public:
	FairnessTable(PropositionTable& propTable);
	virtual ~FairnessTable() {}

	bool hasStrongFairness() const override;
	int nrFairness() const override				{ return fairTable.size();}
	virtual bool isParamFairness(int) const		{ return false; }

	virtual const Formula& getFairFormula(int fairId) const;
	virtual bool isStateFairness(int fairId) const;
	bool isStateEventFairness(int fairId) const;

	int insertFairnessFormula(const Formula& f, const set<int>& propIds, DagNode* fairDag);

protected:
	struct Fairness    // empty base class
	{
		virtual ~Fairness() {}
	};

	struct GroundFairness: public Fairness
	{
		GroundFairness(int fid, bool noEvent): formulaId(fid), noEvent(noEvent) {}
		virtual ~GroundFairness() {}

		const int formulaId;
		const bool noEvent;
	};

	virtual unique_ptr<GroundFairness> createFormulaFairness(int formulaId, const set<int>& propIds, DagNode* fairDag) const;

	vector<unique_ptr<Fairness>> fairTable;		// fairId |-> fairnessInfo

private:
	vector<int> formulaInfoMap;			// formulaId |-> fair id
	indexed_set<Formula> fairFormulas;	// formulaId |-> fair formula

	PropositionTable& propTable;
};

template <> inline bool
FairnessTable<Bdd>::hasStrongFairness() const
{
	return false;
}

template <> inline bool
FairnessTable<pair<Bdd,Bdd>>::hasStrongFairness() const
{
	return nrFairness() > 0;
}

} /* namespace modelChecker */

#include "FairnessTable.cc"

#endif /* FAIRNESSTABLE_HH_ */
