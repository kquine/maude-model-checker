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
	unsigned int nrFairness() const override				{ return fairTable.size();}
	virtual bool isParamFairness(unsigned int) const		{ return false; }

	virtual const Formula& getFairFormula(unsigned int fairId) const;
	virtual bool isStateFairness(unsigned int fairId) const;
	bool isStateEventFairness(unsigned int fairId) const;

	unsigned int insertFairnessFormula(const Formula& f, const set<unsigned int>& propIds, DagNode* fairDag);

protected:
	struct Fairness    // empty base class
	{
		virtual ~Fairness() {}
	};

	struct GroundFairness: public Fairness
	{
		GroundFairness(unsigned int fid, bool noEvent): formulaId(fid), noEvent(noEvent) {}
		virtual ~GroundFairness() {}

		const unsigned int formulaId;
		const bool noEvent;
	};

	virtual unique_ptr<GroundFairness> createFormulaFairness(unsigned int formulaId, const set<unsigned int>& propIds, DagNode* fairDag) const;

	vector<unique_ptr<Fairness>> fairTable;		// fairId |-> fairnessInfo

private:
	vector<unsigned int> formulaInfoMap;	// formulaId |-> fair id
	indexed_set<Formula> fairFormulas;		// formulaId |-> fair formula

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
