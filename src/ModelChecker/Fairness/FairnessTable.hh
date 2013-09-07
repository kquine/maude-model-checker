/*
 * FairnessTable.hh
 *
 *  Created on: Aug 22, 2013
 *      Author: kquine
 */

#ifndef FAIRNESSTABLE_HH_
#define FAIRNESSTABLE_HH_
#include "bdd.h"
#include "Utility/indexedSet.hh"
#include "Interface/FairnessDecoder.hh"
#include "Formula/PropositionTable.hh"
#include "Formula/FormulaBuilder.hh"

namespace modelChecker {

class FairnessTable
{
	typedef FairnessDecoder::FairnessType	FairnessType;
public:
	struct WeakFairnessFormula;
	struct StrongFairnessFormula;

	FairnessTable(PropositionTable& propTable, const FairnessDecoder& fDecoder, const FormulaBuilder& fBuilder);
	virtual ~FairnessTable() {}

	int nrWeakFairness() const									{ return weakFairTable.size(); }
	Bdd getWeakFairFormula(int weakFairId) const				{ return weakFairTable[weakFairId]; }
	bool isStateWeakFairness(int weakFairId) const				{ return weakFairInfoTable[weakFairId]->noEvent; }
	bool isStateEventWeakFairness(int weakFairId) const			{ return ! isStateWeakFairness(weakFairId); }
	virtual bool isParamWeakFairness(int) const					{ return false; }

	int nrStrongFairness() const								{ return strongFairTable.size(); }
	pair<Bdd,Bdd> getStrongFairFormula(int strongFairId) const	{ return strongFairTable[strongFairId]; }
	bool isStateStrongFairness(int strongFairId) const			{ return strongFairInfoTable[strongFairId]->noEvent; }
	bool isStateEventStrongFairness(int strongFairId) const		{ return ! isStateStrongFairness(strongFairId); }
	virtual bool isParamStrongFairness(int) const				{ return false; }

	int insertFairnessDag(DagNode* fairnessDag);

protected:
	struct FairnessInfo
	{
		bool noEvent;
	};

	virtual int createFairnessInfo(const set<int>& propIds);	// update fInfoTable and return the corresponding ids;

	vector<unique_ptr<FairnessInfo>> weakFairInfoTable;		// wFairId |-> fairnessInfo
	vector<unique_ptr<FairnessInfo>> strongFairInfoTable;	// sFairId |-> fairnessInfo
private:
	pair<Bdd,Bdd> interpretFairnessDag(DagNode* fairnessDag, set<int>& propIds);

	indexed_set<Bdd> weakFairTable;				// wFairId |-> weak-fair formula
	indexed_set<pair<Bdd,Bdd>> strongFairTable;	// sFairId |-> strong-fair formula

	PropositionTable& propTable;
	const FairnessDecoder& fDecoder;
	const FormulaBuilder& fBuilder;
};



} /* namespace modelChecker */
#endif /* FAIRNESSTABLE_HH_ */
