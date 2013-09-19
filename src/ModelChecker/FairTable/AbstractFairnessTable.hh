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
	using index_type = unsigned int;

	virtual ~AbstractFairnessTable() {}

	virtual bool hasStrongFairness() const = 0;
	virtual unsigned int nrFairness() const = 0;
};

} /* namespace modelChecker */
#endif /* ABSTRACTFAIRNESSTABLE_HH_ */
