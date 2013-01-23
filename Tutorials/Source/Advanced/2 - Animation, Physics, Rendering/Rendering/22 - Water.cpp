/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Mesh terrain;
/******************************************************************************/
void InitPre()
{
   App.name("Water");
   App.flag=APP_FULL_TOGGLE|APP_MS_EXCLUSIVE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true).ambientPower(0.4).hpRt(true);

   Cam.at.set(0,2,0);
   Cam.pitch=-0.5f;
   Cam.dist =30;
   Cam.yaw  =2.8f;
}
Bool Init()
{
   terrain.load("obj/terrain/0.mesh");                                      // terrain
   terrain.scale(Vec(1,10,1)).move(Vec(0,5,0));                             // scale and move terrain vertically
   Sky.atmospheric();                                                       // sky
   Sun.image="Env/Sky/sun.gfx"; Sun.light_color=1-D.ambientColor(); // sun

   Water.images(Images("Env/Water/0.gfx"), Images("Env/Water/0.n.gfx"), Images("Env/Water/reflection.gfx")); // set water from textures
   Water.draw      =true; // enable drawing
   Water.wave_scale=0.8f; // adjust wave scales

   return true;
}
void Shut()
{
}
/******************************************************************************/
Bool Update()
{
   if(Kb.bp(KB_ESC))return false;
   CamHandle(0.01f, 500, CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT));

   // here you can optionally set different water surface plane
   if(Kb.b(KB_UP  ))Water.plane+=Vec(0,1,0)*Time.d(); // move water surface plane upwards
   if(Kb.b(KB_DOWN))Water.plane-=Vec(0,1,0)*Time.d(); // move water surface plane downwards

   // update water waves movement
   Water.update(Vec2(0.01f, 0.01f));

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE: terrain.draw(MatrixIdentity); break;
   }
}
void Draw()
{
   Renderer(Render);
   D.text(0, 0.9f, S+"Fps "+Time.fps());
   D.text(0, 0.8f, "Press up/down keys to change water level");
}
/******************************************************************************/
