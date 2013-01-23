/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Controller ctrl ; // character controller
Mesh       mesh ; //          mesh (used for rendering)
PhysBody   phys ; // physical body (used for collisions)
Actor      actor; // actors created from physical body
/******************************************************************************/
void InitPre()
{
   App.name("Physical meshes");
   App.flag=APP_MS_EXCLUSIVE;
   DataPath("../data");
   Paks.add("engine.pak");
   D.full(true);
}
Bool Init()
{
   Cam.dist=6;
   Physics.create(CSS_NONE, true, "../Installation/PhysX");
   
   ctrl.create(Capsule(0.4f, 1.7f, Vec(0,1,0))); // create character controller

   mesh .load("obj/terrain/0.mesh");   // load terrain mesh
   phys .parts.New().createMesh(mesh); // create physical body from 'mesh'
   actor.create(phys);                 // create actor from physical body
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

   // update controller (move on WSAD keys, crouch on Shift key, jump on Space key)
   Flt s=3;
   Vec vel(0,0,0);
   if(Kb.b(KB_W))vel+=!PointOnPlane(Cam.matrix.z, Vec(0,1,0))*s;
   if(Kb.b(KB_S))vel-=!PointOnPlane(Cam.matrix.z, Vec(0,1,0))*s;
   if(Kb.b(KB_A))vel-=!PointOnPlane(Cam.matrix.x, Vec(0,1,0))*s;
   if(Kb.b(KB_D))vel+=!PointOnPlane(Cam.matrix.x, Vec(0,1,0))*s;
   ctrl.update(vel, Kb.shift(), Kb.bp(KB_SPACE) ? 3.5f : 0);

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         mesh.draw(MatrixIdentity); // draw mesh

         LightDir(Cam.matrix.z).add();
      }break;
   }
}
void Draw()
{
   Renderer(Render);

   if(Renderer.rebuildDepthNeededForDebugDrawing())Renderer.rebuildDepth();
   Physics.draw(); // draw physics
}
/******************************************************************************/
