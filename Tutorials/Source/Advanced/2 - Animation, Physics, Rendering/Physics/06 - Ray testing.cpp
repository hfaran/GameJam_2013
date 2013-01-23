/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Actor ground, ball;
/******************************************************************************/
void InitPre()
{
   App.name("Ray Testing");
   App.flag=APP_MS_EXCLUSIVE;
   Paks.add("../data/engine.pak");
}
Bool Init()
{
   Cam.dist=4;
   Physics.create(CSS_NONE, true, "../Installation/PhysX");

   ground.create(Box (15, 1, 15, Vec(0,-2,0)), 0);
   ball  .create(Ball(1, Vec(0,3,0)));

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

   if(Kb.bp(KB_SPACE))ball.addVel(Vec(0,5,0)); // add velocity to the ball when space pressed

   return true;
}
/******************************************************************************/
void Draw()
{
   D      .clear();
   Physics.draw ();

   // test if custom ray hits some actor
   {
      Vec     start( 4,0,0), // starting position of ray
              end  (-4,0,0); // ending   position of ray
      PhysHit phys_hit     ; // phys-hit object for receiving hit parameters if any

      if(Physics.ray(start, end-start, &phys_hit)) // if ray hit something
      {
         D.line(RED  , start, phys_hit.plane.pos);                                         // draw a red   line from staring position to hit position
         D.line(GREEN, phys_hit.plane.pos, phys_hit.plane.pos+phys_hit.plane.normal*0.3f); // draw a green line presenting the hit normal vector
         D.dot (RED  , phys_hit.plane.pos);                                                // draw the contact point
      }else
      {
         D.line(BLUE, start, end); // draw a blue line from starting position to end position
      }
   }
}
/******************************************************************************/
