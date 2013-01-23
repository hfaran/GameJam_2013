/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/material_user_shader.enum.h"
/******************************************************************************/
Mesh box,
    *mesh;

Material refraction_material;
/******************************************************************************/
void CompileShaders()
{
   if(D.shaderModel()==SM_GL)
   {
      ShaderCompile("Shader/Refraction.cpp", S+DataPath()+"Shader/GL/User/Custom Shader", SM_GL);
   }else
   if(D.shaderModel()>=SM_4)
   {
      ShaderCompile("Shader/Refraction.cpp", S+DataPath()+"Shader/4/User/Custom Shader", SM_4);
   }else
   {
      ShaderCompile("Shader/Refraction.cpp", S+DataPath()+"Shader/2/User/Custom Shader", SM_2);
      ShaderCompile("Shader/Refraction.cpp", S+DataPath()+"Shader/3/User/Custom Shader", SM_3);
   }
}
/******************************************************************************/
ShaderTech* GetShader(RENDER_MODE mode, Material *material[4], UInt mesh_base_flag, Int lod_index, Bool allow_tesselation)
{
   if(material[0]==&refraction_material)
   {
      if(mode==RM_BLEND)return Shaders("User/Custom Shader")->firstTech(); // override the RM_BLEND mode
      return NULL; // meshes with 'refraction_material' will not be drawn in any other rendering mode
   }
   return GetDefaultShader(mode, material, mesh_base_flag, lod_index, allow_tesselation);
}
/******************************************************************************/
void InitPre()
{
   App.name("Refraction");
   App.flag=APP_FULL_TOGGLE|APP_MS_EXCLUSIVE;
   DataPath("../data");
   Paks.add("engine.pak");
   D.full(true).ambientPower(0.1f);
   D.setGetShaderFunc(GetShader);
}
Bool Init()
{
   Cam.dist=2;

   // compile shader
   CompileShaders();

   // set background box mesh
   box.parts.New().base.create(Box(2), VTX_TEX0|VTX_NRM|VTX_TAN).reverse();
   box.setMaterial(Materials.ptrRequire("mtrl/brick/0.mtrl")).setRender().setBox();

   // set refraction material and mesh
   refraction_material.reset();
   mesh=Meshes("obj/chr/skeleton/0.mesh");
   mesh->setBase().subdivide().setRender();
   mesh->setMaterial(&refraction_material);

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
         box.draw(MatrixIdentity);

         if(mesh)mesh->draw(MatrixIdentity);

         LightPoint(5, Vec(0,1,-1)).add();
      }break;

      case RM_BLEND:
      {
         C Image &back_buffer=Renderer.getBackBuffer(); // copy the render target data and access it
         if(ShaderImage *si=GetShaderImage("BackBuffer"))si->set(back_buffer); // set obtained back buffer image as shader texture
      }break;
   }
}
void Draw()
{
   Renderer(Render);
}
/******************************************************************************/
