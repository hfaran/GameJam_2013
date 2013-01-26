#include "stdafx.h"
#include "mathFunctions.h"


#ifndef DROPS_HPP
#define DROPS_HPP

struct drop {
	Flt x;
	Flt y;
	int pixelX;
	int pixelY;

	Image frame;
	Rect drawBox;
	int pathNum;


	void initDrop( int pNum, bool type);
	void updateDrop();
	void drawDrop();
};

#endif