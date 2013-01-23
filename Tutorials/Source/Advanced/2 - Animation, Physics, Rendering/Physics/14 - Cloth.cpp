/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Actor ground,
      box   ,
      ball  ;

ClothMesh cloth_mesh; // mesh from which the cloth will be created
Cloth     cloth     ; // Physical Cloth
/******************************************************************************/
void InitPre()
{
   App.name("Physical Clothes");
   App.flag=APP_MS_EXCLUSIVE;
   Paks.add("../data/engine.pak");
   D.mode(1024,768);
}
Bool Init()
{
   Cam.dist =4;
   Cam.pitch=-PI_4;

   Physics.create(CSS_NONE, true, "../Installation/PhysX");

   ground.create(Box (15,1,15, Vec( 0,-2,0)), 0);
   box   .create(Box (0.3f   , Vec(-1, 0,0)));
   ball  .create(Ball(0.3f   , Vec( 1, 0,0)));

   // create cloth mesh
   MeshBase mshb; // cloth mesh can be created from MeshBase, so create it first
   mshb.createPlane(32, 32, VTX_TEX0); // create mesh as 32x32 vertex plane with texture coordinates set
   cloth_mesh.create(mshb, Materials.ptrRequire("../data/mtrl/ground/0.mtrl")); // create the cloth mesh from the mesh base

   // create cloth
   Cloth::Param param ; // parameters for cloth creation
   Matrix       matrix; matrix.setRotateX(PI_2).move(Vec(-0.5f, 1, -0.5f)).scale(3); // set cloth matrix
   cloth.create(cloth_mesh, matrix, param); // create cloth from the cloth mesh and custom parameters

   // Warning: ClothMesh cannot be deleted while there are still Cloth's using the mesh

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
         cloth.drawPhysical(); // draw cloth

         LightDir(Cam.matrix.z).add();
      }break;
   }
}
void Draw()
{
 	Renderer(Render);

   if(Renderer.rebuildDepthNeededForDebugDrawing())Renderer.rebuildDepth();
   Physics.draw();
}
/******************************************************************************/
