#include "stdafx.h"
#include "main.h"
#include "mathFunctions.h"


Flt convertY(Flt y)
{
	return ((RES_Y/2.0f)-y)/(RES_Y/2.0f);
	//return (384.0f-y)/384.0f;
}

Flt convertX(Flt x)
{
	return (x-(RES_X/2.0f))/(RES_Y/2.0f);
	//return (x-683.0f)/384.0f;
}