/*
 * AbstractFairnessTable.hh
 *
 *  Created on: Sep 11, 2013
 *      Author: kquine
 */

#ifndef ABSTRACTFAIRNESSTABLE_HH_
#define ABSTRACTFAIRNESSTABLE_HH_

namespace modelChecker {

class AbstractFairnessTable
{
public:
	virtual ~AbstractFairnessTable() {}

	virtual bool hasStrongFairness() const = 0;
	virtual int nrFairness() const = 0;
};

} /* namespace modelChecker */
#endif /* ABSTRACTFAIRNESSTABLE_HH_ */
