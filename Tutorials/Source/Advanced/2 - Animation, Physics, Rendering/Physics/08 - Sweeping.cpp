/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Actor ground, object;
/******************************************************************************/
void InitPre()
{
   App.name("Sweeping");
   App.flag=APP_MS_EXCLUSIVE;
   Paks.add("../data/engine.pak");
}
Bool Init()
{
   Cam.dist=8;
   Physics.create(CSS_NONE, true, "../Installation/PhysX");

   ground.create(Box (8, 1, 8, Vec(0,-2,0)), 0);
   object.create(Ball(1, Vec(0,1,0)));

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
   CamHandle(0.1f, 30, CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT)); // move camera on right mouse button

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
      Ball    ball(0.7f, Vec(0,3,0));
      Vec     move(Sin(Time.time())*3, -3, 0);
      PhysHit phys_hit;
      Bool    collision=Physics.sweep(ball, move, &phys_hit);
      
      ball.draw(YELLOW); // draw ball at original position

      (ball+move).draw(GREY); // draw ball at desired position

      if(collision)move*=phys_hit.frac; // limit movement if collision encountered

      D.line(WHITE, ball.pos, ball.pos+move); // draw the movement line

      (ball+move).draw(collision ? RED : BLUE); // draw ball at destination position
   }
}
/******************************************************************************/
