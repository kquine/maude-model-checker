/*
 * CompositeFairnessTable.hh
 *
 *  Created on: Sep 12, 2013
 *      Author: kquine
 */

#ifndef COMPOSITEFAIRNESSTABLE_HH_
#define COMPOSITEFAIRNESSTABLE_HH_
#include <memory>
#include "AbstractFairnessTable.hh"

namespace modelChecker {

class CompositeFairnessTable: public AbstractFairnessTable
{
public:
	bool hasStrongFairness() const override;
	int nrFairness() const override;

	int nrComponents() const;
	void addComponent(unique_ptr<AbstractFairnessTable> table);
	AbstractFairnessTable& getComponent(int i) const;

private:
	vector<unique_ptr<AbstractFairnessTable>> fairTables;	// own the tables!
};

} /* namespace modelChecker */
#endif /* COMPOSITEFAIRNESSTABLE_HH_ */
