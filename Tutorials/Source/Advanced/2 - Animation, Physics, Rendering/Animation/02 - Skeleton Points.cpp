/******************************************************************************/
#include "stdafx.h"
/******************************************************************************

   Skeleton points are special points attached to skeletons

   They can be used to render items in correct places

   Skeleton points can be accessed after animating skeleton through CSkeleton::getPoint method,
   which returns reference to OrientP structure which is an orientation and position

/******************************************************************************/
CSkeleton cskel;
/******************************************************************************/
void InitPre()
{
   App.name("Skeleton Points");
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
   cskel.clear().animate(L"../data/anim/walk.anim", Time.time()).updateMatrix(MatrixIdentity).updateVelocities();

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

         // draw mesh
         Meshes("../data/obj/chr/skeleton/0.mesh")->draw(cskel);

         // draw item in right hand
         {
            C OrientP &hand_r=cskel.getPoint("HandR");
            Matrix     m;
            m.setPosDir(hand_r.pos, hand_r.perp, hand_r.dir)              // set position and directions according to skeleton point
             .scaleOrn(0.7f);                                             // scale down the matrix orientation a little, making the item smaller
            Meshes("../data/obj/item/weapon/blunt/club/0.mesh")->draw(m); // render item with matrix
         }
      }break;
   }
}
void Draw()
{
   Renderer(Render);

   // draw skeleton points
   D.clearZ();
   SetMatrix();
   cskel.getPoint("HandR").draw(RED);
   cskel.getPoint("HandL").draw(RED);
   cskel.getPoint("Head" ).draw(RED);
}
/******************************************************************************/
