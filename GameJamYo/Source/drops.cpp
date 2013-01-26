#include "stdafx.h"
#include "drops.hpp"
#include "main.hpp"
#include <cmath>

void drop::initDrop( int pNum, bool type) {
	pathNum = pNum;
	switch (pathNum) {

	case 0:
		x = convertX(525);
		y = convertY(60);
		break;
	case 1:
		x = convertX(1174);
		y = convertY(222);
		break;
	case 2:
		x = convertX(347);
		y = convertY(205);
		break;
	case 3:
		x = convertX(820);
		y = convertY(186);
		break;
	case 4:
		x = convertX(715);
		y = convertY(540);
		break;
	}

	if(type) {
		frame.load("_Assets/HeartGame/gfx/bigdrop00.gfx");
		pixelX = 66;
		pixelY = 54;
	} else {
		frame.load("_Assets/HeartGame/gfx/littledrop00.gfx");
		pixelX = 30;
		pixelY = 36;
	}
}

void drop::updateDrop(){
	switch (pathNum) {

	case 1:
		y -= 0.01f;
		x = - sqrt(convertY(60)-y);
		break;
	case 2:
		y -= 0.01f;
		x = sqrt(convertY(222)-y);
		break;
	case 3:
		y -= 0.01f;
		x = - sqrt(convertY(205)-y);
		break;
	case 4:
		y -= 0.01f;
		break;
	case 5:
		y -= 0.02f;
		break;
	}

	drawBox = Rect(this->x, this->y,this->x+2*this->pixelX/RES_Y, this->y+2*this->pixelY/RES_Y);
};

void drop::drawDrop() {
	frame.draw(drawBox);
}