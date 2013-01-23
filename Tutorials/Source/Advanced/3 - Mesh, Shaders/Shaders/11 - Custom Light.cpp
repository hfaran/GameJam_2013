/******************************************************************************

   In this tutorial we're going to present drawing meshes with custom lighting,
      independent of all lights in the scene.

   In order to use custom lighting we must not use RM_SOLID mode,
      because in Deferred Renderer it always gets affected by default lighting.

   Instead, RM_BLEND is used,
      with material technique set to MTECH_TEST_BLEND_LIGHT which enables depth-writing.

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
   if(D.shaderModel()==SM_GL)
   {
      ShaderCompile("Shader/Custom Light.cpp", S+DataPath()+"Shader/GL/User/Custom Shader", SM_GL);
   }else
   if(D.shaderModel()>=SM_4)
   {
      ShaderCompile("Shader/Custom Light.cpp", S+DataPath()+"Shader/4/User/Custom Shader", SM_4);
   }else
   {
      ShaderCompile("Shader/Custom Light.cpp", S+DataPath()+"Shader/2/User/Custom Shader", SM_2);
      ShaderCompile("Shader/Custom Light.cpp", S+DataPath()+"Shader/3/User/Custom Shader", SM_3);
   }
}
/******************************************************************************/
ShaderTech* GetShader(RENDER_MODE mode, Material *material[4], UInt mesh_base_flag, Int lod_index, Bool allow_tesselation)
{
   if(material[0])switch(material[0]->user_shader)
   {
      case MUS_CUSTOM:
      {
         if(mode==RM_BLEND)return Shaders("User/Custom Shader")->firstTech();
         return NULL;
      }break;
   }
   return GetDefaultShader(mode, material, mesh_base_flag, lod_index, allow_tesselation);
}
/******************************************************************************/
void InitPre()
{
   App.name("Custom Light");
   App.flag=APP_FULL_TOGGLE|APP_MS_EXCLUSIVE;
   DataPath("../data");
   Paks.add("engine.pak");
   D.full(true).ambientPower(0.1f);
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
   brick_with_custom_shader.technique  =MTECH_TEST_BLEND_LIGHT; // set test blend light technique (this will internally enable depth-writing)
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

   // set shader's custom parameter value
   Vec dir; SinCos(dir.y, dir.x, Time.time()); dir.z=-1; dir.normalize(); // set view-space light direction
   SPSet("LightDirection", dir);

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

         LightPoint(20, Vec(0,3,3)).add();
      }break;
   }
}
void Draw()
{
   Renderer(Render);
}
/******************************************************************************/
