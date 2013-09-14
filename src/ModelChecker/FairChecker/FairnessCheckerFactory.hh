/*
 * FairnessCheckerFactory.hh
 *
 *  Created on: Sep 12, 2013
 *      Author: kquine
 */

#ifndef FAIRNESSCHECKERFACTORY_HH_
#define FAIRNESSCHECKERFACTORY_HH_
#include <memory>
#include "FairnessChecker.hh"

namespace modelChecker {

class FairnessCheckerFactory
{
public:
	static unique_ptr<FairnessChecker> createChecker(bool stateOnly, AbstractFairnessTable* fairTable);

private:
	template <typename Table, typename Checker>
	static Checker* createChecker(bool stateOnly, Table& table);
};

} /* namespace modelChecker */
#endif /* FAIRNESSCHECKERFACTORY_HH_ */
