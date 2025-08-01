#ifndef MOVEVALUEBOTH_H
#define MOVEVALUEBOTH_H
//
#include "move.h"
#include "changepointsontree.h"
//
class MoveValueBoth : public Move
{
public:
	MoveValueBoth(Param * p);
	Move * clone() {return new MoveValueBoth(*this);};
	void move();
	virtual ~MoveValueBoth();
	inline string desc() {return "Updating top-value in both";}

};
#endif
