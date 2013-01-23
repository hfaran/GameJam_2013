/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Image logo;
/******************************************************************************/
void InitPre()
{
   App.name("Combining Render with Background");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");

   Cam.dist=1;
   Cam.yaw=PI;

   D.full(true).ambientPower(0);
}
/******************************************************************************/
Bool Init()
{
   logo.Import("logo.png", -1, IMAGE_2D, 1);
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

   CamHandle(0.1f, 100, CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT));

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         Meshes("obj/chr/warrior/body.mesh")->draw(MatrixIdentity);

         LightPoint(5, Vec(0,1,1)).add();
      }break;
   }
}
void Draw()
{
   // setup background
   D.clear();
   logo.drawFs();
   Rect_C(0, 0, D.w()*2, D.h()*2).draw(Color(255,255,255,192));

   // enable/disable combine mode before/after calling the renderer
   Renderer.combine=true ; Renderer(Render);
   Renderer.combine=false; 
}
/******************************************************************************/
