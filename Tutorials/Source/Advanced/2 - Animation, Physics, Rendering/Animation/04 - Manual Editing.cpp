/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
CSkeleton cskel;
/******************************************************************************/
void InitPre()
{
   App.name("Animation Manual Editing");
   App.flag=APP_MS_EXCLUSIVE;
   Paks.add("../data/engine.pak");
   D.ambientPower(0.1f);
}
/******************************************************************************/
Bool Init()
{
   Cam.dist=2;
   Cam.yaw =PI;

   cskel.create(Skeletons("../data/obj/chr/skeleton/0.skel"), 1.7f);
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
   CamHandle(0.1f, 10, CAMH_ROT|CAMH_ZOOM);

   // set animations
   {   
      cskel.clear().animate(L"../data/anim/walk.anim", Time.time()); // set default walking animation

      Orient &head=cskel.getBone("head").orn;            // get head bone orientation
      head*=Matrix3().setRotateZ(Sin(Time.time()*1.5f)); // rotate head orientation according to time

      cskel.updateMatrix(MatrixIdentity) // update all matrixes
           .updateVelocities();          // update all bone velocities
   }

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         LightDir(Vec(0,0,-1), 1-D.ambientColor()).add();

         Meshes("../data/obj/chr/skeleton/0.mesh")->draw(cskel);
      }break;
   }
}
void Draw()
{
   Renderer(Render);
}
/******************************************************************************/
