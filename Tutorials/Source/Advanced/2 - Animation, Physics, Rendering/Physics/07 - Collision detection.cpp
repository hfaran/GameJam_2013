/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Actor ground,box;
/******************************************************************************/
void InitPre()
{
   App.name("Collision Detection");
   App.flag=APP_MS_EXCLUSIVE;
   Paks.add("../data/engine.pak");
}
Bool Init()
{
   Cam.dist=4;
   Physics.create(CSS_NONE, true, "../Installation/PhysX");

   ground.create(Box(15, 1, 15, Vec(0,-2,0)), 0);
   box   .create(Box(1, Vec(0,1,0)));

   return true;
}
/******************************************************************************/
void Shut()
{
}
/******************************************************************************/
Bool Update()
{
   if(Kb.bp(KB_ESC))return false;
   CamHandle(0.1f, 10, CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT)); // move camera on right mouse button

   Physics.startSimulation().stopSimulation();

   return true;
}
/******************************************************************************/
void Draw()
{
   D      .clear();
   Physics.draw ();

   // test if a shape collides with some actors
   {
      Ball ball(0.7f, Vec(Sin(Time.time())*3, 0, 0));
      Bool collision=Physics.cuts(ball);
      
      ball.draw(collision ? RED : BLUE);
   }
}
/******************************************************************************/
