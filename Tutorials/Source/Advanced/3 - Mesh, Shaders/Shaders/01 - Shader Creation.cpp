/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/material_user_shader.enum.h" // include custom user shader enums, if this header values will be changed please rebuild it's binary version by simply running Mesh Editor tool
/******************************************************************************

   I. Making Shader Creation Available

      Using custom shaders is available only if you have purchased the "Shader Headers" from Esenthel Store - http://www.esenthel.com/?id=store
         Download the Shader Headers from Esenthel Store
         Extract the archive to your hard disk.
         Please edit the file "EsenthelEngineSDK\Data\Shader\Main.h",
            and change the path to the place where you have extracted the shader archive.

   II. Compiling Shaders

      Shaders need to be stored in following folders in your GameData location:
         "Shader/2/User/"  - here "2"  specifies shaders for Shader Model 2.0
         "Shader/3/User/"  - here "3"  specifies shaders for Shader Model 3.0
         "Shader/4/User/"  - here "4"  specifies shaders for Shader Model 4.0+
         "Shader/GL/User/" - here "GL" specifies shaders for OpenGL

      You must use the "User" subfolder to avoid accidental names collisions with already available engine shaders.
         For example if the engine has a shader "sample shader" located in "Shader/3/sample shader"
         Then if you create your own "sample shader" and store it in "Shader/3/sample shader"
         The engine will get confused which shader to use.
         So for avoiding names collisions, please store all your shaders in "User" subfolder, like this "Shader/3/User/sample shader"

   III. Using Compiled Shaders

      When accessing shaders in the game, do not specify the full path
         for example don't specify    - "Shader/3/User/custom shader"
         but specify only shader name - "User/custom shader"
         The engine will automatically add "Shader/2/", "Shader/3/", ... prefixes depending on which Shader Model is available.

      In order to specify custom shaders usage you must use a global function which will override default shader assignment.
         To do that we'll create a 'GetShader' function which will return manually selected shaders according to mesh parameters.

/******************************************************************************/
Mesh box,
     ball;

MaterialPtr brick;
Material    brick_with_custom_shader; // use a separate material to make an easy detection wheter to use custom or default shader
/******************************************************************************/
void CompileShaders()
{
   // typically shaders will be compiled in a separate tool, so the game will only load compiled data
   // but for simplicity we'll compile the shaders here

   if(D.shaderModel()==SM_GL)
   {
      ShaderCompile("Shader/Shader.cpp", S+DataPath()+"Shader/GL/User/Custom Shader", SM_GL); // compile using OpenGL and store output file in "Shader/GL/User"
   }else
   if(D.shaderModel()>=SM_4)
   {
      ShaderCompile("Shader/Shader.cpp", S+DataPath()+"Shader/4/User/Custom Shader", SM_4); // compile using shader model 4.0 and store output file in "Shader/4/User"
   }else
   {
      ShaderCompile("Shader/Shader.cpp", S+DataPath()+"Shader/2/User/Custom Shader", SM_2); // compile using shader model 2.0 and store output file in "Shader/2/User"
      ShaderCompile("Shader/Shader.cpp", S+DataPath()+"Shader/3/User/Custom Shader", SM_3); // compile using shader model 3.0 and store output file in "Shader/3/User"
   }
}
/******************************************************************************/
ShaderTech* GetShader(RENDER_MODE mode, Material *material[4], UInt mesh_base_flag, Int lod_index, Bool allow_tesselation) // this is the function which will manually choose shaders for the meshes when needed
{
   if(material[0])switch(material[0]->user_shader) // test if the first material has 'user_shader' specified
   {
      case MUS_CUSTOM: // if it's set to custom
      {
         // set shader only for RM_BLEND mode, because this is the simplest mode to make shaders for
         if(mode==RM_BLEND)
         {
            return Shaders("User/Custom Shader")->firstTech(); // load the custom shader from shaders cache
         }else
         {
            return NULL; // return NULL so the mesh won't be displayed in any other rendering mode
         }
      }break;
   }

   // in other case
   return GetDefaultShader(mode, material, mesh_base_flag, lod_index, allow_tesselation); // return the default shader
}
/******************************************************************************/
void InitPre()
{
   App.name("Shader Creation");
   App.flag=APP_FULL_TOGGLE|APP_MS_EXCLUSIVE;
   DataPath("../data");
   Paks.add("engine.pak");
   D.ambientPower(0.1f);

   D.setGetShaderFunc(GetShader); // specify manual shader selection, by setting custom 'GetShader' function, which will manually choose which shader to use for the meshes
}
Bool Init()
{
   Cam.dist=3;

   // compile shader
   CompileShaders();

   // load materials
   brick.require("mtrl/brick/0.mtrl"); // load material from file
   brick_with_custom_shader=*brick                ; // copy all parameters from 'brick' to 'brick_with_custom_shader'
   brick_with_custom_shader.user_shader=MUS_CUSTOM; // change this materials 'user_shader' enum value to 'MUS_CUSTOM'
   brick_with_custom_shader.validate();             // changing parameters requires validation

   // create meshes
   box .parts.New().base.create( Box(5), VTX_TEX0|VTX_NRM|VTX_TAN).reverse();
   ball.parts.New().base.create(Ball(1), VTX_TEX0|VTX_NRM|VTX_TAN)          ;

   // set materials
   box .setMaterial( brick                   ).setRender().setBox(); // set 'brick                   ' for box  mesh
   ball.setMaterial(&brick_with_custom_shader).setRender().setBox(); // set 'brick_with_custom_shader' for ball mesh, this will make the ball mesh to use the custom shader

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
