/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/material_user_shader.enum.h"
/******************************************************************************

   This tutorial presents most of the advanced features of Solid Mode shaders, including:
      -skinning
      -normal mapping
      -alpha  testing
      -detail textures
      -vertex colors
      -glow

   First a shader is created with all possible technique combinations,
      there are many of combinations so the compilation may take a few seconds!

   Once the shader is compiled, you may access the desired technique for a mesh, depending on its parameters.

/******************************************************************************/
Mesh box,
     ball;

MaterialPtr brick;
Material    brick_with_custom_shader;
/******************************************************************************/
Str TechniqueCustomShader(Int skin, Int textures, Int normal_mapping, Int alpha_test, Int detail, Int vtx_color)
{
   Str params=S+    skin+','+textures+','+normal_mapping+','+alpha_test+','+detail+','+vtx_color,
       name  =S+"T"+skin+'_'+textures+'_'+normal_mapping+'_'+alpha_test+'_'+detail+'_'+vtx_color;
   return S+"TECHNIQUE("+name+", VS("+params+"), PS("+params+"));";
}
Str TechniqueCustomShaderName(Int skin, Int textures, Int normal_mapping, Int alpha_test, Int detail, Int vtx_color)
{
   return S+"T"+skin+'_'+textures+'_'+normal_mapping+'_'+alpha_test+'_'+detail+'_'+vtx_color;
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

      REPD(skin          , 2) // 2 possible combinations of skinning       (disabled and enabled)
      REPD(textures      , 3) // 3 possible combinations of textures       (0-no diffuse and no normal, 1-diffuse without normal, 2-diffuse and normal)
      REPD(normal_mapping, 2) // 2 possible combinations of normal mapping (disabled and enabled)
      REPD(alpha_test    , 2) // 2 possible combinations of alpha  testing (disabled and enabled)
      REPD(detail        , 2) // 2 possible combinations of detail texture (disabled and enabled)
      REPD(vtx_color     , 2) // 2 possible combinations of vertex colors  (disabled and enabled)
         techniques+=TechniqueCustomShader(skin, textures, normal_mapping, alpha_test, detail, vtx_color);

      CompileShader("Shader/Solid Mode Advanced.cpp", S+DataPath()+"Shader/"+models[i].path+"/User/Custom Shader", models[i].model, techniques);
   }
}
/******************************************************************************/
ShaderTech* GetShader(RENDER_MODE mode, Material *material[4], UInt mesh_base_flag, Int lod_index, Bool allow_tesselation)
{
   if(material[0])switch(material[0]->user_shader)
   {
      case MUS_CUSTOM:
      {
         if(mode==RM_SOLID)
         {
            Bool skin          =  FlagTest(mesh_base_flag, VTX_SKIN);                                                                                                   // if  skinning       should be used
            Int  textures      =((FlagTest(mesh_base_flag, VTX_TEX0) && material[0]->base_0) ? (material[0]->base_1 ? 2 : 1) : 0                                     ); // get number of used textures
            Bool normal_mapping=(textures>=2 && material[0]->rough>EPSL && D.bumpMode()>=BUMP_NORMAL                                                                 ); // if  normal mapping should be used
            Bool alpha_test    =(textures>=1 && material[0]->technique==MTECH_ALPHA_TEST || material[0]->technique==MTECH_GRASS || material[0]->technique==MTECH_LEAF); // if  alpha  testing should be used
            Bool detail        =(textures>=1 && material[0]->detail_map && material[0]->det_power>EPSL                                                               ); // if  detail texture should be used
            Bool vtx_color     =  FlagTest(mesh_base_flag, VTX_COLOR);                                                                                                  // if  vertex color   should be used

            return Shaders("User/Custom Shader")->getTech(TechniqueCustomShaderName(skin, textures, normal_mapping, alpha_test, detail, vtx_color));
         }
      }break;
   }
   return GetDefaultShader(mode, material, mesh_base_flag, lod_index, allow_tesselation);
}
/******************************************************************************/
void InitPre()
{
   App.name("Advanced Solid Mode Shaders");
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
