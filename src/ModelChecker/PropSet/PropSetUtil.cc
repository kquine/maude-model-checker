/*
 * PropSetUtil.cc
 *
 *  Created on: Sep 24, 2013
 *      Author: kquine
 */

// utility stuff
#include "macros.hh"
#include "vector.hh"

// forward declarations
#include "interface.hh"
#include "core.hh"

// ltlr definitions
#include "ParamPropSet.hh"
#include "PropSetUtil.hh"

namespace modelChecker {

void
PropSetUtil::merge(unique_ptr<PropSet>& ps1, unique_ptr<PropSet>&& ps2)
{
	if (ps1 && ( dynamic_cast<ParamPropSet*>(ps1.get()) || !dynamic_cast<ParamPropSet*>(ps2.get()) ) )
	{
		if (ps2) ps1->setTrue(*ps2);
	}
	else if (ps2)
	{
		if (ps1) ps2->setTrue(*ps1);
		ps1 = move(ps2);
	}
}

void
PropSetUtil::merge(unique_ptr<PropSet>& ps1, const unique_ptr<PropSet>& ps2)
{
	if (ps1 && ( dynamic_cast<ParamPropSet*>(ps1.get()) || !dynamic_cast<const ParamPropSet*>(ps2.get()) ) )
	{
		if (ps2) ps1->setTrue(*ps2);
	}
	else if (ps2)
	{
		PropSet* copy2;
		if (auto pps2 = dynamic_cast<const ParamPropSet*>(ps2.get()))
			copy2 = new ParamPropSet(*pps2);
		else
			copy2 = new PropSet(*ps2);

		if (ps1) copy2->setTrue(*ps1);
		ps1.reset(copy2);
	}
}

} /* namespace modelChecker */
