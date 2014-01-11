#include "stdafx.h"
#include "main.hpp"
#include "mathFunctions.h"


Flt convertY(Flt y)
{
	return (RES_Y/2-y)/RES_Y/2;
}

Flt convertX(Flt x)
{
	return (x-RES_X/2)/RES_Y/2;
}

Flt convertYR(Flt y)
{
	return (RES_Y-y)/RES_Y;
}

Flt convertXR(Flt x)
{
	return (x/RES_X - 0.5f) * RES_X/RES_Y;
}