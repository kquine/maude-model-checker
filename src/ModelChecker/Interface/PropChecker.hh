/*
 * PropChecker
 *
 *  Created on: Dec 3, 2010
 *      Author: kquine
 */

#ifndef PROPCHECKER_HH_
#define PROPCHECKER_HH_

namespace modelChecker {

class PropChecker
{
public:
	PropChecker(RewritingContext* context, Symbol* checkSymbol, DagNode* trueDag);
	bool computeLabel(DagNode* target, DagNode* prop) const;
private:
	RewritingContext* context;
	DagNode* trueDagNode;
	Symbol* checkSymbol;
};

}

#endif /* PROPCHECKER_HH_ */
