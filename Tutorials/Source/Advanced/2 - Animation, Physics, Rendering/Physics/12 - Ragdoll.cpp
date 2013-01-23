/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Actor     ground , // ground actor
          box[3] ; // stairs
CSkeleton cskel  ; // controlled skeleton
Ragdoll   ragdoll; // ragdoll
/******************************************************************************/
void SetPose()
{
   cskel.clear().updateMatrix(Matrix().setRotateX(0.45).move(Vec(0,3,0))).updateVelocities(); // set skeleton animation to default pose and custom matrix

   ragdoll.fromSkel(cskel); // setup ragdoll from skeleton animation
}
/******************************************************************************/
void InitPre()
{
   App.name("Ragdoll");
   App.flag=APP_MS_EXCLUSIVE|APP_MEM_LEAKS;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true).ambientPower(0);
   Cam.dist =5;
   Cam.pitch=-0.3f;
   Cam.yaw  =PI;
}
Bool Init()
{
   Physics.create(CSS_NONE, true, "../Installation/PhysX");

   ground.create(Box_U(15, 1, 15, Vec(0,-1,0)), 0);
   REPA(box)box[i].create(Box(1, Vec(0, i*0.5f-0.5f, i*-0.4f-0.3f)), 0);

   cskel  .create(Skeletons("obj/chr/skeleton/0.skel"), 1.8f); // create skeleton
   ragdoll.create(cskel);                                      // create ragdoll from skeleton

   SetPose();

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
   CamHandle(0.1f, 100, CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT)); // move camera on right mouse button

   Physics.startSimulation().stopSimulation();

   ragdoll.toSkel(cskel); // set skeleton from ragdoll

   if(Kb.bp(KB_SPACE))SetPose();

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         LightDir(Cam.matrix.z).add();

         Meshes("obj/chr/skeleton/0.mesh")->draw(cskel);
      }break;
   }
}
void Draw()
{
   Renderer(Render);

   if(Renderer.rebuildDepthNeededForDebugDrawing())Renderer.rebuildDepth();
	Physics.draw();

   D.text(0, 0.9f, "Press Space to reset simulation");
}
/******************************************************************************/
