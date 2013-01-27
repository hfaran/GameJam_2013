#include "stdafx.h"
#include "drops.hpp"
#include "main.hpp"
#include <cmath>

void drop::initDrop( int pNum, bool type) {
	pathNum = pNum;
	type_ = type;
	switch (pathNum) {

	case 0:
		x = convertXR(569);
		y = convertYR(30);
		orgX = x;
		orgY = y;
		break;
	case 1:
		x = convertXR(879);
		y = convertYR(113);
		orgX = x;
		orgY = y;
		break;
	case 2:
		x = convertXR(489);
		y = convertYR(110);
		orgX = x;
		orgY = y;
		break;
	case 3:
		x = convertXR(567);
		y = convertYR(31);
		orgX = x;
		orgY = y;
		break;
	case 4:
		x = convertXR(662);
		y = convertYR(253);
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

void drop::initDrop( int pNum, bool type,  Flt _x, Flt _y) {
	pathNum = pNum;
	type_ = type;
	switch (pathNum) {

	case 0:
		orgX = convertXR(469);
		orgY = convertYR(30);
		break;
	case 1:
		orgX = convertXR(879);
		orgY = convertYR(113);
		break;
	case 2:
		orgX = convertXR(489);
		orgY = convertYR(110);
		break;
	case 3:
		orgX = convertXR(567);
		orgY = convertYR(31);
		break;
	case 4:
		orgX = convertXR(662);
		orgY = convertYR(253);
		break;
	}

	x = _x;
	y = _y;

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
	switch (pathNum) {

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

