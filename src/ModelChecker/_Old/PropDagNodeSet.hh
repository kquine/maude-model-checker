/*
 * PropDagNodeSet.hh
 *
 *  Created on: Dec 3, 2010
 *      Author: kquine
 */

#ifndef PROPDAGNODESET_HH_
#define PROPDAGNODESET_HH_
#include "dagNodeSet.hh"
#include "natSet.hh"

namespace modelChecker {

class PropDagNodeSet: public DagNodeSet
{
public:
	bool hasEvent()				{ return events.size() > 0; }
	bool isEvent(int propId)	{ return events.contains(propId); }
	void setEvent(int propId)		{ events.insert(propId); }

	int getLocalId(int stateNr, int propId) const;
private:
	NatSet events;
};

}

#endif /* PROPDAGNODESET_HH_ */
