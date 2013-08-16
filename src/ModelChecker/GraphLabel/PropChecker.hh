/*
 * StatePropChecker
 *
 *  Created on: Dec 3, 2010
 *      Author: kquine
 */

#ifndef STATEPROPCHECKER_HH_
#define STATEPROPCHECKER_HH_

namespace modelChecker {

class PropChecker
{
public:
	PropChecker(Symbol* checkSymbol, DagNode* trueDag, RewritingContext& context);
	bool computeLabel(DagNode* target, DagNode* prop) const;

private:
	RewritingContext& context;
	DagNode* trueDagNode;
	Symbol* checkSymbol;
};

}

#endif /* STATEPROPCHECKER_HH_ */
