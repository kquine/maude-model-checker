/*
 * PropSet.hh
 *
 *  Created on: Aug 26, 2013
 *      Author: kquine
 */

#ifndef PROPSET_HH_
#define PROPSET_HH_
#include "natSet.hh"

namespace modelChecker {

class PropSet
{
public:
	PropSet() {}
	virtual ~PropSet() {}

	PropSet(const PropSet&) = delete;	// avoid copy
	PropSet(PropSet&& other) noexcept;

	const NatSet& getTruePropIds() const;
	bool isTrue(int propId) const;

	virtual void setTrue(int propId);
	virtual void setTrue(const PropSet& ps);

	virtual void dump(ostream& s);

private:
	NatSet truePropIds;
};

} /* namespace modelChecker */
#endif /* PROPSET_HH_ */
