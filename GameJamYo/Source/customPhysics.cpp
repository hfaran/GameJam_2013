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

bool checkCollision(Pulse &p, Rect collisionBox)
{
	bool coll = false;
	for(int i=0; i<20; i++)
	{
		for(int j=0; j<5; j++)
		{
			coll = Cuts(p.waveys[i].beatCol[j], collisionBox);
			if(coll)
				return coll;
		}
	}
	return false;
}