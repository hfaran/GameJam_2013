/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
void InitPre()
{
   App.name("Camera");
   App.flag=APP_MS_EXCLUSIVE;
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   Cam.dist=5;
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

   // settings
   Cam.yaw  -=Ms.d().x;                         // modify yaw      according to mouse delta x
   Cam.pitch+=Ms.d().y;                         // modify pitch    according to mouse delta y
   Cam.roll +=(Kb.b(KB_Z)-Kb.b(KB_X))*Time.d(); // modify roll     according to Z and X keyboard buttons
   Cam.dist -=Ms.wheel()*0.4f;                  // modify distance according to mouse wheel
   Clamp(Cam.dist, 0.1f, 10);                   // clamp  distance to minimum and maximum values

   if(Kb.b(KB_LEFT ))Cam.at-=Cam.matrix.x*Time.d(); // move camera left  on left  arrow key
   if(Kb.b(KB_RIGHT))Cam.at+=Cam.matrix.x*Time.d(); // move camera right on right arrow key
   if(Kb.b(KB_UP   ))Cam.at+=Cam.matrix.y*Time.d(); // move camera up    on up    arrow key
   if(Kb.b(KB_DOWN ))Cam.at-=Cam.matrix.y*Time.d(); // move camera down  on down  arrow key

   // camera
   Cam.setSpherical    (Cam.at, Cam.yaw, Cam.pitch, Cam.roll, Cam.dist); // set spherical camera with 'look at' position, angles and distance
   Cam.updateVelocities(                                              ); // after camera settings are up, we need to update camera velocities in order to achieve correct motion blur when enabled
   Cam.set             (                                              ); // set as active camera
   return true;
}
/******************************************************************************/
void Draw()
{
   D.clear(WHITE);
   
   // draw 4x4 boxes
   SetMatrix();
   REPD(x, 4)
   REPD(z, 4)Box(0.3f, Vec(x,0,z)).draw(BLACK);
}
/******************************************************************************/
