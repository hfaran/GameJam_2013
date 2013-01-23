/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Mesh mesh;
/******************************************************************************/
void InitPre()
{
   App.name("Rendering");
   App.flag=APP_MS_EXCLUSIVE;
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   Cam.dist=2;

   mesh.parts.New().base.create(Ball(1), VTX_TEX0|VTX_NRM|VTX_TAN);                          // create Mesh with 1 MeshPart, and create this MeshPart's base from Ball with automatic texture coordinates, normals and tangents
   mesh.setMaterial(Materials.ptrRequire("../data/mtrl/brick/0.mtrl")).setRender().setBox(); // set mesh material, rendering version and bounding box

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
   CamHandle(1.5f, 10, CAMH_ZOOM|CAMH_ROT);
   return true;
}
/******************************************************************************/
void Render() // Rendering Function
{
   switch(Renderer()) // Rendering Function will be called multiple times for different Rendering Modes
   {
      // the most important is the "prepare" mode in which you should draw all the meshes and add lights
      case RM_PREPARE:
      {
         // check if the mesh is inside the viewing frustum and add it to the drawing list
         if(Frustum(mesh.box))mesh.draw(MatrixIdentity);

         // add directional light
         LightDir(Vec(0,0,1)).add();
      }break;
   }
}
void Draw()
{
   // instead of typical "D.clear(WHITE);" we'll now use advanced rendering:
   Renderer(Render); // render using 'Render' function

   D.text(0, 0.9f, S+"Fps: "+Time.fps()); // show number of fps
}
/******************************************************************************/
