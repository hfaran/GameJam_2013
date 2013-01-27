#include "stdafx.h"
#include "drops.hpp"
#include "main.hpp"
#include <cmath>

void drop::initDrop( int pNum, bool type) {
	pathNum = pNum;
	type_ = type;

	if(type_) {
		frame.load("_Assets/HeartGame/gfx/bigdrop00.gfx");
		pixelX = 54;
		pixelY = 66;
	} else {
		frame.load("_Assets/HeartGame/gfx/littledrop00.gfx");
		pixelX = 30;
		pixelY = 36;
	}

	switch (pathNum-1) {

	case 0:
		x = convertXR(569)-4*pixelX/RES_Y;
		y = convertYR(30)-2*pixelY/RES_Y;
		orgX = x;
		orgY = y;
		break;
	case 1:
		x = convertXR(879)-4*pixelX/RES_Y;
		y = convertYR(113)-2*pixelY/RES_Y;
		orgX = x;
		orgY = y;
		break;
	case 2:
		x = convertXR(489)-4*pixelX/RES_Y;
		y = convertYR(110)-2*pixelY/RES_Y;
		orgX = x;
		orgY = y;
		break;
	case 3:
		x = convertXR(567)+2*pixelX/RES_Y;
		y = convertYR(31)-4*pixelY/RES_Y;
		orgX = x;
		orgY = y;
		break;
	case 4:
		x = convertXR(662);
		y = convertYR(253)-4*pixelY/RES_Y;
		orgX = x;
		orgY = y;
		break;
	}

	
}

void drop::initDrop( int pNum, bool type,  Flt _x, Flt _y) {
	pathNum = pNum;
	type_ = type;

	if(type_) {
		frame.load("_Assets/HeartGame/gfx/bigdrop00.gfx");
		pixelX = 54;
		pixelY = 66;
	} else {
		frame.load("_Assets/HeartGame/gfx/littledrop00.gfx");
		pixelX = 30;
		pixelY = 36;
	}

	switch (pathNum-1) {

	case 0:
		orgX = convertXR(569)-4*pixelX/RES_Y;
		orgY = convertYR(30)-2*pixelY/RES_Y;
		break;
	case 1:
		orgX = convertXR(879)-3*pixelX/RES_Y;
		orgY = convertYR(113)-2*pixelY/RES_Y;
		break;
	case 2:
		orgX = convertXR(489)-4*pixelX/RES_Y;
		orgY = convertYR(110)-2*pixelY/RES_Y;
		break;
	case 3:
		orgX = convertXR(567);
		orgY = convertYR(31);
		break;
	case 4:
		orgX = convertXR(662);
		orgY = convertYR(253)-2*pixelY/RES_Y;
		break;
	}

	x = _x;
	y = _y;

	
}

void drop::updateDrop(){
	switch (pathNum) {

	case 1:
		y -= 0.01f;
		x = - sqrt(orgY-y)-4*pixelX/RES_Y;
		break;
	case 2:
		y -= 0.01f;
		x = sqrt(orgY-y)+5*pixelX/RES_Y;
		break;
	case 3:
		y -= 0.01f;
		x = - sqrt(orgY-y)-4*pixelX/RES_Y;
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

void drop::copyDrop(drop & d)
{
	initDrop(d.pathNum, d.type_, d.x, d.y);
}

