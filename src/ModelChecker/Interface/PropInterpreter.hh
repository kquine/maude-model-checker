/*
 * PropInterpreter.hh
 *
 *  Created on: Jun 2, 2011
 *      Author: kquine
 */

#ifndef PROPINTERPRETER_HH_
#define PROPINTERPRETER_HH_

namespace modelChecker {

class PropInterpreter
{
	NO_COPYING(PropInterpreter);
public:
	PropInterpreter();

	bool isEventProp(DagNode* propDag) const;
	bool isParamProp(DagNode* propDag) const;

protected:
    bool attachSymbol(const char* purpose, Symbol* symbol);
    void copyAttachments(PropInterpreter* original, SymbolMap* map);
    void getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols);

private:
	//
	//  Symbol to characterize an action
	//
	Symbol* tauSymbol;
};

}

#endif /* PROPINTERPRETER_HH_ */
