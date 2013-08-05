/*
 * StaticPropHandler.hh
 *
 *  Created on: Jul 6, 2011
 *      Author: kquine
 */

#ifndef STATICPROPHANDLER_HH_
#define STATICPROPHANDLER_HH_
#include "Interface/PropChecker.hh"
#include "Interface/ProofTermGenerator.hh"
#include "BasicPropHandler.hh"

namespace modelChecker {

/*
 * A prop handler where all propositions are computed at once,
 * where prop ids are localized to "propSet", to accomplish more compact prop representation..
 * NOTE: dynamic version can be also developed, but some interface should be modified..
 */
class StaticPropHandler: public BasicPropHandler
{
public:
	typedef BasicPropHandler::PropSet	PropSet;

	StaticPropHandler(const PropChecker* pc, const DagNodeSet& propDags,
			  	  	  const NatSet& propSet, ProofTermSymbol* pfg = NULL);

	bool isSatisfied(const PropHandler::PropSet* d, int propId) const;
	bool isRelated(int propId) const;

private:
	void update(PropSet* d, DagNode* t);

	const PropChecker* pc;
	ProofTermSymbol* pfg;
	const DagNodeSet& propDags;	// globalPropId |-> propDag
	Vector<int> local2global;	// localPropId |-> globalPropId
	Vector<int> global2local;	// globalPropId |-> localPropId (NONE for undefined..)
};

}

#endif /* STATICPROPHANDLER_HH_ */
