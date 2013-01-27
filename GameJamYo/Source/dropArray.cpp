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

void dropArray::updateDropArray(int nDrops, int stage) {
	Stage = stage;
	if(newDrop && dropCheck && stage != 0) {
		dropArr[arrayCount].initDrop(rand() % (stage)+1,rand() % 1);
		newDrop = false;
		dropCheck = false;
		arrayCount++;
	} else if (newDrop && !dropCheck) {
		newDrop = false;
		dropCheck = true;
	}
	for(int i=0; i<arrayCount; i++)
		dropArr[i].updateDrop();

}

void dropArray::drawDrops() {

	for(int i=0; i<arrayCount; i++)
		dropArr[i].drawDrop();
	D.text(0,0,S+"arrayCount: "+arrayCount);
	D.text(0,0.1f,S+"Stage: "+Stage);

}