/*
 * FoldingChecker.hh
 *
 *  Created on: Aug 7, 2011
 *      Author: kquine
 */

#ifndef FOLDINGCHECKER_HH_
#define FOLDINGCHECKER_HH_

namespace modelChecker {

class FoldingChecker
{
public:
	FoldingChecker(Symbol* foldingRelSymbol, DagNode* trueDag);
	bool fold(DagNode* s, DagNode* t, RewritingContext* context) const;

private:
    DagNode* trueDag;
	Symbol* foldingRelSymbol;
};

}

#endif /* FOLDINGCHECKER_HH_ */
