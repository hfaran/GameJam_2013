/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Mesh   mesh;
Matrix matrix;
/******************************************************************************/
void InitPre()
{
   App.name("Separate parts rendering");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.mode(800,600).ambientPower(0.2f);
   Cam.dist=3;
}
/******************************************************************************/
Bool Init()
{
   mesh.create(3); // create mesh with room for 3 parts
  
   // setup #0
   {
      MeshPart &part=mesh.parts[0];
      part.base.create(Ball(0.7f), VTX_TEX0|VTX_NRM|VTX_TAN);      // create MeshBase
      part.setMaterial(Materials.ptrRequire("mtrl/brick/0.mtrl")); // set material
      Set(part.name, "ball");                                      // set name
   }

   // setup #1
   {
      MeshPart &part=mesh.parts[1];
      part.base.create(Tube(0.3f, 3), VTX_TEX0|VTX_NRM|VTX_TAN);   // create MeshBase
      part.setMaterial(Materials.ptrRequire("mtrl/glass/0.mtrl")); // set material
      Set(part.name, "tube");                                      // set name
   }

   // setup #2
   {
      MeshPart &part=mesh.parts[2];
      part.base.create(Box(3, 0.3f, 0.3f), VTX_TEX0|VTX_NRM|VTX_TAN); // create MeshBase
      part.setMaterial(Materials.ptrRequire("mtrl/grass/0.mtrl"));    // set material
      Set(part.name, "box");                                          // set name
   }

   mesh.setRender().setBox(); // setup mesh rendering version and bounding box
   matrix.setPos(Vec(0,0,1)); // setup object matrix with position (0,0,1)

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
void DrawSeparately()
{
   {
      CChar8 *name[]=
      {
         "tube",
         "box",
      };
      mesh.showAll().hide(name, Elms(name)).draw(matrix); // render all parts EXCEPT those listed in 'names'
   }
   {
      Matrix local_transformation;
             local_transformation.setRotateX(Time.time()/2);
      mesh.hideAll().show("tube").draw(local_transformation*matrix); // render ONLY those parts that are named "tube"
   }
   {
      Matrix local_transformation;
             local_transformation.setRotateZ(Time.time()/2);
      mesh.hideAll().show("box").draw(local_transformation*matrix); // render ONLY those parts that are named "box"
   }
   mesh.showAll(); // remember to un-hide all elements after separate parts rendering
}
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         if(!Kb.b(KB_SPACE)) // if space not pressed
         {
            mesh.draw(matrix); // render all parts with one object matrix
         }else
         {
            // when space pressed render each part separately with different matrix
            DrawSeparately();
         }

         LightDir(Cam.matrix.z).add();
      }break;
   }
}
void Draw()
{
   Renderer(Render);
   D.text(0, 0.8f, "Hold space to render each part with different matrix");
}
/******************************************************************************/
