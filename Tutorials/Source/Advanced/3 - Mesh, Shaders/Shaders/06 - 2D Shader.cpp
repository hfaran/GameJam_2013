/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/material_user_shader.enum.h"
/******************************************************************************/
Mesh box,
     ball;

MaterialPtr brick;
/******************************************************************************/
void CompileShaders()
{
   if(D.shaderModel()==SM_GL)
   {
      ShaderCompile("Shader/2D Shader.cpp", S+DataPath()+"Shader/GL/User/Custom Shader", SM_GL);
   }else
   if(D.shaderModel()>=SM_4)
   {
      ShaderCompile("Shader/2D Shader.cpp", S+DataPath()+"Shader/4/User/Custom Shader", SM_4);
   }else
   {
      ShaderCompile("Shader/2D Shader.cpp", S+DataPath()+"Shader/2/User/Custom Shader", SM_2);
      ShaderCompile("Shader/2D Shader.cpp", S+DataPath()+"Shader/3/User/Custom Shader", SM_3);
   }
}
/******************************************************************************/
void InitPre()
{
   App.name("2D Shader");
   App.flag=APP_FULL_TOGGLE|APP_MS_EXCLUSIVE;
   DataPath("../data");
   Paks.add("engine.pak");
   D.full(true).ambientPower(0.1f);
}
Bool Init()
{
   Cam.dist=2;

   // compile shader
   CompileShaders();

   // set meshes
   brick.require("mtrl/brick/0.mtrl");
   box  .parts.New().base.create( Box(5), VTX_TEX0|VTX_NRM|VTX_TAN).reverse();
   ball .parts.New().base.create(Ball(1), VTX_TEX0|VTX_NRM|VTX_TAN)          ;

   box .setMaterial(brick).setRender().setBox();
   ball.setMaterial(brick).setRender().setBox();

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
   CamHandle(0.1f, 1000, CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT));

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         ball.draw(MatrixIdentity);
         box .draw(MatrixIdentity);

         LightPoint(20, Vec(0,3,-3)).add();
      }break;
   }
}
void Draw()
{
   Renderer(Render);

   // perform a 2D shader effect on left half of the screen
   Shaders("User/Custom Shader")->getTech("CustomTechnique")->draw(NULL, &Rect(-D.w(), -D.h(), 0, D.h()));
}
/******************************************************************************/
