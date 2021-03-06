#include "stdafx.h"
#include "customPhysics.h"

// Cuts does the job of this function already (although
//	code from here would be useful for detecting right/left collisions (maybe))
//bool collision(Rect A, Rect B)
//{
//	//Check boundaries of two rectangles to see
//	//	if they intersect
//	if(A.max.y>B.max.y && A.min.y<B.max.y)
//		if(A.max.x>B.max.x && A.min.x<B.max.x)
//			return true;
//	else if(A.max.y>B.min.y && A.min.y<B.min.y)
//		if(A.max.x>B.min.x && A.min.x<B.min.x)
//			return true;
//	else if(A.max.y>B.min.y && A.min.y<B.min.y)
//		if(A.max.x>B.max.x && A.min.x<B.max.x)
//			return true;
//	else if(A.max.y>B.max.y && A.min.y<B.max.y)
//		if(A.max.x>B.min.x && A.min.x<B.min.x)
//			return true;
//	else
//		return false;
//}

bool onTop(Rect A, Rect B)
{
	//If Player's feet are above the top of the platform
	//	he's on top of it
	if(B.min.y>= ( (A.max.y+A.min.y)/2.0 /*+ (A.max.y-A.min.y)/2.0*/) )
		return true;
	else
		return false;
}