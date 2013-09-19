/*
 * CompositeFairnessTable.cc
 *
 *  Created on: Sep 12, 2013
 *      Author: kquine
 */

// utility stuff
#include "macros.hh"
#include "vector.hh"

#include "CompositeFairnessTable.hh"

namespace modelChecker {


bool
CompositeFairnessTable::hasStrongFairness() const
{
	return any_of(fairTables.begin(), fairTables.end(), [] (const unique_ptr<AbstractFairnessTable>& i) { return i->hasStrongFairness(); });
}

CompositeFairnessTable::index_type
CompositeFairnessTable::nrFairness() const
{
	index_type total = 0;
	for (auto& i : fairTables) total += i->nrFairness();
	return total;
}

CompositeFairnessTable::index_type
CompositeFairnessTable::nrComponents() const
{
	return fairTables.size();
}

void
CompositeFairnessTable::addComponent(unique_ptr<AbstractFairnessTable>&& table)
{
	fairTables.push_back(move(table));
}

AbstractFairnessTable&
CompositeFairnessTable::getComponent(index_type i) const
{
	return *fairTables[i];
}



} /* namespace modelChecker */
