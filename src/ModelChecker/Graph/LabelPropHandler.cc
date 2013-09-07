/*
 * LabelPropHandler.cc
 *
 *  Created on: Aug 27, 2013
 *      Author: kquine
 */

// utility stuff
#include "macros.hh"
#include "vector.hh"

// forward declarations
#include "interface.hh"
#include "core.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNodeSet.hh"

// core class definitions
#include "rewritingContext.hh"
#include "symbolMap.hh"

// ltlr definitions
#include "LabelPropHandler.hh"

namespace modelChecker {

bool
LabelPropHandler::Label::operator<(const PropHandler::Label& l) const
{
	return label < static_cast<const Label&>(l).label;
}

bool
LabelPropHandler::satisfiesProp(int localId, const PropHandler::Label& l) const
{
	return static_cast<const Label&>(l).label.contains(localId);
}

void
LabelPropHandler::updateLabel(const PropSet& t, PropHandler::Label& l) const
{
	for (Uint i = propIds.size(); i > 0; --i)
	{
		if ( t.isTrue(propIds[i-1]) )
			static_cast<Label&>(l).label.insert(i-1);
	}
}

void
LabelPropHandler::transferTruth(const PropHandler::Label& l, PropSet& t) const
{
	//TODO
}

void
LabelPropHandler::clearExtra(PropHandler::Label& l) const
{
	//TODO
}

} /* namespace modelChecker */
