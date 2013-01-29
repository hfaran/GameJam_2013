#include "stdafx.h"

#ifndef PLAYER_H
#define PLAYER_H

struct Player {
	Flt x; //x-coordinate of Player
	Flt y; //y-coordinate of Player
	int pixelX; //X-dimension of the frames in pixels
	int pixelY; //Y-dimension of the frames in pixels

	bool jumping; 
	int jumpCount;
	bool falling;
	int jumpTime;//Number of frames the jump lasts
	Flt jumpSpeed;//The distance the jump moves during one frame 
					//(not exactly, is used in math to have a parabolic jump)

	bool facingLeft; //Indicates whether to flip when drawing
	Flt movementSpeed; //movement speed of the Player (default = 1.0f)
	
	Image *currentFrame; //pointer to current frame of animation
	int numFrames;		//total # of animation frames
	Flt frameCounter; //index of frame array for the current frame of animation
	Image * frame; //Dynamic array of images holding the animations
	Flt animSpeed; //speed of animation cycle (default 0.3333)

	
	void initPlayer( int pX, int pY, float moveSpeed=1.0f, 
					Flt jSpeed=0.0001f, int jTime=25, int nFrames=10, Flt animSpd=0.3333 );
	void handleInput( KB_BUTTON jump, KB_BUTTON left, KB_BUTTON right );
	void playerUpdate( KB_BUTTON jump, KB_BUTTON left, KB_BUTTON right );
	void movePlayer( int moveType ); //1 = move left, 2 = move right, 3 = stop, 4 = jump
	void animatePlayer( Str type );
	void handleJump(  );
	void drawPlayer(  );
};




#endif

