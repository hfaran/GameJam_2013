#include "stdafx.h"
#include "dropArray.hpp"
#include "drops.hpp"
#include <cstdlib>

void dropArray::initDropArray(int nDrops, int stage) {
	numDrops = nDrops;
	dropArr = new drop[numDrops];
	newDrop = false;
	arrayCap = nDrops;
	arrayCount = 0;
	Stage = stage;
	dropCheck = false;
}

void dropArray::updateDropArray(int nDrops, int stage, int & score) {
	Stage = stage;
	if(newDrop && dropCheck && stage != 0) {
		dropArr[arrayCount].initDrop(rand() % (stage)+1,rand() % 2);
		newDrop = false;
		dropCheck = false;
		arrayCount++;
	} else if (newDrop && !dropCheck) {
		newDrop = false;
		dropCheck = true;
	}
	for(int j=0; j<arrayCount; j++) {
		dropArr[j].updateDrop();
		if(dropArr[j].y < -1.2f) {
			for(int i=j; i<arrayCount; i++)
			{
				dropArr[i].copyDrop(dropArr[i+1]);
			}
			arrayCount--;
			score--;
			j--;
		}
	}

}

void dropArray::drawDrops() {

	for(int i=0; i<arrayCount; i++)
		dropArr[i].drawDrop();
	//D.text(0,0,S+"arrayCount: "+arrayCount);
	//D.text(0,0.1f,S+"Stage: "+Stage);

}