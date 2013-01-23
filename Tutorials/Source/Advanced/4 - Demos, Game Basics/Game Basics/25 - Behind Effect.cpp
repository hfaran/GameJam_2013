/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/_enums.h"
/******************************************************************************/
STRUCT(Chr , Game::Chr)
//{
   virtual UInt drawPrepare(); // extend drawing to include requesting for RM_BEHIND mode
   virtual void drawBehind (); // extend drawing to include rendering "behind effect"
};
/******************************************************************************/
Game::ObjMemx<Game::Static> Statics;
Game::ObjMemx<      Chr   > Chrs   ;
/******************************************************************************/
UInt Chr::drawPrepare()
{
   return super::drawPrepare() | IndexToFlag(RM_BEHIND);
}
void Chr::drawBehind()
{
   if(mesh)if(Frustum(Ball().setAnimated(mesh->box, cskel)))
   {
      SetBehindBias(mesh->box.h()*cskel.scale());
      mesh->drawBehind(Color(64,128,255,255), Color(255,255,255,0), cskel);
   }
}
/******************************************************************************/
void InitPre()
{
   App.name("\"Behind\" Effect");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");
   Cam.dist=3;

   D.full(true).hpRt(true).ambientPower(0.3f);
}
/******************************************************************************/
Bool Init()
{
   Physics.create(CSS_NONE, true, "../Installation/PhysX");
   Sky    .atmospheric();
   Sun.image="Env/Sky/sun.gfx"; Sun.light_color=1-D.ambientColor();

   // create the world
   Game::World.init()
              .setObjType(Statics, OBJ_STATIC)
              .setObjType(Chrs   , OBJ_CHR   )
              .New("world/behind.world");

   return true;
}
/******************************************************************************/
void Shut()
{
}
/******************************************************************************/
void SetCamera()
{
   if(Chrs.elms())
   {
      Cam.setSpherical(Chrs[0].pos(), Time.time()/3, 0, 0, 7).updateVelocities().set();
   }
   else
   {
      CamHandle(0.1f, 100, CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT));
   }
}
Bool Update()
{
   if(Kb.bp(KB_ESC))return false;
   Game::World.update(Cam.at);
   SetCamera();
   return true;
}
/******************************************************************************/
void Render()
{
   Game::World.draw();
}
void Draw()
{
   Renderer(Render);
}
/******************************************************************************/
