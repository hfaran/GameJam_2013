/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Material material_brick,
         material_fur,
         material_glow,
         material_glass;
Mesh     mesh_brick,
         mesh_fur  ,
         mesh_glow ,
         mesh_glass;
/******************************************************************************/
void InitPre()
{
   App.name("Rendering Materials");
   App.flag=APP_MS_EXCLUSIVE;
   Paks.add("../data/engine.pak");

   D.full(true).ambientPower(0);
}
/******************************************************************************/
Bool Init()
{
   Cam.dist=4;

   material_brick.reset();                                    // setup defaults
   material_brick.base_0    ="../data/mtrl/brick/0.gfx"     ; // set diffuse texture
   material_brick.base_1    ="../data/mtrl/brick/0.n.gfx"   ; // set normal  texture
   material_brick.detail_map="../data/mtrl/brick/detail.gfx"; // set detail  texture
   material_brick.validate();                                 // validate changes

   material_fur.reset();             // setup defaults
   material_fur.technique=MTECH_FUR; // set fur technique
   material_fur.det_power =1;        // in  fur technique 'det_power' is used for fur length
   material_fur.det_scale/=2;        // in  fur technique 'det_scale' is used for fur scale
   material_fur.validate();          // validate changes

   material_glow.reset();                      // setup defaults
   material_glow.color.set(0.5f, 0.75f, 1, 0); // set color
   material_glow.glow=0.5f;                    // set glow amount
   material_glow.validate();                   // validate changes

   material_glass.reset();                                            // setup defaults
   material_glass.base_0        ="../data/mtrl/glass/0.gfx"         ; // set diffuse    texture to white
   material_glass.base_1        ="../data/mtrl/glass/0.n.gfx"       ; // set normal     texture
   material_glass.reflection_map="../data/mtrl/glass/reflection.gfx"; // set reflection texture
   material_glass.color.set(0.7f, 0.5f, 0.2f, 0); // set color
   material_glass.specular=0.5f ;                 // set specular
   material_glass.bump    =0.09f;                 // set bump value
   material_glass.rough   =0.05f;                 // set roughness
   material_glass.reflect =0.4f ;                 // set reflection
   material_glass.validate();                     // validate changes

   mesh_brick.parts.New().base.create(Ball(1), VTX_TEX0|VTX_NRM|VTX_TAN); // create Mesh with 1 MeshBase, and create this MeshBase from Ball and automatic texture coordinates, normals and tangents
   mesh_brick.setMaterial(&material_brick).setRender().setBox();          // set mesh rendering version and bounding box

   mesh_fur.parts.New().base.create(Ball(1), VTX_TEX0|VTX_NRM|VTX_TAN); // create Mesh with 1 MeshBase, and create this MeshBase from Ball and automatic texture coordinates, normals and tangents
   mesh_fur.setMaterial(&material_fur).setRender().setBox();            // set mesh rendering version and bounding box

   mesh_glow.parts.New().base.create(Ball(1), VTX_TEX0|VTX_NRM|VTX_TAN); // create Mesh with 1 MeshBase, and create this MeshBase from Ball and automatic texture coordinates, normals and tangents
   mesh_glow.setMaterial(&material_glow).setRender().setBox();           // set mesh rendering version and bounding box

   mesh_glass.parts.New().base.create(Ball(1), VTX_TEX0|VTX_NRM|VTX_TAN); // create Mesh with 1 MeshBase, and create this MeshBase from Ball and automatic texture coordinates, normals and tangents
   mesh_glass.setMaterial(&material_glass).setRender().setBox();          // set mesh rendering version and bounding box

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
   CamHandle(1.5, 10, CAMH_ZOOM|CAMH_ROT);
   
   if(Ms.b(0)) // on left mouse button change colors of materials
   {
      material_brick.color.y=0.5f;
      material_fur  .color.z=0.5f;
   }else
   {
      // restore default rgb color values
      material_brick.color.v3()=1;
      material_fur  .color.v3()=1;
   }
   // validate changes
   material_glow .validate();
   material_brick.validate();
   material_fur  .validate();
   
   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         mesh_brick.draw(Matrix(Vec(-1, 1,0)));
         mesh_fur  .draw(Matrix(Vec( 1, 1,0)));
         mesh_glow .draw(Matrix(Vec( 1,-1,0)));
         mesh_glass.draw(Matrix(Vec(-1,-1,0)));

         LightDir(Vec(0,0,1)).add();
      }break;
   }
}
void Draw()
{
   Renderer(Render);
}
/******************************************************************************/
