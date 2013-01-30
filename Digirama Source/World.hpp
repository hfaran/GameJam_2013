#include "stdafx.h"


#ifndef WORLD_HPP
#define WORLD_HPP

#include "Player.h"
#include "mathFunctions.h"
#include "main.h"

struct World {

	Image bg;
	Player chip;
	Rect ground, platform[3];

	bool * isPlayerColliding;
	bool isPlayerCollGround;
	bool * isPlayerOnPlatform;

	void InitWorld( int playerPX=93, int playerPY=134, int level=1 );
	void UpdateWorld( );
	void DrawWorld( );

};


#endif