#include "stdafx.h"
#include "Player.h"
#include "main.h"

void Player::drawPlayer( )
{
	currentFrame->draw(drawBox);
}

//1 = move left, 2 = move right, 3 = stop, 4 = jump
void Player::movePlayer( int moveType )
{
	switch (moveType) {
	case 1:
		facingLeft = true;
		x -= Time.d()*movementSpeed;

		//If trying to move off-screen, stop him
		if(x < (Flt) -RES_X/RES_Y)
			x = (Flt) -RES_X/RES_Y;

		//Animation
		animatePlayer( "run" );
		break;

	case 2:
		facingLeft = false;
		x += Time.d()*movementSpeed;

		//If trying to move off-screen, stop him
		if(x+2*(Flt) pixelX/RES_Y > RES_X/RES_Y)
			x = RES_X/RES_Y-2*pixelX/RES_Y;

		//Animation
		animatePlayer( "run" );
		break;

	case 3:
		animatePlayer( "stop" );
		break;

	case 4:
		// Set jumping and let handleJump take care of the rest
		jumping = true;
		break;
	}
}

void Player::handleJump( )
{
	if(jumping && jumpCount < jumpTime) { 
		y += (jumpTime - jumpCount) * (jumpTime - jumpCount) * jumpSpeed; 
		jumpCount++;
	/*} else if(!jumping && jumpCount > 0 && falling==true) {
		y -= (jumpTime + 1 - jumpCount) * (jumpTime + 1 - jumpCount) * jumpSpeed; 
		jumpCount--;*/
	} else if(jumping && jumpCount >= jumpTime) {
		jumping = false;
		jumpCount = 0;
	}
}

void Player::handleFall( )
{
	if( falling )
		y -= jumpSpeed*200;
}

void Player::handleInput( KB_BUTTON jump, KB_BUTTON left, KB_BUTTON right )
{
	//Call movePlayer to handle movement based on given input
	if(Kb.b(jump) && jumpCount == 0) {
		movePlayer(4);
	}
	else if(Kb.b(left)) {
		movePlayer(1);
	}
	else if(Kb.b(right)) {
		movePlayer(2);
	}
	else {
		movePlayer(3);
	}
}

void Player::playerUpdate( KB_BUTTON jump, KB_BUTTON left, KB_BUTTON right )
{
	// If Player if facing left, flip him, otherwise, draw him normally
	if(facingLeft) {
		drawBox = Rect(x+2*pixelX/RES_Y, y,
			x, y+2*pixelY/RES_Y);
	} else {
		drawBox = Rect(x, y, 
			x+2*pixelX/RES_Y, y+2*pixelY/RES_Y);
	}
	//Check for key presses then move player
	handleInput( jump, left, right );
	//Handle jumping if needed
	handleJump( );
	//
	handleFall( );
	//Update animation frame
	currentFrame = &frame[(int) frameCounter];
}

// Initializes all instance variables
void Player::initPlayer( int pX, int pY, float moveSpeed, Flt jSpeed, int jTime, int nFrames, Flt animSpd )
{
	x = -RES_X/RES_Y * 0.9f; //Start at the ~edge of the screen
	y = -0.60f;		//Start on the top of the floor
	jumping = false;
	jumpCount = 0;
	facingLeft = false;
	falling=true;
	pixelX = pX;
	pixelY = pY;
	movementSpeed = moveSpeed;
	jumpTime = jTime;
	jumpSpeed = jSpeed;
	numFrames = nFrames;
	frame = new Image [numFrames];
	animSpeed = animSpd;

	{ //Load frames for chip
		frame[0].load("_Assets/ChipGame/gfx/frame1.gfx");
		frame[1].load("_Assets/ChipGame/gfx/frame2.gfx");
		frame[2].load("_Assets/ChipGame/gfx/frame3.gfx");
		frame[3].load("_Assets/ChipGame/gfx/frame4.gfx");
		frame[4].load("_Assets/ChipGame/gfx/frame5.gfx");
		frame[5].load("_Assets/ChipGame/gfx/frame6.gfx");
		frame[6].load("_Assets/ChipGame/gfx/frame7.gfx");
		frame[7].load("_Assets/ChipGame/gfx/frame8.gfx");
		frame[8].load("_Assets/ChipGame/gfx/frame9.gfx");
		frame[9].load("_Assets/ChipGame/gfx/frame10.gfx"); }
}


void Player::animatePlayer( Str type )
{
	//Animation
	if ( type == "run" ) {
		if(frameCounter<(numFrames-1))
			frameCounter += animSpeed;
		else
			frameCounter=0; 
	}
	//Rapid cycle (if unity animSpeed is rapid) to stop animation
	else if (type == "stop" ) {
		if(frameCounter<(numFrames-1))
			frameCounter++; 
	}
}