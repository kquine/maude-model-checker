/*
 * StateFoldingChecker.hh
 *
 *  Created on: Aug 7, 2011
 *      Author: kquine
 */

#ifndef STATEFOLDINGCHECKER_HH_
#define STATEFOLDINGCHECKER_HH_

namespace modelChecker {

class StateFoldingChecker
{
public:
	StateFoldingChecker();
	void setTrueDag(DagNode* trueDag);
	bool fold(DagNode* s, DagNode* t, RewritingContext* context) const;

protected:
    bool attachSymbol(const char* purpose, Symbol* symbol);
    void copyAttachments(StateFoldingChecker* original, SymbolMap* map);
    void getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols);

private:
    DagNode* trueDag;
	Symbol* foldingRelSymbol;
};

inline void
StateFoldingChecker::setTrueDag(DagNode* trueDag)
{
	this->trueDag = trueDag;
}

}

#endif /* STATEFOLDINGCHECKER_HH_ */
