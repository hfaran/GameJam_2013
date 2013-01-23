/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Mesh box ,
     ball;
/******************************************************************************/
void InitPre()
{
   App.name("Viewport");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE;
   Paks.add("../data/engine.pak");

   D.full(true).ambientPower(0);
}
/******************************************************************************/
Bool Init()
{
   Cam.dist=3;

   MaterialPtr material=Materials.ptrRequire("../data/mtrl/brick/0.mtrl");

   box .parts.New().base.create( Box(4), VTX_TEX0|VTX_NRM|VTX_TAN).reverse(); // create mesh box, reverse it because it's meant to be viewed from inside
   ball.parts.New().base.create(Ball(1), VTX_TEX0|VTX_NRM|VTX_TAN)          ; // create mesh ball

   // set mesh materials, rendering versions and bounding boxes
   box .setMaterial(material).setRender().setBox();
   ball.setMaterial(material).setRender().setBox();

   return true;
}
void Shut()
{
}
/******************************************************************************/
Bool Update()
{
   if(Kb.bp(KB_ESC))return false;
   CamHandle(0.1f, 10, CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT)); // move camera on right mouse button
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

         LightPoint(25, Vec(0,3,0)).add();
      }break;
   }
}
void Draw()
{
   // render to main viewport
   Renderer(Render);

   // render to another viewport
   {
      // set new viewport
      Rect rect(-D.w(), -D.h(), 0, 0); // setup viewport rectangle to left bottom quarter
           rect.extend(-0.05f);        // extend with negative value, to make it smaller
      D.viewRect(rect);                // commit new viewport rectangle

      // render everything once again
      Renderer(Render);
   }
}
/******************************************************************************/
