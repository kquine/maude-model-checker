/*
 * BaseFairLabel.hh
 *
 *  Created on: Sep 21, 2013
 *      Author: kquine
 */

#ifndef BASEFAIRLABEL_HH_
#define BASEFAIRLABEL_HH_
#include <memory>
#include "FairSet/FairSet.hh"
#include "FairChecker/FairnessChecker.hh"

namespace modelChecker {

//
// a base class for either StateFairLabel or EventFairLabel
//
class BaseFairLabel
{
public:
	struct Label
	{
		unique_ptr<FairSet> fs;
		bool operator<(const Label& l) const					{ return *fs < *l.fs; }
	};

	BaseFairLabel(FairnessChecker& fc): fairC(fc) 				{}

	void updateLabel(const PropSet& trueProps, Label& l) const	{ l.fs = fairC.computeAllFairness(trueProps); }
	unique_ptr<FairSet> makeFairSet(Label* l) const				{ return l->fs->clone(); }

private:
	FairnessChecker& fairC;
};

} /* namespace modelChecker */
#endif /* BASEFAIRLABEL_HH_ */
