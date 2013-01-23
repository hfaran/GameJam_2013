/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/_enums.h"
/******************************************************************************/
Game::ObjMemx<Game::Static> Statics; // container for static objects
Game::ObjMemx<Game::Chr   > Chrs   ; // container for player objects
/******************************************************************************/
void InitPre()
{
   App.name("Character Ragdoll");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true).shadowSoft(1).ambientPower(0.3f).hpRt(true).viewRange(30);

   Cam.at.set(16,0,16);
   Cam.yaw   =-PI_4;
   Cam.pitch =-0.5f;
   Cam.dist  =4;
}
/******************************************************************************/
Bool Init()
{
   Physics.create(CSS_NONE, true, "../Installation/PhysX");
   Sky    .atmospheric();
   Sun.image="Env/Sky/sun.gfx"; Sun.light_color=1-D.ambientColor();

   // create the world
   Game::World.init()
              .setObjType(Statics, OBJ_STATIC )
              .setObjType(Chrs   , OBJ_PLAYER )
              .New       ("world/sample.world");

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

   Game::World.update(Cam.at);

   if(Chrs.elms())
   {
      Game::Chr &chr=Chrs[0];

      if(Kb.bp(KB_1))chr.ragdollEnable (); // switch to ragdoll
      if(Kb.bp(KB_2))chr.ragdollDisable(); // switch to skeleton animation

      if(Kb.bp(KB_Q))if(chr.ragdollBlend())if(Ragdoll::Bone *bone=chr.ragdoll.findBone("Head" ))bone->actor.addVel(Vec(0,0,3));
      if(Kb.bp(KB_W))if(chr.ragdollBlend())if(Ragdoll::Bone *bone=chr.ragdoll.findBone("Body" ))bone->actor.addVel(Vec(0,0,3));
      if(Kb.bp(KB_E))if(chr.ragdollBlend())if(Ragdoll::Bone *bone=chr.ragdoll.findBone("FootR"))bone->actor.addVel(Vec(0,0,4));
      if(Kb.bp(KB_R))if(chr.ragdollBlend())if(Ragdoll::Bone *bone=chr.ragdoll.findBone("HandR"))bone->actor.addVel(Vec(0,0,4));

      Cam.setSpherical(chr.pos(), Cam.yaw-Ms.d().x, Cam.pitch+Ms.d().y, 0, Max(0.1f, Cam.dist*ScaleFactor(Ms.wheel()*-0.2f))).updateVelocities().set();
   }

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

   if(Ms.b(0))Physics.draw();

   D.text(0, 0.9f, "Press 1,2 to switch between skeleton<->ragdoll animation");
   D.text(0, 0.8f, "Press q,w,e,r to simulate shot hits");
}
/******************************************************************************/
