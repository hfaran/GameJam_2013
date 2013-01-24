#include "stdafx.h"

#ifndef PLAYER_H
#define PLAYER_H

struct Player {
	Flt x; //x-coordinate of Player
	Flt y; //y-coordinate of Player
	Image *currentFrame; //pointer to current frame of animation
	bool jumping; 
	int jumpCount;
	bool facingLeft; //Indicates whether to flip when drawing
	bool falling;
	Flt frameCounter; //index of frame array for the current frame of animation
	Image frame[10]; //Array of images holding the animations
	int pixelX; //X-dimension of the frames in pixels
	int pixelY; //Y-dimension of the frames in pixels
	Flt movementSpeed; //movement speed of the Player (default = 1.0f)
	int jumpTime;//Number of frames the jump lasts
	Flt jumpSpeed;//The distance the jump moves during one frame (not exactly, is used in math to have a parabolic jump)

	void initPlayer( int pX, int pY, float moveSpeed=1.0f, Flt jSpeed=0.0001f, int jTime=25 );
	void handleInput( KB_BUTTON jump, KB_BUTTON left, KB_BUTTON right );
	void playerUpdate( KB_BUTTON jump, KB_BUTTON left, KB_BUTTON right );
	void movePlayer( int moveType ); //1 = move left, 2 = move right, 3 = stop, 4 = jump
	void handleJump(  );
	void drawPlayer(  );
};




#endif

