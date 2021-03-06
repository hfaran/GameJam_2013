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
	for(int i=0; i<p.waveCount; i++)
	{
		for(int j=0; j<5; j++)
		{
			coll = Cuts(p.waveys[i].beat[j], collisionBox);
			if(coll)
				return coll;
		}
	}
	return false;
}

int checkCollisionEdge(Pulse &p, Rect collisionBox)
{
	bool coll = false;
	for(int i=0; i<p.waveCount; i++)
	{
		for(int j=0; j<5; j++)
		{
			if(p.waveys[i].amplitude!=0.0f) {
			
			coll = Cuts(p.waveys[i].beat[j], collisionBox);
			if(coll)
				return j; 
			}
		}
	}
	return -1;
}

int checkDropsCollision(dropArray & dArray, Rect bucketBox)
{
	bool coll = false;
	
		for(int j=0; j<dArray.arrayCount; j++)
		{
			coll = Cuts(bucketBox, dArray.dropArr[j].drawBox);
			if(coll) {
				for(int i=j; i<dArray.arrayCount; i++)
				{
					//dArray.dropArr[i]=dArray.dropArr[i+1];
					dArray.dropArr[i].copyDrop(dArray.dropArr[i+1]);
				}
				dArray.arrayCount--;
				return 1;
			}
		
		}
		return 0;
}