/*
 * BasicPropHandler.hh
 *
 *  Created on: Jul 7, 2011
 *      Author: kquine
 */

#ifndef BASICPROPHANDLER_HH_
#define BASICPROPHANDLER_HH_
#include "Interface/PropChecker.hh"
#include "PropHandler.hh"

namespace modelChecker {

/*
 * A basic prop handler interface.
 */
class BasicPropHandler: public PropHandler
{
public:
	struct PropSet: public PropHandler::PropSet
	{
		NatSet label;
		bool operator<(const PropHandler::PropSet& td) const	{ return label < safeCast(const PropSet*,&td)->label; }
	};
	BasicPropHandler(ProofTermGenerator* pfg);
	void updateProp(PropHandler::PropSet* d, DagNode* t);
	void updateProp(PropHandler::PropSet* d, RewriteTransitionState* t);

protected:
	virtual void update(PropSet* d, DagNode* t) = 0;
private:
	ProofTermGenerator* pfg;
};

inline
BasicPropHandler::BasicPropHandler(ProofTermGenerator* pfg): pfg(pfg) {}

inline void
BasicPropHandler::updateProp(PropHandler::PropSet* d, DagNode* t)
{
	update(safeCast(PropSet*,d),t);
}

inline void
BasicPropHandler::updateProp(PropHandler::PropSet* d, RewriteTransitionState* t)
{
	Assert(pfg != NULL, "BasicPropHandler::updateProp: No proof term when computing event props");
	update(safeCast(PropSet*,d), pfg->makeProofDag(t->getPosition(), *t->getRule(), t->getSubstitution()));
}


}

#endif /* BASICPROPHANDLER_HH_ */
