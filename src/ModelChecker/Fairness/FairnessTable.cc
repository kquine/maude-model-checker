/*
 * FairnessTable.cc
 *
 *  Created on: Aug 22, 2013
 *      Author: kquine
 */

// utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "temporal.hh"
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"

// interface class definitions
#include "symbol.hh"
#include "DagNodeSet.hh"
#include "term.hh"

// core class definitions
#include "rewritingContext.hh"
#include "dagArgumentIterator.hh"

// temporal class definitions
#include "logicFormula.hh"

//      free theory class definitions
#include "freeDagNode.hh"


// ltlr definitions
#include "FairnessTable.hh"

namespace modelChecker {

FairnessTable::FairnessTable(PropositionTable& propTable, const FairnessDecoder& fDecoder, const FormulaBuilder& fBuilder):
		propTable(propTable), fDecoder(fDecoder), fBuilder(fBuilder) {}

int
FairnessTable::insertFairnessDag(DagNode* fairnessDag)
{
	set<int> propIds;
	pair<Bdd,Bdd> ff = interpretFairnessDag(fairnessDag,propIds);

	int fIndex = NONE;
	if (ff.first == bdd_true())
		fIndex = weakFairTable.insert(ff.second);	// weak fairness if the premise is true
	else
		fIndex = strongFairTable.insert(ff);

	propTable.updatePropTable();
	return fIndex;
}

pair<Bdd,Bdd>
FairnessTable::interpretFairnessDag(DagNode* fairnessDag, set<int>& propIds)
{
	FairnessDecoder::Fairness ff = fDecoder.decodeFairnessDag(fairnessDag);

	LogicFormula premF;
	Bdd prem = fBuilder.translateFairnessFormula(fBuilder.build(premF, propTable.getDagNodeSet(), ff.prem), premF, propIds);

	LogicFormula consF;
	Bdd cons = fBuilder.translateFairnessFormula(fBuilder.build(consF, propTable.getDagNodeSet(), ff.cons), consF, propIds);

	return ff.type == FairnessDecoder::WEAK_FAIRNESS ? make_pair<Bdd,Bdd>(bdd_true(), prem >> cons) : make_pair(prem, cons);
}

int
FairnessTable::createFairnessInfo(const set<int>& propIds)
{

}


//int
//FairnessTable::insertAux(DagNode* fairnessDag, const FairnessFormula& f, vector<int>& typeTable)
//{
//	int oldSize = fairFormulaTable.size();
//	int fi = fairFormulaTable.insert(f);
//	if (fi >= oldSize)	// new index identified
//	{
//		typeTable.push_back(fi);
//	}
//	propTable.updatePropTable();
//	return fi;
//}

} /* namespace modelChecker */
