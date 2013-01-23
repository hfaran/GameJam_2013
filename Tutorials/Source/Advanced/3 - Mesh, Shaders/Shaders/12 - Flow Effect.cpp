/******************************************************************************

   This tutorial presents drawing mesh with:
      -default    solid mode
      -additional blend mode (as flow effect)

   The flow effect works by using a texture mask,
      over which is applied additional flow texture (with UV texture movement,
      performed in the shader).

   Since there is no default texture channel for "mask" in the materials,
      the "bump" channel is used as the mask - when using custom masks,
      simply import them into materials as "bump" textures, and then
      disable the "bump intensity" slider in the material properties,
      which will disable the bump effect, however "bump" channel
      will still be available as the custom "mask" texture.

/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/material_user_shader.enum.h"
/******************************************************************************/
Mesh box,
     ball;

MaterialPtr brick;
Material    arrow;
/******************************************************************************/
void CompileShaders()
{
   if(D.shaderModel()==SM_GL)
   {
      ShaderCompile("Shader/Flow.cpp", S+DataPath()+"Shader/GL/User/Custom Shader", SM_GL);
   }else
   if(D.shaderModel()>=SM_4)
   {
      ShaderCompile("Shader/Flow.cpp", S+DataPath()+"Shader/4/User/Custom Shader", SM_4);
   }else
   {
      ShaderCompile("Shader/Flow.cpp", S+DataPath()+"Shader/2/User/Custom Shader", SM_2);
      ShaderCompile("Shader/Flow.cpp", S+DataPath()+"Shader/3/User/Custom Shader", SM_3);
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
      }break;
   }
   return GetDefaultShader(mode, material, mesh_base_flag, lod_index, allow_tesselation);
}
/******************************************************************************/
void InitPre()
{
   App.name("Flow Effect");
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
   brick="mtrl/brick/0.mtrl";

   arrow.load("mtrl/arrow/0.mtrl");
   arrow.user_shader=MUS_CUSTOM;
   arrow.validate();

   box .parts.New().base.create( Box(5), VTX_TEX0|VTX_NRM|VTX_TAN).reverse();
   ball.parts.New().base.create(Ball(1), VTX_TEX0|VTX_NRM|VTX_TAN)          ;

   box .setMaterial( brick).setRender().setBox();
   ball.setMaterial(&arrow).setRender().setBox();

   // set flow texture
   GetShaderImage("FlowImage")->set(*Images("mtrl/arrow/flow.gfx"));

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
   SPSet("TexOffset", Vec2(0, Time.time()));

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

         LightPoint(20, Vec(0,3,-3)).add();
      }break;

      case RM_BLEND:
      {
         ball.drawBlend(MatrixIdentity); // draw ball additionally in blend phase
      }break;
   }
}
void Draw()
{
   Renderer(Render);
}
/******************************************************************************/
