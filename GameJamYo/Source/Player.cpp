#include "stdafx.h"
#include "Player.h"
#include "main.h"

void Player::drawPlayer( )
{
	// If Player if facing left, flip him, otherwise, draw him normally
	if(this->facingLeft) {
		this->currentFrame->draw(Rect(this->x+2*this->pixelX/RES_Y, this->y,
										this->x, this->y+2*this->pixelY/RES_Y));
	} else {
		this->currentFrame->draw(Rect(this->x, this->y, 
										this->x+2*this->pixelX/RES_Y, this->y+2*this->pixelY/RES_Y));
	}
}

//1 = move left, 2 = move right, 3 = stop, 4 = jump
void Player::movePlayer( int moveType )
{
	switch (moveType) {
	case 1:
		this->facingLeft = true;
		this->x -= Time.d()*this->movementSpeed;

		//If trying to move off-screen, stop him
		if(this->x < (Flt) -RES_X/RES_Y)
			this->x = (Flt) -RES_X/RES_Y;

		//Animation
		if(this->frameCounter<(this->numFrames-1))
			this->frameCounter += this->animSpeed;
		else
			this->frameCounter=0;
		break;

	case 2:
		this->facingLeft = false;
		this->x += Time.d()*this->movementSpeed;

		//If trying to move off-screen, stop him
		if(this->x+2*(Flt) this->pixelX/RES_Y > RES_X/RES_Y)
			this->x = RES_X/RES_Y-2*this->pixelX/RES_Y;

		//Animation
		if(this->frameCounter<(this->numFrames-1))
			this->frameCounter += this->animSpeed;
		else
			this->frameCounter=0;
		break;

	case 3:
		//Rapid cycle (if unity animSpeed is rapid) to stop animation
		if(this->frameCounter<(this->numFrames-1))
			this->frameCounter++;
		break;

	case 4:
		// Set jumping and let handleJump take care of the rest
		this->jumping = true;
		break;
	}
}

void Player::handleJump( )
{
	if(this->jumping && this->jumpCount < this->jumpTime) { 
		this->y += (this->jumpTime - this->jumpCount) * (this->jumpTime - this->jumpCount) * this->jumpSpeed; 
		this->jumpCount++;
	} else if(!this->jumping && this->jumpCount > 0) {
		this->y -= (this->jumpTime + 1 - this->jumpCount) * (this->jumpTime + 1 - this->jumpCount) * this->jumpSpeed; 
		this->jumpCount--;
	} else if(this->jumping && this->jumpCount >= this->jumpTime) {
		this->jumping = false;
	}
}

void Player::handleInput( KB_BUTTON jump, KB_BUTTON left, KB_BUTTON right )
{
	//Call movePlayer to handle movement based on given input
	if(Kb.b(jump) && this->jumpCount == 0) {
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
	//Check for key presses then move player
	handleInput( jump, left, right );
	//Handle jumping if needed
	handleJump( );
	//Update animation frame
	this->currentFrame = &this->frame[(int) this->frameCounter];
}

// Initializes all instance variables
void Player::initPlayer( int pX, int pY, float moveSpeed, Flt jSpeed, int jTime, int nFrames, Flt animSpd )
{
	this->x = -RES_X/RES_Y * 0.9f; //Start at the ~edge of the screen
	this->y = -0.75f;		//Start on the top of the floor
	this->jumping = false;
	this->jumpCount = 0;
	this->facingLeft = false;
	this->falling=true;
	this->pixelX = pX;
	this->pixelY = pY;
	this->movementSpeed = moveSpeed;
	this->jumpTime = jTime;
	this->jumpSpeed = jSpeed;
	this->numFrames = nFrames;
	this->frame = new Image [numFrames];
	this->animSpeed = animSpd;
}


