#include "stdafx.h"
#include "main.hpp"
#include "Wave.h"

void Wave::initWave( Flt xPosition, Flt yPosition, Flt period, Flt Amp )
{
	xPos=xPosition;
	yPos = yPosition;
	t=period;
	amplitude=Amp;

	this->beat = new Edge2 [5];
	
	this->beat[0] = Edge2(this->xPos, this->yPos, 
		this->xPos+this->t/4.0, this->yPos+this->amplitude);
	this->beat[1] = Edge2(this->xPos+this->t/4.0, this->yPos+this->amplitude,
		this->xPos+this->t/2.0, this->yPos+this->amplitude*-1);
	this->beat[2] = Edge2(this->xPos+this->t/2.0, this->yPos+this->amplitude*-1,
		this->xPos+this->t/4.0*3.0, this->yPos+this->amplitude*0.33333);
	this->beat[3] = Edge2(this->xPos+this->t/4.0*3.0, this->yPos+this->amplitude*0.33333, this->xPos+this->t/4.0*3.5, this->yPos+this->amplitude*-0.33333);
	this->beat[4] = Edge2(this->xPos+this->t/4.0*3.5, this->yPos+this->amplitude*-0.33333, this->xPos+t, this->yPos);


}

void Wave::drawWave( )
{
	for( int i=0; i<5; i++ )
		this->beat[i].draw(RED);
}
