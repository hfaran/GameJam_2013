#include "stdafx.h"
#include "World.hpp"
#include "customPhysics.h"

void World::InitWorld( int playerPX, int playerPY, int level )
{
	//Initially false
	isPlayerOnPlatform = false;

	//Initialize level dependent structures
	switch (level) {
	case 1:
		//Load background for the level
		bg.load("_Assets/ChipGame/gfx/lololol.gfx");
		//Initialize platforms for the level
		ground = Rect(convertX(0), convertY(664), convertX(1366), convertY(768));
		//platform[0] = Rect(convertX(469), convertY(497), convertX(296), convertY(543));
		platform[0] = Rect(convertX(296), convertY(543), convertX(469), convertY(497));
		//platform[1] = Rect(convertX(1366), convertY(357), convertX(1019), convertY(405));
		platform[1] = Rect(convertX(1019), convertY(405), convertX(1366), convertY(357));
		//platform[2] = Rect(convertX(1366), convertY(497), convertX(727), convertY(543));
		platform[2] = Rect(convertX(727), convertY(543), convertX(1366), convertY(497));
		//
		isPlayerColliding = new bool [3];
		//
		break;
	}
	//Initialize the player
	chip.initPlayer( playerPX, playerPY );
}

void World::UpdateWorld( )
{
	for(int i=0; i<3; i++)
		isPlayerColliding[i] = Cuts(platform[i], chip.drawBox);

	
	for(int i=0; i<3; i++) {
		isPlayerOnPlatform = onTop(platform[i], chip.drawBox);
		if( isPlayerOnPlatform )
			break;
	}

	//Update chip
	if( (isPlayerColliding[0] || isPlayerColliding[1] || isPlayerColliding[2]) && !isPlayerOnPlatform ) {
		chip.jumping = false;
		chip.falling = true;
		chip.playerUpdate( KB_ESC, KB_LEFT, KB_RIGHT );
	}
	else if( (isPlayerColliding[0] || isPlayerColliding[1] || isPlayerColliding[2]) && isPlayerOnPlatform ) {
		//chip.jumping = false;
		chip.falling = false;
		chip.jumpCount = 0;
		chip.playerUpdate( KB_UP, KB_LEFT, KB_RIGHT );
	}
	else {
		chip.falling = true;
		chip.playerUpdate( KB_UP, KB_LEFT, KB_RIGHT );
	}

}

void World::DrawWorld( )
{
	//Clear the screen before redrawing
	D.clear(BLACK);
	// Draw the background
	bg.draw(Rect((Flt) -RES_X/RES_Y, -1.0f, (Flt) RES_X/RES_Y, 1.0f));
	// Draw the player
	chip.drawPlayer( );

	//Debug text
	D.text(0,0,S+"isPlayerColliding: "+isPlayerColliding[0]);
	D.text(0,0.1f,S+"isPlayerColliding[1]: "+isPlayerColliding[1]);
	D.text(0,0.2f,S+"isPlayerColliding[2]: "+isPlayerColliding[2]);
	D.text(0,0.3f,S+"isPlayerOnTop: "+isPlayerOnPlatform);
	/*platform[0].draw(BLUE);
	platform[1].draw(BLUE);
	platform[2].draw(BLUE);
	chip.drawBox.draw(BLUE);*/
	D.text(0,-0.4f,S+"jumping: "+chip.jumping);
	D.text(0,-0.3f,S+"falling: "+chip.falling);
	D.text(0,-0.2f,S+"jumpCount: "+chip.jumpCount);
	D.text(0,-0.1f,S+"jumpTime: "+chip.jumpTime);
}