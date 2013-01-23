/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Mesh      box  ,
         *body ;
CSkeleton cskel;
/******************************************************************************/
void CompileShaders()
{
   if(D.shaderModel()==SM_GL)
   {
      ShaderCompile("Shader/Skinning.cpp", S+DataPath()+"Shader/GL/User/Custom Shader", SM_GL);
   }else
   if(D.shaderModel()>=SM_4)
   {
      ShaderCompile("Shader/Skinning.cpp", S+DataPath()+"Shader/4/User/Custom Shader", SM_4);
   }else
   {
      ShaderCompile("Shader/Skinning.cpp", S+DataPath()+"Shader/2/User/Custom Shader", SM_2);
      ShaderCompile("Shader/Skinning.cpp", S+DataPath()+"Shader/3/User/Custom Shader", SM_3);
   }
}
/******************************************************************************/
ShaderTech* GetShader(RENDER_MODE mode, Material *material[4], UInt mesh_base_flag, Int lod_index, Bool allow_tesselation)
{
   if(mesh_base_flag&VTX_SKIN) // here for simplicity - detect if the mesh has skinning data in the vertex
   {
      if(mode==RM_SOLID)return Shaders("User/Custom Shader")->firstTech(); // override only for the RM_SOLID mode
   }
   return GetDefaultShader(mode, material, mesh_base_flag, lod_index, allow_tesselation);
}
/******************************************************************************/
void InitPre()
{
   App.name("Skinning");
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

   // load skinned mesh
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

   // animate the controlled skeleton to walking
   cskel.clear()
        .animate(L"anim/walk.anim", Time.time())
        .updateMatrix(Matrix().setRotateY(PI))
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
         box  .draw(MatrixIdentity);
         body->draw(cskel);

         LightPoint(5, Vec(0,1,-1)).add();
      }break;
   }
}
void Draw()
{
   Renderer(Render);
}
/******************************************************************************/
