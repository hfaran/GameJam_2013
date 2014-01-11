#include "stdafx.h"
#include "mathFunctions.h"


#ifndef DROPS_HPP
#define DROPS_HPP

struct drop {
	Flt x;
	Flt y;

	Flt orgX;
	Flt orgY;

	int pixelX;
	int pixelY;

	Image frame;
	Rect drawBox;
	int pathNum;
	bool type_;


	void initDrop( int pNum, bool type);
	void initDrop( int pNum, bool type, Flt _x, Flt _y);
	void updateDrop(float speedOfDrops);
	void drawDrop();
	void copyDrop(drop & d);
};

#endif