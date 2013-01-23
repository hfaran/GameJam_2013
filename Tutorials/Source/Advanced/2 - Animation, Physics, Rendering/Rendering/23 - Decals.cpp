/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Mesh  terrain;
Decal decal[2]; // 2 decals
/******************************************************************************/
void InitPre()
{
   App.name("Decals");
   App.flag=APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true).ambientPower(0.3f).hpRt(true);

   Cam.pitch=-0.5f;
   Cam.dist =10;
}
Bool Init()
{
   terrain.load("obj/terrain/0.mesh");
   Sky.atmospheric();
   Sun.image="Env/Sky/sun.gfx";

   // setup decals
   REPA(decal)
   {
      // get position at terrain surface
      Vec pos; PosPointMeshY(Vec2(i ? 1 : -1, 0), terrain, &pos);

      // set decal data
      Decal &d=decal[i];
      d.color=(i ? Vec4(1,1,0,1) : Vec4(1,0,0,1));
      d.material(Materials.ptrRequire("Decal/star.mtrl"));
      d.matrix.setPosDir(pos, Vec(0,1,0));
   }
   return true;
}
void Shut()
{
}
/******************************************************************************/
Bool Update()
{
   if(Kb.bp(KB_ESC))return false;
   CamHandle(0.01f, 500, CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT));

   // rotate decals matrix in their Z axis according to time delta
   REPA(decal)decal[i].matrix.rotateZVec(Time.d());

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE: terrain.draw(MatrixIdentity); break;

      // decals can be rendered in RM_OVERLAY, RM_BLEND or RM_PALETTE modes, in this tutorial we'll use RM_BLEND
      case RM_BLEND:
         REPA(decal)decal[i].drawStatic();
      break;
   }
}
void Draw()
{
   Renderer(Render);
}
/******************************************************************************/
