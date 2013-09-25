/*
 * PropSetUtil.hh
 *
 *  Created on: Sep 24, 2013
 *      Author: kquine
 */

#ifndef PROPSETUTIL_HH_
#define PROPSETUTIL_HH_
#include <memory>
#include "PropSet.hh"

namespace modelChecker {

class PropSetUtil
{
public:
	static void merge(unique_ptr<PropSet>& ps1, unique_ptr<PropSet>&& ps2);
	static void merge(unique_ptr<PropSet>& ps1, const unique_ptr<PropSet>& ps2);
};


} /* namespace modelChecker */
#endif /* PROPSETUTIL_HH_ */
