#include "stdafx.h"
#include "Player.h"
#include "main.h"

void Player::drawPlayer( )
{
	if(this->facingLeft) {
		this->currentFrame->draw(Rect(this->x+2*this->pixelX/RES_Y, this->y, this->x, this->y+2*this->pixelY/RES_Y));
	} else {
		this->currentFrame->draw(Rect(this->x, this->y, this->x+2*this->pixelX/RES_Y, this->y+2*this->pixelY/RES_Y));
	}
}

//1 = move left, 2 = move right, 3 = stop, 4 = jump
void Player::movePlayer( int moveType )
{
	switch (moveType) {
	case 1:
		this->facingLeft = true;
		this->x -= Time.d()*this->movementSpeed;

		if(this->x < (Flt) -RES_X/RES_Y)
			this->x = (Flt) -RES_X/RES_Y;

		if(this->frameCounter<9)
			this->frameCounter += 0.3333;
		else
			this->frameCounter=0;
		break;
	case 2:
		this->facingLeft = false;
		this->x += Time.d()*this->movementSpeed;

		if(this->x+2*(Flt) this->pixelX/RES_Y > RES_X/RES_Y)
			this->x = RES_X/RES_Y-2*this->pixelX/RES_Y;

		if(this->frameCounter<9)
			this->frameCounter += 0.3333;
		else
			this->frameCounter=0;
		break;
	case 3:
		if(this->frameCounter<9)
			this->frameCounter++;
		break;
	case 4:
		this->jumping = true;
		break;
	}
}

void Player::handleJump( )
{
	//These numbers should probably be definitions instead.

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
	if(Kb.b(jump) && this->jumpCount == 0) {
		movePlayer(4);
	}
	if(Kb.b(left)) {
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
	handleInput( jump, left, right );
	handleJump( );
	this->currentFrame = &this->frame[(int) this->frameCounter];
}

void Player::initPlayer( int pX, int pY, float moveSpeed, Flt jSpeed, int jTime )
{
	this->x = -RES_X/RES_Y * 0.9f;
	this->y = -0.75f;
	this->jumping = false;
	this->jumpCount = 0;
	this->facingLeft = false;
	this->falling=true;
	this->pixelX = pX;
	this->pixelY = pY;
	this->movementSpeed = moveSpeed;
	this->jumpTime = jTime;
	this->jumpSpeed = jSpeed;
}


