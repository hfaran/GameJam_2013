/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Actor ground,
      ball  ;

SoftBodyMesh soft_body_mesh; // mesh from which the soft_body will be created
SoftBody     soft_body     ; // Soft Body
/******************************************************************************/
void InitPre()
{
   App.name("Soft Bodies");
   App.flag=APP_MS_EXCLUSIVE|APP_MEM_LEAKS;
   DataPath("../data");
   Paks.add("engine.pak");
   D.mode(1024,768);
}
Bool Init()
{
   Cam.dist =4;
   Cam.pitch=-PI_4;

   Physics.create(CSS_NONE, true, "../Installation/PhysX");

   ground.create(Box (15, 1, 15, Vec( 0,-2,0)), 0);
   ball  .create(Ball(1.0f     , Vec( 0, 0,0)));

   // create soft body mesh
   soft_body_mesh.create(*Meshes("Obj/vehicle/tank/0.mesh"), 0.0f); // create the soft body mesh from mesh

   // create soft body
   Matrix matrix; matrix.setScale(2).move(Vec(0, 2, 0.1f)); // set soft body matrix
   soft_body.create(soft_body_mesh, matrix); // create cloth from the soft body mesh

   // Warning: SoftBodyMesh cannot be deleted while there are still SoftBody's using the mesh

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
   Physics.startSimulation().stopSimulation();
   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         soft_body.draw(); // draw soft body

         LightDir(Cam.matrix.z).add();
      }break;
   }
}
void Draw()
{
   Renderer(Render);
   if(Renderer.rebuildDepthNeededForDebugDrawing())Renderer.rebuildDepth();
   Physics.draw();

   if(Ms.b(0))soft_body.drawTetra(ColorAlpha(0.4f), ColorAlpha(0.5f));
   
   D.text(0, 0.9f, "Press LMB to show Control Points");
}
/******************************************************************************/
