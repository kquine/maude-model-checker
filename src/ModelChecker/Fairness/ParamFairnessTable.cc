/*
 * ParamFairnessTable.cc
 *
 *  Created on: Sep 3, 2013
 *      Author: kquine
 */

// utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "temporal.hh"
#include "interface.hh"
#include "core.hh"

// interface class definitions
#include "symbol.hh"
#include "DagNodeSet.hh"
#include "term.hh"

// core class definitions
#include "rewritingContext.hh"
#include "dagArgumentIterator.hh"

// temporal class definitions
#include "logicFormula.hh"

// ltlr definitions
#include "ParamFairnessTable.hh"

namespace modelChecker {

ParamFairnessTable::ParamFairnessTable(PropositionTable& propTable, const FairnessDecoder& fDecoder, const FormulaBuilder& fBuilder):
		FairnessTable(propTable,fDecoder,fBuilder) {}

//TODO: if newly identified:
//  create param variable info..
//  if there is a variable (i.e., not ground), create the builder

//int
//FairnessTable::insertAux(DagNode* fairnessDag, const FairnessFormula& f, vector<int>& typeTable)
//{
//	int oldSize = fairFormulaTable.size();
//	int fi = fairFormulaTable.insert(f);
//	if (fi >= oldSize)	// new index identified
//	{
//		typeTable.push_back(fi);
//		varInfoTable.emplace_back(new ParamVariableInfo());
//		varInfoTable.back()->indexVariables(fairnessDag);
//	}
//	propTable.updatePropTable();
//	return fi;
//}

} /* namespace modelChecker */
