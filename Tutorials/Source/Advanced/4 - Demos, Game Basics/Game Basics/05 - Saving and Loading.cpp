/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/_enums.h"
/******************************************************************************

   In this tutorial is presented how to save and load World states

/******************************************************************************/
Game::ObjMemx<Game::Item> Items;
/******************************************************************************/
void InitPre()
{
   App.name("Lights");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true).ambientPower(0.3f).hpRt(true);

   Cam.dist =10;
   Cam.yaw  =-PI_4;
   Cam.pitch=-0.5f;
   Cam.at.set(16,0,16);
}
/******************************************************************************/
Bool Init()
{
   Physics.create(CSS_NONE, true, "../Installation/PhysX");
   Sky    .atmospheric();
   Sun.image="Env/Sky/sun.gfx"; Sun.light_color=1-D.ambientColor(); Sun.pos.set(0,1,0);

   Game::World.init      (                     )
              .setObjType(Items, OBJ_ITEM      )
              .New       ("world/barrels.world")
              .update    (Cam.at               );

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
   CamHandle(0.1f, 100, CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT));

   if(Kb.bp(KB_F2))Game::World.save("LocalData/save.sav"); // save game
   if(Kb.bp(KB_F3))Game::World.load("LocalData/save.sav"); // load game

   Game::World.update(Cam.at);

   // add some velocity to objects
   if(Kb.bp(KB_SPACE))REPA(Items)Items[i].actor.addVel(5*Random.dir(Vec(0,1,0), 0.7f));

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
   D.text(0, 0.9f, "Press F2 to save and F3 to load");
   D.text(0, 0.8f, "Press Space to add velocity");
}
/******************************************************************************/
