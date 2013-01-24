#include "stdafx.h"
#include <iostream>
#include <string>
#include <sstream>

#define RES_Y 768.0f
#define RES_X 1366.0f

// Structs
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
};

// Function prototypes
Flt convertY(Flt y);
Flt convertX(Flt x);
void initPlayer( Player &p, int pX, int pY, Flt moveSpeed=1.0f );
void initPlatform( );
bool collision(Rect A, Rect B);
bool onTop(Rect A, Rect B);
//These functions should be methods, maybe?
void handleInput( Player &p, KB_BUTTON jump, KB_BUTTON left, KB_BUTTON right );
void currentFrameUpdate( Player &p, KB_BUTTON jump, KB_BUTTON left, KB_BUTTON right );
void movePlayer( int moveType, Player &p ); //1 = move left, 2 = move right, 3 = stop, 4 = jump
void handleJump( Player &p );
void drawPlayer( Player &p );
//Invisible Assets
MusicTheme mtIdle;
// Objects
Image bg; // image object
Player chip;
Rect ground, platform[3];

//---------------------Engine-Functions--------------------------------//

void InitPre()
{
	App.name("Game_001");
	Paks.add("_Assets/engine.pak");
	D.mode(RES_X,RES_Y);
}

Bool Init()
{
	bg.load("_Assets/ChipGame/lololol.gfx"); // load bg

	{ //Load frames for chip
	chip.frame[0].load("_Assets/ChipGame/frame1.gfx");
	chip.frame[1].load("_Assets/ChipGame/frame2.gfx");
	chip.frame[2].load("_Assets/ChipGame/frame3.gfx");
	chip.frame[3].load("_Assets/ChipGame/frame4.gfx");
	chip.frame[4].load("_Assets/ChipGame/frame5.gfx");
	chip.frame[5].load("_Assets/ChipGame/frame6.gfx");
	chip.frame[6].load("_Assets/ChipGame/frame7.gfx");
	chip.frame[7].load("_Assets/ChipGame/frame8.gfx");
	chip.frame[8].load("_Assets/ChipGame/frame9.gfx");
	chip.frame[9].load("_Assets/ChipGame/frame10.gfx"); }

	if(!mtIdle.songs()){
	  mtIdle+="_Assets/ChipGame/sound/LXTronic.ogg"; 
	}

	initPlayer(chip, 93, 134);
	initPlatform();

	return true;
}

void Shut()
{
}

Bool Update()
{

	if(Kb.bp(KB_ESC))return false;

	currentFrameUpdate( chip, KB_UP, KB_LEFT, KB_RIGHT );
	Music.play(mtIdle);

	return true;
}

void Draw()
{
	D.clear(BLACK);

	// draw background
	bg.draw(Rect((Flt) -RES_X/RES_Y, -1.0f, (Flt) RES_X/RES_Y, 1.0f));

	drawPlayer( chip );
}

//---------------Non-engine Functions-----------------------------//

bool collision(Rect A, Rect B)
{
	if(A.max.y>B.max.y && A.min.y<B.max.y)
		if(A.max.x>B.max.x && A.min.x<B.max.x)
			return true;
	if(A.max.y>B.min.y && A.min.y<B.min.y)
		if(A.max.x>B.min.x && A.min.x<B.min.x)
			return true;
	if(A.max.y>B.min.y && A.min.y<B.min.y)
		if(A.max.x>B.max.x && A.min.x<B.max.x)
			return true;
	if(A.max.y>B.max.y && A.min.y<B.max.y)
		if(A.max.x>B.min.x && A.min.x<B.min.x)
			return true;

	return false;
}

bool onTop(Rect A, Rect B)
{
	if(B.max.y>A.max.y)
		return true;
	else
		return false;
}

void initPlayer( Player &p, int pX, int pY, Flt moveSpeed )
{
	p.x = -RES_X/RES_Y * 0.9f;
	p.y = -0.75f;
	p.jumping = false;
	p.jumpCount = 0;
	p.facingLeft = false;
	p.falling=true;
	p.pixelX = pX;
	p.pixelY = pY;
	p.movementSpeed = moveSpeed;
}

void initPlatform( )
{
	ground = Rect(convertX(0), convertY(664), convertX(1366), convertY(768));
	platform[0] = Rect(convertX(469), convertY(497), convertX(296), convertY(543));
	platform[1] = Rect(convertX(1366), convertY(357), convertX(1019), convertY(405));
	platform[2] = Rect(convertX(1366), convertY(497), convertX(727), convertY(543));
}

Flt convertY(Flt y)
{
	return (384.0-y)/384.0;
}

Flt convertX(Flt x)
{
	return (x-683.0)/384.0;
}

void handleInput( Player &p, KB_BUTTON jump, KB_BUTTON left, KB_BUTTON right )
{
	if(Kb.b(jump) && p.jumpCount == 0) {
		movePlayer(4, p);
	}
	if(Kb.b(left)) {
		movePlayer(1, p);
	}
	else if(Kb.b(right)) {
		movePlayer(2, p);
	}
	else {
		movePlayer(3, p);
	}
}

void currentFrameUpdate( Player &p, KB_BUTTON jump, KB_BUTTON left, KB_BUTTON right )
{
	handleInput( p, jump, left, right );
	handleJump( p );
	p.currentFrame = &p.frame[(int) p.frameCounter];
}

void handleJump( Player &p )
{
	if(p.jumping && p.jumpCount < 25) {
		p.y += .0225f;
		p.jumpCount++;
	} else if(!p.jumping && p.jumpCount > 0) {
		p.y -= .0225f; //Jump Speed
		p.jumpCount--;
	} else if(p.jumping && p.jumpCount >= 25) {
		p.jumping = false;
	}
}

//1 = move left, 2 = move right, 3 = stop, 4 = jump
void movePlayer( int moveType, Player &p )
{
	switch (moveType) {
	case 1:
		p.facingLeft = true;
		p.x -= Time.d()*p.movementSpeed;

		if(p.x < (Flt) -RES_X/RES_Y)
			p.x = (Flt) -RES_X/RES_Y;

		if(p.frameCounter<9)
			p.frameCounter += 0.3333;
		else
			p.frameCounter=0;
		break;
	case 2:
		p.facingLeft = false;
		p.x += Time.d()*p.movementSpeed;

		if(p.x+2*(Flt) p.pixelX/RES_Y > RES_X/RES_Y)
			p.x = RES_X/RES_Y-2*p.pixelX/RES_Y;

		if(p.frameCounter<9)
			p.frameCounter += 0.3333;
		else
			p.frameCounter=0;
		break;
	case 3:
		if(p.frameCounter<9)
			p.frameCounter++;
		break;
	case 4:
		p.jumping = true;
		break;
	}
}

void drawPlayer( Player &p )
{
	if(p.facingLeft) {
		p.currentFrame->draw(Rect(p.x+2*p.pixelX/RES_Y, p.y, p.x, p.y+2*p.pixelY/RES_Y));
	} else {
		p.currentFrame->draw(Rect(p.x, p.y, p.x+2*p.pixelX/RES_Y, p.y+2*p.pixelY/RES_Y));
	}
}