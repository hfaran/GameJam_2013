#include "stdafx.h"
#include "Wave.h"
#include "dropArray.hpp"

#ifndef CUSTOMPHYSICS_H
#define CUSTOMPHYSICS_H

bool onTop(Rect A, Rect B);
bool checkCollision(Pulse &p, Rect collisionBox);
int checkCollisionEdge(Pulse &p, Rect collisionBox);
int checkDropsCollision(dropArray & dArray, Rect bucketBox);

#endif