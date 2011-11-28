/*
 * TermUtil.cc
 *
 *  Created on: Jul 27, 2011
 *      Author: kquine
 */

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "freeTheory.hh"
#include "S_Theory.hh"
#include "NA_Theory.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "dagArgumentIterator.hh"

// symbols
#include "freeSymbol.hh"
#include "floatSymbol.hh"
#include "S_Symbol.hh"
#include "stringSymbol.hh"
#include "quotedIdentifierSymbol.hh"

// terms
#include "variableTerm.hh"
#include "floatTerm.hh"
#include "S_Term.hh"
#include "stringTerm.hh"
#include "quotedIdentifierTerm.hh"

// dagnodes
#include "freeDagNode.hh"
#include "variableDagNode.hh"
#include "floatDagNode.hh"
#include "S_DagNode.hh"
#include "stringDagNode.hh"
#include "quotedIdentifierDagNode.hh"


#include "TermUtil.hh"

namespace modelChecker {


Term*
TermUtil::constructTerm(DagNode* dag)
{
	Symbol* s = dag->symbol();

	if (VariableSymbol* vs = dynamic_cast<VariableSymbol*>(s))
	{
		VariableDagNode* vdag = safeCast(VariableDagNode*, dag);
		return new VariableTerm(vs, vdag->id());
	}
	else if (FloatSymbol* fs = dynamic_cast<FloatSymbol*>(s))
	{
		FloatDagNode* fdag = safeCast(FloatDagNode*, dag);
		return new FloatTerm(fs, fdag->getValue());
	}
	else if (QuotedIdentifierSymbol* qs = dynamic_cast<QuotedIdentifierSymbol*>(s))
	{
		QuotedIdentifierDagNode* qdag = safeCast(QuotedIdentifierDagNode*, dag);
		return new QuotedIdentifierTerm(qs, qdag->getIdIndex());
	}
	else if (StringSymbol* strs = dynamic_cast<StringSymbol*>(s))
	{
		StringDagNode* sdag = safeCast(StringDagNode*, dag);
		return new StringTerm(strs, sdag->getValue());
	}
	else
	{
		Vector<Term*> argList(0, nrPreallocatedArgs);  // pre-allocate memory for speed
		for (DagArgumentIterator i(*dag); i.valid(); i.next())
			argList.append(constructTerm(i.argument()));

		if (S_DagNode* sdag = dynamic_cast<S_DagNode*>(dag))
			return new S_Term(safeCast(S_Symbol*,s), sdag->getNumber(), argList[0]);
		else
			return  s->makeTerm(argList);
	}
}

}
