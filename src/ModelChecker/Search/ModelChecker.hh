/*
 * ModelChecker.hh
 *
 *  Created on: Nov 26, 2010
 *      Author: kquine
 */

#ifndef MODELCHECKER_HH_
#define MODELCHECKER_HH_
#include <list>
#include "macros.hh"

namespace modelChecker {

class ModelChecker
{
	typedef pair<int,int>	Edge;
public:
	virtual ~ModelChecker()	{}
	virtual bool findCounterExample() = 0;

	const list<Edge>& getLeadIn() const;
	const list<Edge>& getCycle() const;

protected:
	list<Edge> leadIn;
	list<Edge> cycle;
};

inline const list<pair<int,int> >&
ModelChecker::getLeadIn() const
{
	return leadIn;
}

inline const list<pair<int,int> >&
ModelChecker::getCycle() const
{
	return cycle;
}

}

#endif /* MODELCHECKER_HH_ */
