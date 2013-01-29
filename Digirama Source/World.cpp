#include "stdafx.h"
#include "World.hpp"

void World::InitWorld( int playerPX, int playerPY, int level )
{
	//Initialize level dependent structures
	switch (level) {
	case 1:
		//Load background for the level
		bg.load("_Assets/ChipGame/gfx/lololol.gfx");
		//Initialize platforms for the level
		ground = Rect(convertX(0), convertY(664), convertX(1366), convertY(768));
		platform[0] = Rect(convertX(469), convertY(497), convertX(296), convertY(543));
		platform[1] = Rect(convertX(1366), convertY(357), convertX(1019), convertY(405));
		platform[2] = Rect(convertX(1366), convertY(497), convertX(727), convertY(543));
		break;
	}
	//Initialize the player
	chip.initPlayer( playerPX, playerPY );
}

void World::UpdateWorld( )
{
	//Update chip
	chip.playerUpdate( KB_UP, KB_LEFT, KB_RIGHT );
}

void World::DrawWorld( )
{
	//Clear the screen before redrawing
	D.clear(BLACK);

	// Draw the background
	bg.draw(Rect((Flt) -RES_X/RES_Y, -1.0f, (Flt) RES_X/RES_Y, 1.0f));

	// Draw the player
	chip.drawPlayer( );
}