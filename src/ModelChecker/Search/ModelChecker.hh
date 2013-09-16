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
public:
	using Edge =  pair<unsigned int,int>;

	ModelChecker()			{}
	virtual ~ModelChecker()	{}

	ModelChecker(const ModelChecker&) = delete;
	ModelChecker& operator=(const ModelChecker&) = delete;

	virtual bool findCounterExample() = 0;
	virtual const DagSystemGraph& getSystemGraph() const = 0;

	const list<Edge>& getLeadIn() const		{ return leadIn; }
	const list<Edge>& getCycle() const		{ return cycle; }

protected:
	list<Edge> leadIn;
	list<Edge> cycle;
};

}

#endif /* MODELCHECKER_HH_ */
