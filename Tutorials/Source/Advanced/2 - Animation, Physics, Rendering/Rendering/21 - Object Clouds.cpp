/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Image image[3]; // cloud images

struct Cloud
{
   Image *image;
   OBox   obox ;

   Cloud()
   {
      image=NULL;
   }

   void draw(Color color, Color color_add)
   {
      if(image)image->drawVolume(color, color_add, obox, 0.5, 0.5);
   }
};
Memb<Cloud> clouds; // container of clouds
/******************************************************************************/
void InitPre()
{
   App.name("Object Clouds");
   App.flag=APP_FULL_TOGGLE|APP_MS_EXCLUSIVE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true).ambientPower(0.3f).hpRt(true).viewRange(200);

   Cam.dist =1;
   Cam.yaw  =2.8;
   Cam.pitch=0.3;
   Cam.at.set(0,0,0);
}
Bool Init()
{
   Sky.atmospheric();
   Sun.image="Env/Sky/sun.gfx";
   Sun.rays_color=0.15;

   // create cloud images
   Image detail; CreateVolumetricDetail(detail, 32, 16, 32); detail.blur(1, false);
   REPA(image)
   {
      CreateVolumetricCloud(image[i], &detail, 64, 32, 64, 3, true, 80);
      image[i].mulAdd(Vec4(1,1,1,4), Vec4(0,0,0,0)); // scale alpha value
      image[i].lumFromAlphaAndLight(DIRF_RIGHT|DIRF_DOWN|DIRF_FORWARD, 0.5f);
   }

   Flt average_size=4;

   // volumetric images may not intersect with each other, so set positions based on a regular grid modified by slight rotations and offsets
   for(Int x=-2; x<=2; x++)
   for(Int z=-2; z<=2; z++)
   {
      Cloud &cloud=clouds.New();

      // set random image
      cloud.image=&image[Random(Elms(image))];

      // set cloud matrix
      cloud.obox.box.set(average_size*RandomF(0.8f, 1.2f));
      cloud.obox.box.min.y/=2;
      cloud.obox.box.max.y/=2;
      cloud.obox.matrix.setRotateXY(RandomF(-0.1f, 0.1f), RandomF(-0.4f, 0.4f))
                       .move       ((Vec(x,0,z)+Random.vec(-0.25f, 0.25f))*average_size*2)
                       .move       (0,10,0);
   }

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

   return true;
}
/******************************************************************************/
Int Compare(C Cloud &c0, C Cloud &c1)
{
   return CompareTransparencyOrder(c0.obox.matrix.pos, c1.obox.matrix.pos);
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE: Meshes("obj/terrain/0.mesh")->draw(MatrixIdentity); break; // draw terrain

      case RM_CLOUD: // clouds in order to affect the sun rays must be drawn in RM_CLOUD mode
      {
         Flt   color_dark  =0.38,
               color_bright=0.78;
         Color color       =           ColorBrightness(color_bright-color_dark)    ,
               color_add   =ColorAlpha(ColorBrightness(             color_dark), 0);

         // clouds are transparent so they need to be sorted before rendering
         clouds.sort(Compare);

         // draw the clouds
         REPAO(clouds).draw(color, color_add);
      }break;
   }
}
void Draw()
{
   Renderer(Render);
   D.text(0, 0.9f, S+"Fps "+Time.fps());
}
/******************************************************************************/
