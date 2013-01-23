/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Mesh  box ,
      ball;
Image rtt ; // Render Target Tetxture (of IMAGE_RT mode)
/******************************************************************************/
Bool Lost()
{
   rtt.del();   // IMAGE_RT Image's must be manually deleted before the D3D Device is reset
   return true; // return success
}
/******************************************************************************/
void InitPre()
{
   App.name("Render To Texture");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE;
   Paks.add("../data/engine.pak");

   D.lost=Lost; // specify custom function called when D3D Device is lost
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
void RenderToTexture()
{
   Int tex_size=256;

   // create the Render Target Texture if it hasn't been created yet
   if(!rtt.is())rtt.create(tex_size, tex_size, 1, IMAGE_B8G8R8A8, IMAGE_RT, 1);

   // render to texture
   Bool super_sample=true;                                               // optionally use super sampling to enable anti-aliasing at the cost of greater viewport size
   Int  render_size =Min(D.x(), D.y(), tex_size*(super_sample ? 2 : 1)); // calculate rendering viewport size
   D.viewRect(&RectI(0, 0, render_size, render_size));                   // set new viewport
   Renderer.dest_rt=&rtt;                                                // specify custom render target
   Renderer(Render);                                                     // perform rendering
   Renderer.dest_rt=NULL;                                                // disable custom render target
   D.viewRect(NULL);                                                     // restore full viewport
}
void Draw()
{
   // first handle rendering to texture before anyother drawing/rendering
   RenderToTexture();

   // render normally
   Renderer(Render);
   
   // now we can use previously rendered texture
   ALPHA_MODE alpha=D.alpha(ALPHA_NONE);                 // the rendering result's alpha channel is undefined so we can't use it, for that we need to disable alpha blending
   rtt.drawRotate(Vec2(0,0), Vec2(1,1), Time.appTime()); // draw texture
   D.alpha(alpha);                                       // restore previously modified alpha mode
}
/******************************************************************************/
