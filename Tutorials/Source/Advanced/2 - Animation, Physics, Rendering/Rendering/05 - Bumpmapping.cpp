/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Mesh box ,
     ball;
/******************************************************************************

   In this tutorial we'll show different Bump Mapping modes changed dynamically
   Changing Bump Mapping modes in runtime requires reseting shaders of manually created Meshes
   To achieve this we need to create a function which will reset shaders of those meshes:

/******************************************************************************/
void SetShader()
{
   box .setShader();
   ball.setShader();
}
/******************************************************************************/
void InitPre()
{
   App.name("Rendering Bumpmapping");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE;
   Paks.add("../data/engine.pak");

   D.set_shader=SetShader                          ; // set 'SetShader' function to be used when needed
   D.full(true).ambientPower(0).bumpMode(BUMP_FLAT); // start with flat bump mapping
}
/******************************************************************************/
Bool Init()
{
   Cam.dist=3;

   MaterialPtr material=Materials.ptrRequire("../data/mtrl/brick/0.mtrl");

   box .parts.New().base.create( Box(4), VTX_TEX0|VTX_NRM|VTX_TAN).reverse(); // create mesh box, reverse it because it's meant to be viewed from inside
   ball.parts.New().base.create(Ball(1), VTX_TEX0|VTX_NRM|VTX_TAN)          ; // create mesh ball

   // set mesh materials, rendering versions and bounding boxes
   box .setMaterial(material).setRender().setBox();
   ball.setMaterial(material).setRender().setBox();

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
   CamHandle(0.1f, 10, CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT)); // move camera on right mouse button

   // change bump mapping when keys pressed
   if(Kb.bp(KB_1))D.bumpMode(BUMP_FLAT    );
   if(Kb.bp(KB_2))D.bumpMode(BUMP_NORMAL  );
   if(Kb.bp(KB_3))D.bumpMode(BUMP_PARALLAX);
   if(Kb.bp(KB_4))D.bumpMode(BUMP_RELIEF  );

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         box .draw(MatrixIdentity);
         ball.draw(MatrixIdentity);

         LightPoint(25, Vec(0,3,0)).add();
      }break;
   }
}
void Draw()
{
   Renderer(Render);
   D.text(0, 0.9f, S+"Fps "+Time.fps());
   D.text(0, 0.8f, "Press 1,2,3,4 keys for different Bumpmapping modes");
}
/******************************************************************************/
