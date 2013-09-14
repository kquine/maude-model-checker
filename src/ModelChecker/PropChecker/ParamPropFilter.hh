/*
 * ParamPropFilter.hh
 *
 *  Created on: Sep 13, 2013
 *      Author: kquine
 */

#ifndef PARAMPROPFILTER_HH_
#define PARAMPROPFILTER_HH_

namespace modelChecker {

struct ParamPropFilter
{
	static vector<int> filterParamProp(const vector<int>& target, const PropositionTable& propTable, bool flag)
	{
		vector<int> result(target.size());
		auto it = std::copy_if(target.begin(), target.end(), result.begin(), [&propTable, &flag] (int i) { return propTable.isParamProp(i) == flag; });
		result.resize(std::distance(result.begin(),it));
		return result;
	}
};

} /* namespace modelChecker */
#endif /* PARAMPROPFILTER_HH_ */
