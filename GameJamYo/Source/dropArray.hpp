#include "stdafx.h"
#include "NPC.hpp"

#ifndef DROPARRAY_H
#define DROPARRAY_H

struct dropArray {

	NPC * dropArr;
	int numDrops;

	void initDropArray(int nDrops) {
		numDrops = nDrops;
		dropArr = new NPC[numDrops];

		for(int i=0; i<numDrops; i++)
			dropArr[i].initNPC(70,54);
	}
	void updateDropArray(int nDrops) {


		for(int i=0; i<numDrops; i++)
			dropArr[i].updateNPC();


	}
	void drawDrops() {

		for(int i=0; i<numDrops; i++)
			dropArr[i].drawNPC();

	}
}



#endif