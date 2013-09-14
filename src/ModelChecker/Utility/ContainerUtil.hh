/*
 * ContainerUtil.hh
 *
 *  Created on: Sep 13, 2013
 *      Author: kquine
 */

#ifndef CONTAINERUTIL_HH_
#define CONTAINERUTIL_HH_

namespace modelChecker {

class ContainerUtil
{
public:
	template <typename T>
	static vector<T> filterVector(const vector<T>& source, const function<bool(int)>& filter)
	{
		vector<T> result(source.size());
		auto it = std::copy_if(source.begin(), source.end(), result.begin(), filter);
		result.resize(std::distance(result.begin(),it));
		return result;
	}
};

} /* namespace modelChecker */
#endif /* CONTAINERUTIL_HH_ */
