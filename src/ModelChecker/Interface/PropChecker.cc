/*
 * LabelSymbolChecker.cc
 *
 *  Created on: Dec 3, 2010
 *      Author: kquine
 */

//      utility stuff
#include <memory>
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"

//      core class definitions
#include "rewritingContext.hh"
#include "symbolMap.hh"

//		built in class definitions
#include "bindingMacros.hh"

#include "PropChecker.hh"

//#define TDEBUG

namespace modelChecker {


PropChecker::PropChecker(RewritingContext* context, Symbol* checkSymbol, DagNode* trueDag):
		context(context), checkSymbol(checkSymbol), trueDagNode(trueDag) {}

bool
PropChecker::computeLabel(DagNode* target, DagNode* prop) const
{
	static Vector<DagNode*> args(2);
	args[0] = target;
	args[1] = prop;
	DagNode* checkDag = checkSymbol->makeDagNode(args);
#ifdef TDEBUG
	cout << checkDag << " is reduced to ";
#endif
	const auto_ptr<RewritingContext> testContext(context->makeSubcontext(checkDag));
	testContext->reduce();
	bool result = trueDagNode->equal(testContext->root());
#ifdef TDEBUG
	cout << (result ? "true" : "false") << endl;
#endif
	context->addInCount(*testContext);
	return result;
}

}
