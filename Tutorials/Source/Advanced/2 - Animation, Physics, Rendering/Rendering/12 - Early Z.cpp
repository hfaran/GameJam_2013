/******************************************************************************/
#include "stdafx.h"
/******************************************************************************

   EarlyZ is a technique of rejecting pixels which aren't visible

   It works by rendering the object in additional pass only to the depth buffer

      Advantage of EarlyZ is that it will make rendering faster because occluded pixels won't be processed many times
   Disadvantage is that it requires additional pass of rendering, but fortunately it consists only of vertex transformations

/******************************************************************************/
Mesh box ,
     ball;
/******************************************************************************/
void InitPre()
{
   App.name("Early Z");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE;
   Paks.add("../data/engine.pak");

   D.full(true).ambientPower(0).bumpMode(BUMP_RELIEF); // setup relief bump mapping to enable pixel-heavy processing
}
/******************************************************************************/
Bool Init()
{
   Cam.dist=3;

   MaterialPtr material=Materials.ptrRequire("../data/mtrl/brick/0.mtrl");

   box .parts.New().base.create( Box(5), VTX_TEX0|VTX_NRM|VTX_TAN).reverse();
   ball.parts.New().base.create(Ball(1), VTX_TEX0|VTX_NRM|VTX_TAN);

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
   CamHandle(0.1f, 10, CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT));
   return true;
}
/******************************************************************************/
void Render()
{
   Bool use_early_z=Kb.b(KB_SPACE);

   switch(Renderer())
   {
      case RM_PREPARE:
      {
                box .draw(MatrixIdentity         , Vec(0), Vec(0), use_early_z);
         FREP(5)ball.draw(Matrix(2, Vec(0,0,5-i)), Vec(0), Vec(0), use_early_z); // draw in back-to-front order to present EarlyZ speedup

         LightPoint(25, Vec(0,3,-1)).add();
      }break;
   }
}
void Draw()
{
   Renderer(Render);
   D.text(0, 0.9f, S+"Fps: "+Time.fps());
   D.text(0, 0.8f,   "Hold Space to enable EarlyZ");
   D.text(0, 0.7f,   "(EarlyZ may be disabled if tesselation is available)");
}
/******************************************************************************/
