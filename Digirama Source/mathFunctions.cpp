#include "stdafx.h"
#include "mathFunctions.h"

Flt convertY(Flt y)
{
	return (384.0-y)/384.0;
}

Flt convertX(Flt x)
{
	return (x-683.0)/384.0;
}