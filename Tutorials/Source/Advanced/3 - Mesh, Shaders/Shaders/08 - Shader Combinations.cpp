/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/material_user_shader.enum.h"
/******************************************************************************

   This tutorial will present how to create a shader with multiple techniques,
      where each of the possible technique combination is specified here in the C++ codes,
      instead of in the shader file.

   First you need to create a shader file which accepts uniform compilation parameters in vertex/pixel shader.

   Then you need to manually define the shader techniques from the C++ codes instead of defining them in the shader.

   When shader is compiled you can load it and select desired shader technique inside the 'GetShader' function.

/******************************************************************************/
Mesh      ball ,
         *body ;
CSkeleton cskel;
/******************************************************************************/
Str TechniqueCustomShader(Int skin/*, Int param1, Int param2*/) // get the desired technique name of a "custom shader" depending on given parameters
{
   Str params=S+    skin/*+','+param1+','+param2*/, // setup uniform combination parameters (this must match to the specified 'PARAMS' macro inside the shader file), this will be equal to something like this :  "1,0,2" - list of parameters
       name  =S+"T"+skin/*+'_'+param1+'_'+param2*/; // technique name cannot start from a number, so add 'T' at the start (as in Technique)                         , this will be equal to something like this : "T1_0_2" - technique name
   return S+"TECHNIQUE("+name+", VS("+params+"), PS("+params+"));"; // return the Technique specification                                                           , this will be equal to something like this : "TECHNIQUE(T1_0_2, VS(1,0,2), PS(1,0,2));"
}
Str TechniqueCustomShaderName(Int skin/*, Int param1, Int param2*/) // get only the name of the desired technique, this is similar to the 'TechniqueCustomShader'
{
   return S+"T"+skin/*+'_'+param1+'_'+param2*/;
}
/******************************************************************************/
void CompileShader(Str src, Str dest, SHADER_MODEL model, Str techniques)
{
   Memc<ShaderMacro> macros;
   macros.New().set("CUSTOM_TECHNIQUE", techniques); // replace the 'CUSTOM_TECHNIQUE' macro with manually specified list of desired 'techniques'
   ShaderCompile(src, dest, model, &macros); // call the engine shader compilation with specified parameters
}
struct ShaderModel
{
   SHADER_MODEL model;
   Str          path ;

   void set(SHADER_MODEL model, Str path) {T.model=model; T.path=path;}
};
void CompileShaders()
{
   Memc<ShaderModel> models;
   if(D.shaderModel()==SM_GL)models.New().set(SM_GL, "GL");else
   if(D.shaderModel()>=SM_4 )models.New().set(SM_4 , "4" );else
   {
                             models.New().set(SM_2 , "2" );
                             models.New().set(SM_3 , "3" );
   }

   REPA(models) // compile shaders for specified models
   {
      Str techniques; // this will hold a list of all desired technique combinations

      REPD(skin, 2) // 2 possible combinations of skinning (disabled and enabled)
         techniques+=TechniqueCustomShader(skin); // add desired technique to the technique list

      CompileShader("Shader/Shader with Combinations.cpp", S+DataPath()+"Shader/"+models[i].path+"/User/Custom Shader", models[i].model, techniques); // compile the shader for specified model and desired list of techniques
   }
}
/******************************************************************************/
ShaderTech* GetShader(RENDER_MODE mode, Material *material[4], UInt mesh_base_flag, Int lod_index, Bool allow_tesselation)
{
   if(mode==RM_BLEND)
   {
      return Shaders("User/Custom Shader")->getTech(TechniqueCustomShaderName(FlagTest(mesh_base_flag, VTX_SKIN))); // load the custom shader and specify technique name depending on parameters, in this example - if the mesh contains skinning information then use the "skinned" version of the shader
   }
   return NULL;
}
/******************************************************************************/
void InitPre()
{
   App.name("Shader Combinations");
   App.flag=APP_FULL_TOGGLE|APP_MS_EXCLUSIVE;
   DataPath("../data");
   Paks.add("engine.pak");
   D.full(true).ambientPower(0.1f);
   D.setGetShaderFunc(GetShader);
}
Bool Init()
{
   Cam.dist=2;
   Sky.atmospheric();

   CompileShaders();

   ball.parts.New().base.create(Ball(0.3f), VTX_TEX0|VTX_NRM|VTX_TAN);
   ball.setMaterial(Materials.ptrRequire("mtrl/brick/0.mtrl")).setRender().setBox();

   body =          Meshes("obj/chr/warrior/body.mesh") ;
   cskel.create(Skeletons("obj/chr/warrior/body.skel"));

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

   cskel.clear()
        .animate(L"anim/walk.anim", Time.time())
        .updateMatrix(Matrix().setRotateY(PI).move(0.5f,0,0))
        .updateVelocities();

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         ball. draw(Matrix(Vec(-0.5f,0,0)));
         body->draw(cskel);

         LightPoint(20, Vec(0,3,-3)).add();
      }break;
   }
}
void Draw()
{
   Renderer(Render);
}
/******************************************************************************/
