/*
 * RealizedFairnessGenerator.hh
 *
 *  Created on: Sep 8, 2013
 *      Author: kquine
 */

#ifndef REALIZEDFAIRNESSGENERATOR_HH_
#define REALIZEDFAIRNESSGENERATOR_HH_
#include "Utility/BddUtil.hh"
#include "FairTable/RealizedFairnessTable.hh"
#include "FairSet/ParamRealizedSet.hh"

namespace modelChecker {

class RealizedFairnessGenerator
{
public:
	using index_type = RealizedFairnessTable::index_type;

	RealizedFairnessGenerator(const vector<index_type>& paramFairIds, RealizedFairnessTable& fairTable);

	bool empty() const;
	deque<pair<index_type,NatSet>> generateRealizedFairness(const ParamPropSet& pps);

private:
	const vector<index_type> paramFairIds;
	RealizedFairnessTable& fairTable;
};


} /* namespace modelChecker */

#endif /* REALIZEDFAIRNESSGENERATOR_HH_ */
