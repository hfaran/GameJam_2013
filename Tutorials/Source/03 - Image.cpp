#include "stdafx.h"
#include <iostream>
#include <string>
#include <sstream>

// Structs;
struct Player {
	Flt x;
	Flt y;
	Image *player;
	bool jumping;
	int jumpCount;
	bool facingLeft;
	bool falling;
};

// Function prototypes
Flt convertY(Flt y);
Flt convertX(Flt x);
void initPlayer( Player &p );
void initPlatform( );
bool collision(Rect A, Rect B);
bool onTop(Rect A, Rect B);
void playerInput( Player &p );
void playerUpdate( Player &p );


// Variables
Image bg; // image object
Image frame[10];
Flt frameCounter;
Player chip;
Rect ground, platform[3];

void InitPre()
{
	App.name("Image");
	Paks.add("_Assets/engine.pak");
	D.mode(1366,768);
}

Bool Init()
{
	bg.load("_Assets/ChipGame/lololol.gfx"); // load image from file
	frame[0].load("_Assets/ChipGame/frame1.gfx");
	frame[1].load("_Assets/ChipGame/frame2.gfx");
	frame[2].load("_Assets/ChipGame/frame3.gfx");
	frame[3].load("_Assets/ChipGame/frame4.gfx");
	frame[4].load("_Assets/ChipGame/frame5.gfx");
	frame[5].load("_Assets/ChipGame/frame6.gfx");
	frame[6].load("_Assets/ChipGame/frame7.gfx");
	frame[7].load("_Assets/ChipGame/frame8.gfx");
	frame[8].load("_Assets/ChipGame/frame9.gfx");
	frame[9].load("_Assets/ChipGame/frame10.gfx");

	initPlayer(chip);
	initPlatform();

	return true;
}

void Shut()
{
}

Bool Update()
{

	if(Kb.bp(KB_ESC))return false;

	playerInput( chip );
	playerUpdate( chip );

	return true;
}

void Draw()
{
	D.clear(BLACK);


	// draw image
	bg.draw(Rect(-1.778645833333f, -1.0f, 1.778645833333f, 1.0f)); // draw at given rectangle
	if(chip.facingLeft) {
		chip.player->draw(Rect(chip.x+2*121.094E-3f, chip.y, chip.x, chip.y+2*174.479E-3));
	} else {
		chip.player->draw(Rect(chip.x, chip.y, chip.x+2*121.094E-3f, chip.y+2*174.479E-3));
	}

	/*ground.draw(BLUE, true);
	platform[0].draw(BLUE, true);
	platform[1].draw(BLUE, true);
	platform[2].draw(BLUE, true);*/
}



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

void initPlayer( Player &p )
{
	p.x = -121.094E-3f;
	p.y = -0.75f;
	p.jumping = false;
	p.jumpCount = 0;
	p.facingLeft = false;
	p.falling=true;
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

void playerInput( Player &p )
{
	
	if(Kb.b(KB_UP) && p.jumpCount == 0) {
		p.jumping = true;
	}

	if(Kb.b(KB_LEFT)) {
		p.facingLeft = true;
		p.x -= Time.d()/2;

		if(p.x < -1.778645833333f)
			p.x = -1.778645833333f;

		if(frameCounter<9)
			frameCounter += 0.3333;
		else
			frameCounter=0; 
	}
	else if(Kb.b(KB_RIGHT)) {
		p.facingLeft = false;
		p.x += Time.d()/2;

		if(p.x+2*121.094E-3f > 1.778645833333f)
			p.x = 1.778645833333f-2*121.094E-3f;
		
		if(frameCounter<9)
			frameCounter += 0.3333;
		else
			frameCounter=0; 
	}
	else {
		if(frameCounter<9)
			frameCounter++; 
	}
}

void playerUpdate( Player &p )
{
	if(p.jumping && p.jumpCount < 35) {
		p.y += .015f;
		p.jumpCount++;
	} else if(!p.jumping && p.jumpCount > 0) {
		p.y -= .015f; //Jump Speed
		p.jumpCount--;
	} else if(p.jumping && p.jumpCount >= 35) {
		p.jumping = false;
	}

	p.player = &frame[(int) frameCounter];
}