/*
 * SimplePropHandler.hh
 *
 *  Created on: Jul 6, 2011
 *      Author: kquine
 */

#ifndef SIMPLEPROPHANDLER_HH_
#define SIMPLEPROPHANDLER_HH_
#include "Interface/PropChecker.hh"
#include "Interface/ProofTermGenerator.hh"
#include "BasicPropHandler.hh"

namespace modelChecker {

/*
 * A simple prop handler where all propositions are computed at once,
 * recommended to use this class only if total # of props (including event props) is less than word size.
 */
class SimplePropHandler: public BasicPropHandler
{
public:
	typedef BasicPropHandler::PropSet	PropSet;

	SimplePropHandler(const PropChecker* pc, const DagNodeSet& propDags,
					  const NatSet& propSet, ProofTermGenerator* pfg = NULL);

	bool isSatisfied(const PropHandler::PropSet* d, int propId) const;
	bool isRelated(int propId) const;

private:
	void update(PropSet* d, DagNode* t);

	const PropChecker* pc;
	const DagNodeSet& propDags;
	const NatSet& propSet;
};


}

#endif /* SIMPLEPROPHANDLER_HH_ */
