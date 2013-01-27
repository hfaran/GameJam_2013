#include "stdafx.h"
#include "drops.hpp"

#ifndef DROPARRAY_H
#define DROPARRAY_H

struct dropArray {

	drop * dropArr;
	int numDrops;
	bool newDrop;
	int arrayCap;
	int arrayCount;
	int Stage;
	bool dropCheck;

	void initDropArray(int nDrops, int stage);
	void updateDropArray(int nDrops, int stage, int & score, float speedOfDrops);
	void drawDrops( void );
	
};



#endif