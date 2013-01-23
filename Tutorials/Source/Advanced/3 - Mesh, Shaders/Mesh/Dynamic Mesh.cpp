/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Mesh mesh;
/******************************************************************************/
void InitPre()
{
   App.name("Dynamic Mesh");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.mode(800,600);
   Cam.dist=3;
}
/******************************************************************************/
Bool Init()
{
   MeshPart &part=mesh.parts.New(); // create a new part
   MeshBase &base=part.base; // access software version
   base.create(3, 0, 1, 0, VTX_TEX0); // create part with 3 vertexes, 1 triangle, and texture coordinates (vertex positions and face vertex indexes are always created, and don't need to be specified manually)
   base.vtx.pos (0).set(-1, 1, 0); // set #0 vertex position
   base.vtx.pos (1).set( 1, 1, 0); // set #1 vertex position
   base.vtx.pos (2).set( 0,-1, 0); // set #2 vertex position
   base.vtx.tex0(0).set(-1, 1   ); // set #0 vertex texture coordinates
   base.vtx.tex0(1).set( 1, 1   ); // set #1 vertex texture coordinates
   base.vtx.tex0(2).set( 0,-1   ); // set #2 vertex texture coordinates
   base.tri.ind (0).set( 0, 1, 2); // set triangle vertex indexes (#0 #1 #2 vertexes)
   base.setNormals(); // set automatic vertex normals

   mesh.setMaterial  (Materials.ptrRequire("mtrl/brick/0.mtrl")); // set material for all parts
   mesh.setAutoTanBin(); // calculate tangents and binormals if needed
   mesh.setRender    (); // set rendering versions from software versions
   mesh.setBox       (); // recalculate bounding box from vertexes
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
   CamHandle(0.01f, 10, CAMH_ZOOM|(Ms.b(1) ? CAMH_MOVE : CAMH_ROT));
   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         mesh.draw(MatrixIdentity);
         LightDir(Cam.matrix.z).add();
      }break;
   }
}
/******************************************************************************/
void Draw()
{
   Renderer(Render);
}
/******************************************************************************/
