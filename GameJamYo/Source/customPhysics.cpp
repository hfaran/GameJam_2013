#include "stdafx.h"
#include "customPhysics.h"

bool onTop(Rect A, Rect B)
{
	//If Player's feet are above the top of the platform
	//	he's on top of it
	if(B.min.y>A.max.y)
		return true;
	else
		return false;
}