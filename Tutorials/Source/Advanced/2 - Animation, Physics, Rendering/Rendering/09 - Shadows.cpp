/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Mesh box , // mesh box
     ball; // mesh ball
/******************************************************************************/
void InitPre()
{
   App.name("Rendering Shadows");
   App.flag=APP_MS_EXCLUSIVE;
   Paks.add("../data/engine.pak");
   D.ambientPower(0);
}
/******************************************************************************/
Bool Init()
{
   Cam.dist=3;

   MaterialPtr material=Materials.ptrRequire("../data/mtrl/brick/0.mtrl");

   box .parts.New().base.create( Box(3), VTX_TEX0|VTX_NRM|VTX_TAN).reverse(); // create mesh box, reverse it because it's meant to be viewed from inside
   ball.parts.New().base.create(Ball(1), VTX_TEX0|VTX_NRM|VTX_TAN)          ; // create mesh ball

   // set mesh materials, rendering versions and bounding boxes
   box .setMaterial(material).setRender().setBox();
   ball.setMaterial(material).setRender().setBox();

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
   CamHandle(1.5f, 10, CAMH_ZOOM|CAMH_ROT);
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
         ball.draw(MatrixIdentity);

         LightPoint(10, Vec(Cos(Time.time()), Sin(Time.time()), -1.5f)).add();
      }break;
      
      // since we want to render shadows we have to process additional rendering mode 'RM_SHADOW' (render shadows through shadow maps)
      // here we will render all meshes which cast shadows
      case RM_SHADOW:
      {
         ball.drawShadow(MatrixIdentity);
      }break;
   }
}
void Draw()
{
   Renderer(Render);

   D.text(0, 0.9f, S+"Fps "+Time.fps()); // show number of fps
}
/******************************************************************************/
