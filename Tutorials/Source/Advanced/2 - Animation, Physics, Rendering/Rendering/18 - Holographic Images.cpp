/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Mesh  box,ball;
Image image;
Flt   density_factor=0.05f;
/******************************************************************************/
void InitPre()
{
   App.name("Holographic Images");
   App.flag=APP_FULL_TOGGLE|APP_MS_EXCLUSIVE;
   DataPath("../data");
   Paks.add("engine.pak");
   D.full(true).ambientPower(0.1f);
}
/******************************************************************************/
Bool Init()
{
   Cam.dist=3;

   // create standard meshes and materials
   MaterialPtr material=Materials.ptrRequire("mtrl/brick/0.mtrl");

   box .parts.New().base.create( Box(5), VTX_TEX0|VTX_NRM|VTX_TAN).reverse(); box .setMaterial(material).setRender().setBox();
   ball.parts.New().base.create(Ball(1), VTX_TEX0|VTX_NRM|VTX_TAN)          ; ball.setMaterial(material).setRender().setBox();

   // create holographic image
   Int size=64;
   image.create(size, size, size, IMAGE_B8G8R8A8, IMAGE_3D, 1); // size x size x size dimensions, B8G8R8A8 type, 3D mode, 1 mip map
   if(image.lock()) // start manual pixel editing
   {
      REPD(z, image.z())
      REPD(y, image.y())
      REPD(x, image.x())
      {
         Flt fx=Flt(x)/(image.x()-1), // x pixel fraction value 0..1
             fy=Flt(y)/(image.y()-1), // y pixel fraction value 0..1
             fz=Flt(z)/(image.z()-1); // z pixel fraction value 0..1

         Flt px=fx*2-1, // x pixel position value -1..1
             py=fy*2-1, // y pixel position value -1..1
             pz=fz*2-1; // z pixel position value -1..1

         Vec4 color=0;

         if(x<=1 || x>=image.x()-2
         || y<=1 || y>=image.y()-2
         || z<=1 || z>=image.z()-2) // if current pixel is on border
         {
            color.set(0, 1, 0, RandomF(0.5f, 1)); // green color with random alpha
         }
         else // if current pixel is inside
         {
            // blue sphere
            Flt length=Vec(px, py, pz).length();
            Flt sphere=(1-Sat(Abs(length-0.5f)*8));//*RandomF();

            // red fade
            Flt red_fade=0.5f*fx*fx*RandomF();

            // color value from blue sphere and red fade
            if(Flt sum=sphere+red_fade) // total alpha value
               color.set(1*red_fade/sum, 0.5f*sphere/sum, 1*sphere/sum, sum);
         }

         image.color3DF(x, y, z, color); // store the pixel color
      }
      image.unlock(); // end pixel editing
   }

   REP(3)image.sharpen(1, 1, true, true); // sharpen the image 3 times

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

   if(Kb.b(KB_Q))density_factor/=1+Time.d();
   if(Kb.b(KB_W))density_factor*=1+Time.d();

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

      case RM_BLEND: // holographic images need to be drawn in RM_BLEND mode
      {
         image.drawVolume(WHITE, Color(0,0,0,0), OBox(Box(1)), density_factor, 1, 4, 64);
      }break;
   }
}
void Draw()
{
   Renderer(Render);
   D.text(0, 0.9f, S+"Fps: "+Time.fps());
   D.text(0, 0.8f, S+"Voxel Density Factor: "+density_factor+" (Q/W to change)");
}
/******************************************************************************/
