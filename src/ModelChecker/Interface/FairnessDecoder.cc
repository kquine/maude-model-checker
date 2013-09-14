/*
 * FairnessDecoder.cc
 *
 *  Created on: Dec 4, 2010
 *      Author: kquine
 */

//      utility stuff
#include <stdexcept>
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "dagArgumentIterator.hh"

//		temporal class definitions
#include "logicFormula.hh"

//      free theory class definitions
#include "freeDagNode.hh"

// ltlr definitions
#include "Utility/StringStream.hh"
#include "FairTable/ParamFairnessTable.hh"
#include "FairTable/CompositeFairnessTable.hh"
#include "FairnessDecoder.hh"


namespace modelChecker {

FairnessDecoder::FairnessDecoder(const FormulaBuilder& fBuilder, PropositionTable& propTable, Symbol* fairnessSymbol,
		Symbol* strongFairTypeSymbol, Symbol* weakFairTypeSymbol, Symbol* fairnessSetSymbol, Symbol* emptyFairnessSetSymbol):
				fBuilder(fBuilder), propTable(propTable), fairnessSymbol(fairnessSymbol),
				strongFairTypeSymbol(strongFairTypeSymbol), weakFairTypeSymbol(weakFairTypeSymbol),
				fairnessSetSymbol(fairnessSetSymbol), emptyFairnessSetSymbol(emptyFairnessSetSymbol) {}

unique_ptr<AbstractFairnessTable>
FairnessDecoder::interpretFairnessSet(DagNode* fairSetDag) const
{
	vector<DagNode*> fairDags = parseFairnessSet(fairSetDag);
	if (! fairDags.empty())
	{
		vector<tuple<set<int>,bdd,bdd>> fairConds(fairDags.size());

		vector<int> weakIds;	bool weakParam = false;
		vector<int> strongIds;	bool strongParam = false;

		for (unsigned int i = 0; i < fairDags.size(); ++i)
		{
			fairConds[i] = interpretFairnessDag(fairDags[i]);
			if (get<1>(fairConds[i]) == bdd_true())
			{
				weakParam |= (! fairDags[i]->isGround());
				weakIds.push_back(i);
			}
			else
			{
				strongParam |= (! fairDags[i]->isGround());
				strongIds.push_back(i);
			}
		}

		unique_ptr<WeakFairnessTable>	wft(weakParam	? new ParamWeakFairnessTable(propTable)	  : new WeakFairnessTable(propTable));
		unique_ptr<StrongFairnessTable> sft(strongParam ? new ParamStrongFairnessTable(propTable) : new StrongFairnessTable(propTable));

		for (int i : weakIds)	wft->insertFairnessFormula(get<2>(fairConds[i]), get<0>(fairConds[i]), fairDags[i]);
		for (int i : strongIds)	sft->insertFairnessFormula(make_pair(get<1>(fairConds[i]),get<2>(fairConds[i])), get<0>(fairConds[i]), fairDags[i]);

		cout << "weak : " << wft->nrFairness() << ", strong : " << sft->nrFairness() << endl;

		if (wft->nrFairness() > 0 && sft->nrFairness() > 0)
		{
			CompositeFairnessTable* cft = new CompositeFairnessTable;
			cft->addComponent(move(wft));	cft->addComponent(move(sft));
			return unique_ptr<AbstractFairnessTable>(cft);
		}
		if (wft->nrFairness() > 0)
			return unique_ptr<AbstractFairnessTable>(move(wft));
		if (sft->nrFairness() > 0)
			return unique_ptr<AbstractFairnessTable>(move(sft));
	}
	return nullptr;
}

vector<DagNode*>
FairnessDecoder::parseFairnessSet(DagNode* fairSetDag) const
{
	vector<DagNode*> result;
	if (fairSetDag->symbol() == fairnessSymbol)
	{
		result.push_back(fairSetDag);
	}
	else if (fairSetDag->symbol() == fairnessSetSymbol)
	{
		for (DagArgumentIterator fi(fairSetDag); fi.valid(); fi.next())
			result.push_back(fi.argument());
	}
	else
	{
		if (fairSetDag->symbol() != emptyFairnessSetSymbol)
			throw invalid_argument(StringStream() << "fairness condition " << QUOTE(fairSetDag) << " did not reduce to a valid term.");
	}
	return result;
}

tuple<set<int>,bdd,bdd>
FairnessDecoder::interpretFairnessDag(DagNode* fairnessDag) const
{
	Assert(fairnessDag->symbol() == fairnessSymbol, "FairnessDecoder::interpretFairnessDag: invalid fairness formula");
	FreeDagNode* d = safeCast(FreeDagNode*, fairnessDag);
	LogicFormula premF, consF;

	set<int> propIds;
	bdd prem = fBuilder.translateFairnessFormula(fBuilder.build(premF, propTable.getDagNodeSet(), d->getArgument(1)), premF, propIds);
	bdd cons = fBuilder.translateFairnessFormula(fBuilder.build(consF, propTable.getDagNodeSet(), d->getArgument(2)), consF, propIds);
	propTable.updatePropTable();

	return d->getArgument(0)->symbol() == weakFairTypeSymbol ? make_tuple(propIds,bdd_true(),prem>>cons) : make_tuple(propIds,prem,cons);
}

}
