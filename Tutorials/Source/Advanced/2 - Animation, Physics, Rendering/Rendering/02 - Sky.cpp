/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Mesh  mesh  ; // sample mesh
Image skybox; // sky box texture
/******************************************************************************/
void InitPre()
{
   App.name("Rendering Sky");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE;
   Paks.add("../data/engine.pak");

   D.mode(800,600);
}
/******************************************************************************/
Bool Init()
{
   Cam.dist=6;

   // create mesh
   mesh.parts.New().base.create(Ball(1), VTX_TEX0|VTX_NRM|VTX_TAN);
   mesh.setMaterial(Materials.ptrRequire("../data/mtrl/brick/0.mtrl")).setRender().setBox();

   // import skybox
   skybox.Import("../data/Env/Sky/skybox.jpg", IMAGE_B8G8R8A8, IMAGE_CUBE);   

   // set default sky
   Sky.atmospheric();

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
   CamHandle(2.5f, 18, CAMH_ZOOM|CAMH_ROT);

   if(Kb.bp(KB_1))Sky.clear      (      ); // disable sky
   if(Kb.bp(KB_2))Sky.atmospheric(      ); // set atmospheric sky
   if(Kb.bp(KB_3))Sky.skybox     (skybox); // set sky from skybox

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         REPD(x, 3)
         REPD(z, 3)mesh.draw(Matrix(1, Vec(x-1, 0, z-1)*3));

         LightDir(!Vec(1,-1,1)).add();
      }break;
   }
}
void Draw()
{
   Renderer(Render);
   D.text(0, 0.9f, "Press 1,2,3 for different skies");
}
/******************************************************************************/
