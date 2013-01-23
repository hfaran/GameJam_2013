/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Mesh ball;

struct Obj
{
   Vec      pos;
   Material material;

   void create()
   {
      material.reset();
      material.color.v3()=ColorHue(RandomF()).asVec();
      material.validate();
      pos=Random(Box(8, 4, 8, Vec(0,2,0)));
   }

   void draw()
   {
      MaterialLock=&material; ball.draw(Matrix(pos));
      MaterialLock=NULL;
   }
}obj[32];
/******************************************************************************/
void InitPre()
{
   App.name("Cel Shading");
   App.flag=APP_FULL_TOGGLE|APP_MS_EXCLUSIVE;
   DataPath("../data");
   Paks.add("engine.pak");
   D.full(true).ambientPower(0);
}
/******************************************************************************/
Bool Init()
{
   // set cel shading
   D.bumpMode  (BUMP_FLAT      ); // cel-shading works best with flat bump mapping
   D.hpRt      (true           ); // cel-shading works best with high precision render targets
   D.edgeDetect(EDGE_DETECT_FAT); // set edge detection
   Renderer.cel_shade_palette=Images("gfx/fx/cel light palette.gfx"); // set light palette which modifies global lighting

   // set sky, material and mesh
   Sky.atmospheric();

   ball.parts.New().base.create(Ball(0.5f), VTX_TEX0|VTX_NRM|VTX_TAN);
   ball.setRender().setBox();

   REPAO(obj).create();

   Cam.dist=10;

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
   CamHandle(0.01f, 100, CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT));

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         Meshes("obj/terrain/0.mesh")->draw(MatrixIdentity); // draw terrain

         REPAO(obj).draw();

         LightDir(!Vec(1,-1,1)).add();
      }break;
   }
}
void Draw()
{
   Renderer(Render);
}
/******************************************************************************/
