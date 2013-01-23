/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Mesh box, // mesh box
     mmb; // mesh middle barrier
/******************************************************************************/
void InitPre()
{
   App.name("Volumetric Lights");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE; // mouse exclusive, display can be toggled with Alt+Enter
   Paks.add("../data/engine.pak");

   D.mode(1024,700).ambientPower(0).volLight(true); // enable volumetric lighting
}
/******************************************************************************/
Bool Init()
{
   Cam.dist=8;

   MaterialPtr material=Materials.ptrRequire("../data/mtrl/brick/0.mtrl");

   box.parts.New().base.create(Box(3)    , VTX_TEX0|VTX_NRM|VTX_TAN).reverse();
   mmb.parts.New().base.create(Box(6,1,6), VTX_TEX0|VTX_NRM|VTX_TAN);

   // make hole inside 'mmb' by subtracting from it a temporary box (3D geometry subtraction will be used)
   {
      MeshBase temp;
      temp.create(Box(1), VTX_TEX0|VTX_NRM|VTX_TAN); // create temporary box
      Csg(mmb.parts[0].base, temp, SEL_SUB); // subtract 'temp' from 'mmb'
   }

   box.setMaterial(material).setRender().setBox();
   mmb.setMaterial(material).setRender().setBox();

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
   CamHandle(2, 10, CAMH_ZOOM|CAMH_ROT);
   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         box.draw(MatrixIdentity);
         mmb.draw(MatrixIdentity);

         LightPoint(5, Vec(Cos(Time.time()), 1, Sin(Time.time())), Vec(1), 0.1f).add(); // render light with volumetric amount set to 0.1
      }break;

      case RM_SHADOW:
      {
         box.drawShadow(MatrixIdentity); // volumetric lighting depends on shadow map, so we need to render 'box' too
         mmb.drawShadow(MatrixIdentity);
      }break;
   }
}
void Draw()
{
   Renderer(Render);
}
/******************************************************************************/
