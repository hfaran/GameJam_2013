#include "stdafx.h"
#include "World.hpp"
#include "customPhysics.h"

//TODO
//	-Add parabolic fall
//	-Add detection for right/left edge collision of platform and subsequent restriction of right/left movement
//	-Find a better way of movement restriction than replacing the key with KB_ESC


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
		ground = Rect(convertX(1366), convertY(768), convertX(0), convertY(664));
		platform[0] = Rect(convertX(296), convertY(543), convertX(469), convertY(497));
		platform[2] = Rect(convertX(1019), convertY(405), convertX(1366), convertY(357));
		platform[1] = Rect(convertX(727), convertY(543), convertX(1366), convertY(497));
		//
		isPlayerColliding = new bool [3];
		isPlayerOnPlatform = new bool [3];
		//
		break;
	}
	//Initialize the player
	chip.initPlayer( playerPX, playerPY );
}

void World::UpdateWorld( int level )
{
	//Update method for level 1
	switch (level) {
	case 1:
		//Set bools that show whether Player is colliding with the ith platform or is on top of it
		for(int i=0; i<3; i++)
		{
			isPlayerColliding[i] = Cuts(platform[i], chip.collBox);
			isPlayerOnPlatform[i] = onTop( platform[i], chip.collBox);
		}
		//Check if Player is colliding with the ground
		isPlayerCollGround = (chip.collBox.min.y <= ground.max.y);

		//Sequentially check collisions with each platform and handle cases
		if( isPlayerColliding[2] )
		{
			// If player is colliding with, AND above platform 2, he should stop falling, reset his jump count
			//	and be allowed to use all key inputs
			if( isPlayerOnPlatform[2] ) {
				chip.falling = false;
				chip.jumpCount = 0;
				chip.playerUpdate( KB_UP, KB_LEFT, KB_RIGHT );
			}
			//Otherwise, we assume the player is colliding with some other part of the platform (not the top)
			// so he should stop jumping and start falling; we disable the jump input here so he can't jump again
			//	until he lands (KB_ESC is a hacky way of doing it, replace this)
			else
			{
				chip.jumping = false;
				chip.falling = true;
				chip.playerUpdate( KB_ESC, KB_LEFT, KB_RIGHT );
			}
		}
		//Do the same for this platform as for the last one
		else if( isPlayerColliding[1] )
		{
			if( isPlayerOnPlatform[1] ) {
				chip.falling = false;
				chip.jumpCount = 0;
				chip.playerUpdate( KB_UP, KB_LEFT, KB_RIGHT );
			}
			else
			{
				chip.jumping = false;
				chip.falling = true;
				chip.playerUpdate( KB_ESC, KB_LEFT, KB_RIGHT );
			}
		}
		//Do the same for this platform as for the last one
		else if( isPlayerColliding[0] )
		{
			if( isPlayerOnPlatform[0] ) {
				chip.falling = false;
				chip.jumpCount = 0;
				chip.playerUpdate( KB_UP, KB_LEFT, KB_RIGHT );
			}
			else
			{
				chip.jumping = false;
				chip.falling = true;
				chip.playerUpdate( KB_ESC, KB_LEFT, KB_RIGHT );
			}
		}
		// If player is on the ground, he should stop falling, have his jumpCount reset and be able to do whatever
		else if( isPlayerCollGround ) {
			chip.falling = false;
			chip.jumpCount = 0;
			chip.playerUpdate( KB_UP, KB_LEFT, KB_RIGHT );
		}
		// If he otherwise falling through midair (i.e., not touching anything AND not jumping), he should fall
		//	 and not be allowed to jump in midair (currently done through replacing the KB_UP key, find a better method)
		else {
			if(!chip.jumping)
				chip.falling = true;
			chip.playerUpdate( KB_ESC, KB_LEFT, KB_RIGHT );
		}
	}



	//OLD COLLISION CODE ->
	/*if( (isPlayerColliding[0] || isPlayerColliding[1] || isPlayerColliding[2] || isPlayerCollGround) && !isPlayerOnPlatform ) {
	chip.jumping = false;
	chip.falling = true;
	chip.playerUpdate( KB_ESC, KB_LEFT, KB_RIGHT );
	}
	else if( (isPlayerColliding[0] || isPlayerColliding[1] || isPlayerColliding[2] || isPlayerCollGround) && isPlayerOnPlatform ) {
	chip.falling = false;
	chip.jumpCount = 0;
	chip.playerUpdate( KB_UP, KB_LEFT, KB_RIGHT );
	}
	else {
	chip.falling = true;
	chip.playerUpdate( KB_ESC, KB_LEFT, KB_RIGHT );
	}*/

}

void World::DrawWorld( )
{
	//Clear the screen before redrawing
	D.clear(BLACK);
	// Draw the background
	bg.draw(Rect((Flt) -RES_X/RES_Y, -1.0f, (Flt) RES_X/RES_Y, 1.0f));
	// Draw the player
	chip.drawPlayer( );



	// ************* DEBUGGING STUFF ***************************** //
	//D.text(0,0,S+"isPlayerCollidingRED: "+isPlayerColliding[0]);
	//D.text(0,0.1f,S+"isPlayerCollidingBLUE: "+isPlayerColliding[1]);
	//D.text(0,0.2f,S+"isPlayerCollidingGREEN: "+isPlayerColliding[2]);
	//D.text(0,0.3f,S+"isPlayerOnTopRED: "+isPlayerOnPlatform[0]);
	//D.text(0,0.4f,S+"isPlayerOnTopBLUE: "+isPlayerOnPlatform[1]);
	//D.text(0,0.5f,S+"isPlayerOnTopGREEN: "+isPlayerOnPlatform[2]);
	//D.text(0,0.6f,S+"isPlayerCollGround: "+isPlayerCollGround);
	//platform[0].draw(RED);
	//platform[1].draw(BLUE);
	//platform[2].draw(GREEN);
	//chip.drawBox.draw(RED);
	//ground.draw(BLUE);
	//D.text(0,-0.4f,S+"jumping: "+chip.jumping);
	//D.text(0,-0.3f,S+"falling: "+chip.falling);
	//D.text(0,-0.2f,S+"jumpCount: "+chip.jumpCount);
	//D.text(0,-0.1f,S+"jumpTime: "+chip.jumpTime);

	Circle(0.01f, chip.drawBox.min).draw(RED  );
	Circle(0.01f, chip.drawBox.max).draw(BLUE  );

	for(int i=0;i<3;i++) {
		Circle(0.01f, platform[i].max).draw(BLUE  );
		Circle(0.01f, platform[i].min).draw(RED  );
	}
}