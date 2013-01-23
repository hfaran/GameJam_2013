/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Mesh box,ball;
Int  fog_type=0; // 0..3
/******************************************************************************/
void InitPre()
{
   App.name("Local Fog");
   App.flag=APP_FULL_TOGGLE|APP_MS_EXCLUSIVE;
   DataPath("../data");
   Paks.add("engine.pak");
   D.full(true).ambientPower(0.1f);
}
/******************************************************************************/
Bool Init()
{
   Cam.at.set(0,-1,0);
   Cam.pitch=-0.5f;
   Cam.dist =7;

   // create standard meshes and materials
   MaterialPtr material=Materials.ptrRequire("mtrl/brick/0.mtrl");

   box .parts.New().base.create( Box(5), VTX_TEX0|VTX_NRM|VTX_TAN).reverse(); box .setMaterial(material).setRender().setBox();
   ball.parts.New().base.create(Ball(1), VTX_TEX0|VTX_NRM|VTX_TAN)          ; ball.setMaterial(material).setRender().setBox();

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
   CamHandle(0.1f, 1000, CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT));

   if(Kb.bp(KB_0))fog_type=0;
   if(Kb.bp(KB_1))fog_type=1;
   if(Kb.bp(KB_2))fog_type=2;
   if(Kb.bp(KB_3))fog_type=3;

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         box .draw(MatrixIdentity);
         ball.draw(Matrix(Vec(0,-3,0)));

         LightPoint(20, Vec(0,3,3)).add();
      }break;

      case RM_BLEND: // local fog needs to be drawn in RM_BLEND mode
      {
         switch(fog_type)
         {
            case 1:       FogDraw(OBox(Box(10, 4, 10, Vec(0,-3,0))), 0.6f, Vec(0.5f,0.5f,0.5f)); break;
            case 2: HeightFogDraw(OBox(Box(10, 4, 10, Vec(0,-3,0))), 0.6f, Vec(0.5f,0.5f,0.5f)); break;
            case 3:       FogDraw(Ball(5            , Vec(0,-5,0) ), 0.6f, Vec(0.5f,0.5f,0.5f)); break;
         }
      }break;
   }
}
void Draw()
{
   Renderer(Render);
   D.text(0, 0.9f, S+"Fps: "+Time.fps());
   D.text(0, 0.8f, S+"Press 0,1,2,3 keys for different local fogs");
}
/******************************************************************************/
