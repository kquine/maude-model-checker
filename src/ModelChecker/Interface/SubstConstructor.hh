/*
 * SubstitutionConstructor.hh
 *
 *  Created on: Jul 16, 2012
 *      Author: kquine
 */

#ifndef SUBSTCONSTRUCTOR_HH_
#define SUBSTCONSTRUCTOR_HH_

namespace modelChecker {

class SubstConstructor {
public:
	SubstConstructor(Symbol* assignmentSymbol, Symbol* substitutionSymbol,
			Symbol* emptySubstSymbol, QuotedIdentifierSymbol* qidSymbol);

	DagNode* makeSubstDag(MixfixModule* module, const Substitution* substitution, const VariableInfo* variableInfoconst);

private:
	DagNode* makeAssignmentDag(const Term* variable, DagNode* value, Symbol* assignSymbol) const;

	Symbol* assignmentSymbol;	// 2 arguments, the first polymorphic
	Symbol* substitutionSymbol;
	Symbol* emptySubstSymbol;
	QuotedIdentifierSymbol* qidSymbol;
};

} /* namespace modelChecker */
#endif /* SUBSTCONSTRUCTOR_HH_ */
