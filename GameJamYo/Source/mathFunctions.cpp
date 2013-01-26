#include "stdafx.h"
#include "main.h"
#include "mathFunctions.h"


Flt convertY(Flt y)
{
	return (RES_Y/2-y)/RES_Y/2;
}

Flt convertX(Flt x)
{
	return (x-RES_X/2)/RES_Y/2;
}