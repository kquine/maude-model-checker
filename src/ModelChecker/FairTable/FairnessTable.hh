/*
 * FairnessTable.hh
 *
 *  Created on: Aug 22, 2013
 *      Author: kquine
 */

#ifndef FAIRNESSTABLE_HH_
#define FAIRNESSTABLE_HH_
#include <map>
#include <set>
#include "natSet.hh"
#include "bdd.h"
#include "AbstractFairnessTable.hh"
#include "PropTable/PropositionTable.hh"

namespace modelChecker {

template <typename Formula> class FairnessTable;

using WeakFairnessTable	= 	FairnessTable<Bdd>;
using StrongFairnessTable = FairnessTable<pair<Bdd,Bdd>>;

template <typename Formula>
class FairnessTable: public AbstractFairnessTable
{
public:
	using index_type = AbstractFairnessTable::index_type;

	FairnessTable(PropositionTable& propTable);
	virtual ~FairnessTable() {}

	bool hasStrongFairness() const override;
	virtual bool isParamFairness(index_type) const		{ return false; }

	index_type nrFairness() const override				{ return fairTable.size();}
	const NatSet& getOrigFairnessIds() const			{ return origFairIds; }

	virtual const Formula& getFairFormula(index_type fairId) const;
	virtual bool isStateFairness(index_type fairId) const;
	bool isStateEventFairness(index_type fairId) const;

	index_type insertFairnessFormula(const Formula& f, const vector<index_type>& propIds, DagNode* fairDag);

protected:
	struct Fairness    // empty base class
	{
		virtual ~Fairness() {}
	};

	struct GroundFairness: public Fairness
	{
		GroundFairness(const Formula& f, bool noEvent): formula(f), noEvent(noEvent) {}

		const Formula& formula;
		const bool noEvent;
	};

	virtual unique_ptr<GroundFairness> createFormulaFairness(const Formula& f, const vector<index_type>& propIds, DagNode* fairDag) const;

	vector<unique_ptr<Fairness>> fairTable;		// fairId |-> fairnessInfo

private:
	NatSet origFairIds;							// a set of original (ground or param) fair ids
	map<Formula,index_type> fairFormulas;		// fair formula |-> fair id

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
