/*
 * SubstConstructor.cc
 *
 *  Created on: Jul 16, 2012
 *      Author: kquine
 */

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"
#include "mixfix.hh"
#include "strategyLanguage.hh"

//      core class definitions
#include "rewritingContext.hh"
#include "rule.hh"
#include "positionState.hh"
#include "symbolMap.hh"

//		built in class definitions
#include "bindingMacros.hh"

#include "quotedIdentifierSymbol.hh"
#include "quotedIdentifierDagNode.hh"
#include "mixfixModule.hh"

#include "SubstConstructor.hh"

namespace modelChecker {

SubstConstructor::SubstConstructor(Symbol* assignmentSymbol, Symbol* substitutionSymbol,
			Symbol* emptySubstSymbol, QuotedIdentifierSymbol* qidSymbol):
		assignmentSymbol(assignmentSymbol), substitutionSymbol(substitutionSymbol),
		emptySubstSymbol(emptySubstSymbol), qidSymbol(qidSymbol)
{
}

DagNode*
SubstConstructor::makeSubstDag(MixfixModule* module, const Substitution* substitution, const VariableInfo* variableInfo)
{
	if (substitution != NULL && variableInfo != NULL)
	{
		int nrVariable = variableInfo->getNrRealVariables();
		if (nrVariable > 0)
		{
			Vector<DagNode*> args;  // size may vary
			static Vector<ConnectedComponent*> domain(2);
			domain[0] = qidSymbol->rangeComponent();

			for (int i = 0; i < nrVariable ; i++)
			{
				domain[1] = variableInfo->index2Variable(i)->getComponent();
				Symbol* assignSymbol = module->findSymbol(assignmentSymbol->id(), domain, assignmentSymbol->rangeComponent());

				if (assignSymbol != NULL)
					args.append(makeAssignmentDag(variableInfo->index2Variable(i), substitution->value(i), assignSymbol));
			}
			if (args.size() > 1)
				return substitutionSymbol->makeDagNode(args);
			else if (args.size() == 1)
				return args[0];
		}
	}
    return emptySubstSymbol->makeDagNode();
}

DagNode*
SubstConstructor::makeAssignmentDag(const Term* variable, DagNode* value, Symbol* assignSymbol) const
{
    static Vector<DagNode*> args(2);
    args[0] = new QuotedIdentifierDagNode(qidSymbol, safeCast(const VariableTerm*,variable)->id());
    args[1] = value;
    return assignSymbol->makeDagNode(args);
}

} /* namespace modelChecker */
