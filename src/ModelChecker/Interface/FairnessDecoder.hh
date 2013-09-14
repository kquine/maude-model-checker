/*
 * FairnessDecoder.hh
 *
 *  Created on: Dec 4, 2010
 *      Author: kquine
 */

#ifndef FAIRNESSDECODER_HH_
#define FAIRNESSDECODER_HH_
#include "Interface/FormulaBuilder.hh"
#include "PropTable/PropositionTable.hh"
#include "FairTable/FairnessTable.hh"

namespace modelChecker {

class FairnessDecoder
{
	typedef FairnessTable<Bdd>					WeakFairnessTable;
	typedef FairnessTable<pair<Bdd,Bdd>>		StrongFairnessTable;
	typedef ParamFairnessTable<Bdd>				ParamWeakFairnessTable;
	typedef ParamFairnessTable<pair<Bdd,Bdd>>	ParamStrongFairnessTable;
public:
	FairnessDecoder(const FormulaBuilder& fBuilder, PropositionTable& propTable,
			Symbol* fairnessSymbol, Symbol* strongFairTypeSymbol, Symbol* weakFairTypeSymbol, Symbol* fairnessSetSymbol, Symbol* emptyFairnessSetSymbol);

	unique_ptr<AbstractFairnessTable> interpretFairnessSet(DagNode* fairSetDag) const;

private:
	vector<DagNode*> parseFairnessSet(DagNode* fairSetDag) const;
	tuple<set<int>,bdd,bdd> interpretFairnessDag(DagNode* fairnessDag) const;		// formula (premise -> conclusion), a set of propIds, and a hasParam flag

	const FormulaBuilder& fBuilder;
	PropositionTable& propTable;

    Symbol* fairnessSymbol;
    Symbol* strongFairTypeSymbol;
    Symbol* weakFairTypeSymbol;
    Symbol* fairnessSetSymbol;
    Symbol* emptyFairnessSetSymbol;
};

}

#endif /* FAIRNESSDECODER_HH_ */
