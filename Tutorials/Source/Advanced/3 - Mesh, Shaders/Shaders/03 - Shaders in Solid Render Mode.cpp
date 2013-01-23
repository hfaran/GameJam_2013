/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/material_user_shader.enum.h"
/******************************************************************************/
Mesh box,
     ball;

MaterialPtr brick;
Material    brick_with_custom_shader;
/******************************************************************************/
void CompileShaders()
{
   // compile solid mode shaders
   if(D.shaderModel()==SM_GL)
   {
      ShaderCompile("Shader/Solid Mode.cpp", S+DataPath()+"Shader/GL/User/Custom Shader", SM_GL);
   }else
   if(D.shaderModel()>=SM_4)
   {
      ShaderCompile("Shader/Solid Mode.cpp", S+DataPath()+"Shader/4/User/Custom Shader", SM_4);
   }else
   {
      ShaderCompile("Shader/Solid Mode.cpp", S+DataPath()+"Shader/2/User/Custom Shader", SM_2);
      ShaderCompile("Shader/Solid Mode.cpp", S+DataPath()+"Shader/3/User/Custom Shader", SM_3);
   }
}
/******************************************************************************/
ShaderTech* GetShader(RENDER_MODE mode, Material *material[4], UInt mesh_base_flag, Int lod_index, Bool allow_tesselation)
{
   if(material[0])switch(material[0]->user_shader)
   {
      case MUS_CUSTOM:
      {
         if(mode==RM_SOLID)return Shaders("User/Custom Shader")->firstTech(); // override the shader for the RM_SOLID mode
      }break;
   }
   return GetDefaultShader(mode, material, mesh_base_flag, lod_index, allow_tesselation);
}
/******************************************************************************/
void InitPre()
{
   App.name("Solid Mode Shaders");
   App.flag=APP_FULL_TOGGLE|APP_MS_EXCLUSIVE;
   DataPath("../data");
   Paks.add("engine.pak");
   D.full(true).ambientPower(0.1);
   D.setGetShaderFunc(GetShader);
}
Bool Init()
{
   Cam.dist=3;

   // compile shader
   CompileShaders();

   // set materials and meshes
   brick.require("mtrl/brick/0.mtrl");
   brick_with_custom_shader=*brick;
   brick_with_custom_shader.user_shader=MUS_CUSTOM;
   brick_with_custom_shader.validate();

   box .parts.New().base.create( Box(5), VTX_TEX0|VTX_NRM|VTX_TAN).reverse();
   ball.parts.New().base.create(Ball(1), VTX_TEX0|VTX_NRM|VTX_TAN)          ;

   box .setMaterial( brick                   ).setRender().setBox();
   ball.setMaterial(&brick_with_custom_shader).setRender().setBox();

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
}
/******************************************************************************/