/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/_enums.h"
/******************************************************************************

   In this tutorial is presented how to dynamically create objects

/******************************************************************************/
Game::ObjMemx<Game::Item> Items;
/******************************************************************************/
void InitPre()
{
   App.name("Dynamically Created Objects");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true).ambientPower(0.3f).hpRt(true);

   Cam.dist =16;
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

   Game::World.update(Cam.at);

   if(Kb.bp(KB_SPACE)) // on space
   {
      Game::ObjParamsPtr obj=Game::Objs.ptrRequire("obj/item/misc/barrel/0.obj"); // get barrel object parameters
      Game::World.objCreate(*obj, Matrix(obj->scale(), Vec(16,8,16)));            // create new object at (16,8,16) position and give objects default scaling
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
   D.text(0, 0.9f, "Press Space to add a Barrel");
}
/******************************************************************************/
