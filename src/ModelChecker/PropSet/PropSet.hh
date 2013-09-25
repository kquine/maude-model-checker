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

class PropSetUtil;

class PropSet
{
	friend class PropSetUtil;
public:
	PropSet() = default;
	virtual ~PropSet() = default;

	PropSet(const PropSet&) = default;
	PropSet(PropSet&& other) noexcept;

	bool isTrue(unsigned int propId) const;
	const NatSet& getTruePropIds() const;

	virtual void setTrue(unsigned int propId);

	virtual void dump(ostream& s);

protected:
	virtual void setTrue(const PropSet& ps);	// can be a null ptr

private:
	NatSet truePropIds;
};

} /* namespace modelChecker */
#endif /* PROPSET_HH_ */
