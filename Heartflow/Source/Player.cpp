#include "stdafx.h"
#include "Player.h"
#include "main.hpp"

int collEdgee;

void Player::drawPlayer( )
{
	// If Player if facing left, flip him, otherwise, draw him normally
	currentFrame->draw(drawBox);
	//collBox.draw(BLUE);
}

//1 = move left, 2 = move right, 3 = stop, 4 = jump
void Player::movePlayer( int moveType, Pulse & pPulse )
{
	bool noRight = false;
	bool noLeft = false;

	if( collEdgee == 0 || collEdgee == 2 || collEdgee == 4 ) {
		noRight = true;
		noLeft = false;
		x -= pPulse.pSpeed;
	} else if ( collEdgee == 1 || collEdgee == 3 ) {
		noRight = false;
		noLeft = true;
	} else {
		noRight = false;
		noLeft = false;
	}

	switch (moveType) {
	case 1:
		this->facingLeft = true;
		if(noLeft == false) {
			this->x -= Time.d()*this->movementSpeed;

			//If trying to move off-screen, stop him


			//Animation
			if(this->frameCounter<(this->numFrames-1))
				this->frameCounter += this->animSpeed;
			else
				this->frameCounter=0;
		}
		break;

	case 2:
		this->facingLeft = false;
		if(noRight == false) {
			this->x += Time.d()*this->movementSpeed;

			//If trying to move off-screen, stop him
			if(this->x+2*(Flt) this->pixelX/RES_Y > RES_X/RES_Y)
				this->x = RES_X/RES_Y-2*this->pixelX/RES_Y;

			//Animation
			if(this->frameCounter<(this->numFrames-1))
				this->frameCounter += this->animSpeed;
			else
				this->frameCounter=0;
		}
		break;

	case 3:
		//Rapid cycle (if unity animSpeed is rapid) to stop animation
		if(this->frameCounter<(this->numFrames-2))
			this->frameCounter++;
		break;

	case 4:
		// Set jumping and let handleJump take care of the rest
		if(checkCollision(pPulse, collBox)) 
			this->jumping = true;
		break;
	}
}

void Player::handleJump(Pulse & pPulse)
{
	static int fallCount = 0;

	if(jumping && jumpCount < jumpTime) { 
		y += (jumpTime - jumpCount) * (jumpTime - jumpCount) * jumpSpeed; 
		jumpCount++;
	} else if(jumping && jumpCount >= jumpTime) {
		jumping = false;
	} else if(!jumping && !checkCollision(pPulse, collBox)) {
		fallCount++;
		y -= fallCount * fallCount * .0001f;
	} else {
		fallCount = 0;	
		jumping = false;
		jumpCount = 0;
	}
}

void Player::handleInput( KB_BUTTON jump, KB_BUTTON left, KB_BUTTON right, Pulse & pPulse )
{
	//Call movePlayer to handle movement based on given input

	if(Kb.b(KB_1)) {
		changeChar(0);
		characterID = 0;
	} else if(Kb.b(KB_2)) {
		changeChar(1);
		characterID = 1;
	} else if(Kb.b(KB_3)) {
		changeChar(2);
		characterID = 2;
	} else if(Kb.b(KB_4)) {
		changeChar(3);
		characterID = 3;
	} else if((Kb.b(jump) || Kb.b(KB_W) || Kb.b(KB_SPACE)) && this->jumpCount == 0) {
		movePlayer(4, pPulse);
	} else if(Kb.b(left) || Kb.b(KB_A)) {
		movePlayer(1, pPulse);
	} else if(Kb.b(right) || Kb.b(KB_D)) {
		movePlayer(2, pPulse);
	}
	else {
		movePlayer(3, pPulse);
	}
}

void Player::changeChar ( int charrID ) {

	switch (charrID) {

	case 0:
	default:
		this->frame[0].load("HeartGame/gfx/mofo01.gfx");
		this->frame[1].load("HeartGame/gfx/mofo00.gfx");
		this->frame[2].load("HeartGame/gfx/mofo01.gfx");
		this->frame[3].load("HeartGame/gfx/mofo00.gfx");
		this->frame[4].load("HeartGame/gfx/mofo01.gfx");
		this->frame[5].load("HeartGame/gfx/mofo00.gfx");
		this->frame[6].load("HeartGame/gfx/mofo01.gfx");
		this->frame[7].load("HeartGame/gfx/mofo00.gfx");
		this->frame[8].load("HeartGame/gfx/mofo01.gfx");
		this->frame[9].load("HeartGame/gfx/mofo00.gfx");
		this->frame[10].load("HeartGame/gfx/mofo00.gfx");
		this->pixelX = 53;
		this->pixelY = 106;
		this->movementSpeed = 1.2f;
		this->jumpTime = 20;
		this->jumpSpeed = .0001f;
		break;

	case 1:
		this->frame[0].load("HeartGame/gfx/bat01.gfx");
		this->frame[1].load("HeartGame/gfx/bat00.gfx");
		this->frame[2].load("HeartGame/gfx/bat01.gfx");
		this->frame[3].load("HeartGame/gfx/bat00.gfx");
		this->frame[4].load("HeartGame/gfx/bat01.gfx");
		this->frame[5].load("HeartGame/gfx/bat00.gfx");
		this->frame[6].load("HeartGame/gfx/bat01.gfx");
		this->frame[7].load("HeartGame/gfx/bat00.gfx");
		this->frame[8].load("HeartGame/gfx/bat01.gfx");
		this->frame[9].load("HeartGame/gfx/bat00.gfx");
		this->frame[10].load("HeartGame/gfx/bat00.gfx");
		this->pixelX = 119/2;
		this->pixelY = 220/2;
		this->movementSpeed = 0.9f;
		this->jumpTime = 18;
		this->jumpSpeed = .000175;
		break;

	case 2:
		this->frame[0].load("HeartGame/gfx/sea01.gfx");
		this->frame[1].load("HeartGame/gfx/sea00.gfx");
		this->frame[2].load("HeartGame/gfx/sea01.gfx");
		this->frame[3].load("HeartGame/gfx/sea00.gfx");
		this->frame[4].load("HeartGame/gfx/sea01.gfx");
		this->frame[5].load("HeartGame/gfx/sea00.gfx");
		this->frame[6].load("HeartGame/gfx/sea01.gfx");
		this->frame[7].load("HeartGame/gfx/sea00.gfx");
		this->frame[8].load("HeartGame/gfx/sea01.gfx");
		this->frame[9].load("HeartGame/gfx/sea00.gfx");
		this->frame[10].load("HeartGame/gfx/sea00.gfx");
		this->pixelX = 116/2;
		this->pixelY = 202/2;
		this->movementSpeed = 1.1f;
		this->jumpTime = 15;
		this->jumpSpeed = .000225;
		break;

	case 3:
		this->frame[0].load("HeartGame/gfx/mofo01.gfx");
		this->frame[1].load("HeartGame/gfx/mofo00.gfx");
		this->frame[2].load("HeartGame/gfx/mofo01.gfx");
		this->frame[3].load("HeartGame/gfx/mofo00.gfx");
		this->frame[4].load("HeartGame/gfx/mofo01.gfx");
		this->frame[5].load("HeartGame/gfx/mofo00.gfx");
		this->frame[6].load("HeartGame/gfx/mofo01.gfx");
		this->frame[7].load("HeartGame/gfx/mofo00.gfx");
		this->frame[8].load("HeartGame/gfx/mofo01.gfx");
		this->frame[9].load("HeartGame/gfx/mofo00.gfx");
		this->frame[10].load("HeartGame/gfx/mofo00.gfx");
		this->pixelX = 53;
		this->pixelY = 106;
		this->movementSpeed = movementSpeedd;
		this->jumpTime = JUMPTIME;
		this->jumpSpeed = JUMPSPEED;
		break;

	}
}

void Player::playerUpdate( KB_BUTTON jump, KB_BUTTON left, KB_BUTTON right, Pulse & pPulse, int collEdg )
{
	collEdgee = collEdg;
	//Check for key presses then move player
	handleInput( jump, left, right, pPulse );
	//Handle jumping if needed
	handleJump(pPulse);
	//Update the player drawBox
	if(this->facingLeft) {
		drawBox = Rect(this->x+2*this->pixelX/RES_Y, this->y, this->x, this->y+2*this->pixelY/RES_Y);
	} else {
		drawBox = Rect(this->x, this->y,this->x+2*this->pixelX/RES_Y, this->y+2*this->pixelY/RES_Y);
	}
	collBox = Rect(this->x+(collBox.max.x - collBox.min.x) / 5.0f, this->y,this->x+2*this->pixelX/RES_Y - (collBox.max.x - collBox.min.x) / 5.0f, this->y+2*this->pixelY/RES_Y);
	//Update animation frame
	this->currentFrame = &this->frame[(int) this->frameCounter];

	if(this->y < (-1.0f))
		gameOver = true;
}

// Initializes all instance variables
void Player::initPlayer( int pX, int pY, float moveSpeed, Flt jSpeed, int jTime, int nFrames, Flt animSpd, int charID )
{
	this->x = -RES_X/RES_Y * 0.9f; //Start at the ~edge of the screen
	this->y = -0.1f;		//Start on the top of the floor
	this->jumping = false;
	this->jumpCount = 0;
	this->facingLeft = false;
	this->pixelX = pX;
	this->pixelY = pY;
	this->movementSpeed = moveSpeed;
	this->jumpTime = jTime;
	this->jumpSpeed = jSpeed;
	this->numFrames = nFrames;
	this->frame = new Image [numFrames];
	this->animSpeed = animSpd;
	this->gameOver = false;

	switch (charID) {

	case 0:
	default:
		this->frame[0].load("HeartGame/gfx/mofo01.gfx");
		this->frame[1].load("HeartGame/gfx/mofo00.gfx");
		this->frame[2].load("HeartGame/gfx/mofo01.gfx");
		this->frame[3].load("HeartGame/gfx/mofo00.gfx");
		this->frame[4].load("HeartGame/gfx/mofo01.gfx");
		this->frame[5].load("HeartGame/gfx/mofo00.gfx");
		this->frame[6].load("HeartGame/gfx/mofo01.gfx");
		this->frame[7].load("HeartGame/gfx/mofo00.gfx");
		this->frame[8].load("HeartGame/gfx/mofo01.gfx");
		this->frame[9].load("HeartGame/gfx/mofo00.gfx");
		this->frame[10].load("HeartGame/gfx/mofo00.gfx");
		break;

	case 1:
		this->frame[0].load("HeartGame/gfx/bat01.gfx");
		this->frame[1].load("HeartGame/gfx/bat00.gfx");
		this->frame[2].load("HeartGame/gfx/bat01.gfx");
		this->frame[3].load("HeartGame/gfx/bat00.gfx");
		this->frame[4].load("HeartGame/gfx/bat01.gfx");
		this->frame[5].load("HeartGame/gfx/bat00.gfx");
		this->frame[6].load("HeartGame/gfx/bat01.gfx");
		this->frame[7].load("HeartGame/gfx/bat00.gfx");
		this->frame[8].load("HeartGame/gfx/bat01.gfx");
		this->frame[9].load("HeartGame/gfx/bat00.gfx");
		this->frame[10].load("HeartGame/gfx/bat00.gfx");
		break;

	case 2:
		this->frame[0].load("HeartGame/gfx/sea01.gfx");
		this->frame[1].load("HeartGame/gfx/sea00.gfx");
		this->frame[2].load("HeartGame/gfx/sea01.gfx");
		this->frame[3].load("HeartGame/gfx/sea00.gfx");
		this->frame[4].load("HeartGame/gfx/sea01.gfx");
		this->frame[5].load("HeartGame/gfx/sea00.gfx");
		this->frame[6].load("HeartGame/gfx/sea01.gfx");
		this->frame[7].load("HeartGame/gfx/sea00.gfx");
		this->frame[8].load("HeartGame/gfx/sea01.gfx");
		this->frame[9].load("HeartGame/gfx/sea00.gfx");
		this->frame[10].load("HeartGame/gfx/sea00.gfx");
		break;

	case 3:
		this->frame[0].load("HeartGame/gfx/mofo01.gfx");
		this->frame[1].load("HeartGame/gfx/mofo00.gfx");
		this->frame[2].load("HeartGame/gfx/mofo01.gfx");
		this->frame[3].load("HeartGame/gfx/mofo00.gfx");
		this->frame[4].load("HeartGame/gfx/mofo01.gfx");
		this->frame[5].load("HeartGame/gfx/mofo00.gfx");
		this->frame[6].load("HeartGame/gfx/mofo01.gfx");
		this->frame[7].load("HeartGame/gfx/mofo00.gfx");
		this->frame[8].load("HeartGame/gfx/mofo01.gfx");
		this->frame[9].load("HeartGame/gfx/mofo00.gfx");
		this->frame[10].load("HeartGame/gfx/mofo00.gfx");
		this->movementSpeed = movementSpeedd;
		this->jumpTime = JUMPTIME;
		this->jumpSpeed = JUMPSPEED;
		break;

	}

}


//***************             BUCKET FUNCTIONS                      ************//
void Player::initBucket( int pX, int pY )
{
	b_x = x;
	b_y = y;
	b_pixelX = pX;
	b_pixelY = pY;
	
	bucket.load("HeartGame/gfx/bowl00.gfx");
}

void Player::updateBucket()
{
	b_x = x;
	b_y = y;
	if(this->facingLeft) {
		b_drawBox = Rect(drawBox.min.x+161/RES_Y - 2*(-drawBox.max.x+drawBox.min.x), drawBox.max.y-175/RES_Y, drawBox.min.x+75/RES_Y-2*(-drawBox.max.x+drawBox.min.x), drawBox.max.y-125/RES_Y);
		b_collBox = Rect(collBox.min.x+75/RES_Y - 1.5*(-drawBox.max.x+drawBox.min.x), collBox.max.y-175/RES_Y, collBox.min.x+161/RES_Y - 1.5*(-drawBox.max.x+drawBox.min.x), collBox.max.y-125/RES_Y);
	} else {
		b_drawBox = Rect(drawBox.min.x+75/RES_Y, drawBox.max.y-175/RES_Y, drawBox.min.x+161/RES_Y, drawBox.max.y-125/RES_Y);
		b_collBox = Rect(collBox.min.x+75/RES_Y, collBox.max.y-175/RES_Y, collBox.min.x+161/RES_Y, collBox.max.y-125/RES_Y);}
}

void Player::drawBucket()
{
	bucket.draw(b_drawBox);
}
