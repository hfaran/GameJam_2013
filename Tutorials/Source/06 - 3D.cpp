
#include "stdafx.h"

void InitPre()
{
   App.name("3D");
   App.flag=APP_MS_EXCLUSIVE; // setup mouse exclusive mode to hide cursor
   Paks.add("../data/engine.pak");
}

Bool Init()
{
   Cam.dist=5; // set initial camera distance to 5 meters
   return true;
}

void Shut()
{
}

Bool Update()
{
   if(Kb.bp(KB_ESC))return false;

   CamHandle(0.1f, 10, CAMH_ROT|CAMH_ZOOM); // simple camera handle allowing minimum 0.1 and maximum 10 meters zoom, and allowing rotation and zooming

   return true;
}

void Draw()
{
   D.clear(WHITE);

   SetMatrix(MatrixIdentity); // set matrix before drawing

              Box (3                   ).draw(BLACK); // draw black box with 1 meter radius
   if(Ms.b(0))Ball(1   ,    Vec(-3,0,0)).draw(BLACK); // when 0th mouse button pushed draw black ball with 1   meter radius and (-3,0,0) position
   if(Ms.b(1))Tube(0.3f, 2, Vec( 3,0,0)).draw(BLACK); // when 1st mouse button pushed draw black tube with 0.1 meter radius, 2 meters height and (3,0,0) position
}

