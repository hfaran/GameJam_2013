#include "stdafx.h"
#include "drops.hpp"
#include "main.hpp"
#include <cmath>

void drop::initDrop( int pNum, bool type) {
	pathNum = pNum;
	type_ = type;
	switch (pathNum) {

	case 0:
		x = convertX(569);
		y = convertY(30);
		orgX = x;
		orgY = y;
		break;
	case 1:
		x = convertX(879);
		y = convertY(113);
		orgX = x;
		orgY = y;
		break;
	case 2:
		x = convertX(489);
		y = convertY(110);
		orgX = x;
		orgY = y;
		break;
	case 3:
		x = convertX(567);
		y = convertY(31);
		orgX = x;
		orgY = y;
		break;
	case 4:
		x = convertX(662);
		y = convertY(253);
		orgX = x;
		orgY = y;
		break;
	}

	if(type_) {
		frame.load("_Assets/HeartGame/gfx/bigdrop00.gfx");
		pixelX = 54;
		pixelY = 66;
	} else {
		frame.load("_Assets/HeartGame/gfx/littledrop00.gfx");
		pixelX = 30;
		pixelY = 36;
	}
}

void drop::updateDrop(){
	/*switch (pathNum) {

	case 1:
		y -= 0.01f;
		x = - sqrt(orgY-y);
		break;
	case 2:
		y -= 0.01f;
		x = sqrt(orgY-y);
		break;
	case 3:
		y -= 0.01f;
		x = - sqrt(orgY-y);
		break;
	case 4:
		y -= 0.01f;
		break;
	case 5:
		y -= 0.02f;
		break;
	}*/

	drawBox = Rect(this->x, this->y,this->x+2*this->pixelX/RES_Y, this->y+2*this->pixelY/RES_Y);
};

void drop::drawDrop() {
	frame.draw(drawBox);
}

void drop::copyDrop(drop & d)
{
	initDrop(d.pathNum, d.type_);
}

