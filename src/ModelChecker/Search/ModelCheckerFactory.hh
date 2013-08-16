/*
 * ModelCheckerFactory.hh
 *
 *  Created on: Aug 6, 2013
 *      Author: kquine
 */

#ifndef MODELCHECKERFACTORY_HH_
#define MODELCHECKERFACTORY_HH_
#include "ModelChecker.hh"
#include "Graph/DagGraphMap.hh"

namespace modelChecker {

class ModelCheckerFactory
{
public:
	ModelCheckerFactory();
	virtual ~ModelCheckerFactory() {}

	ModelChecker* getModelChecker();
	const DagGraphMap* getDagGraphMap() const;
};

} /* namespace modelChecker */
#endif /* MODELCHECKERFACTORY_HH_ */
