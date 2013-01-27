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
	void updateDrop();
	void drawDrop();
	void copyDrop(drop & d);
};

#endif