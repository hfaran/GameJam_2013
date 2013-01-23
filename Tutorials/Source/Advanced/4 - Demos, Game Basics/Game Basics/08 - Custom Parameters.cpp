/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/_enums.h"
/******************************************************************************/
// ITEM
/******************************************************************************/
STRUCT(Item , Game::Item) // extend items
//{
   Int value; // add new parameter 'value'
   Str name ; // add new parameter 'name'

   virtual void create(Game::ObjParams &obj); // extend default creation

   // since new parameters are declared we need to properly initialize them in constructor, and save/load them in IO methods:

   // constructor
   Item();

   // io methods
   virtual void save(File &f);
   virtual Bool load(File &f);
};
/******************************************************************************/
void Item::create(Game::ObjParams &obj)
{
   super::create(obj); // default create

   // now setup custom parameters from 'obj'
   if(Param *par=obj.findParam("name" ))name =par->asText();
   if(Param *par=obj.findParam("value"))value=par->asInt ();
}
/******************************************************************************/
Item::Item()
{
   value=0;
}
/******************************************************************************/
void Item::save(File &f)
{
   super::save(f); // default save

   f<<value; f.putStr(name); // save custom parameters
}
Bool Item::load(File &f)
{
   if(super::load(f)) // if default load was successful
   {
      f>>value; f.getStr(name); // load custom parameters
      return true;    // return success
   }
   return false; // return failure
}
/******************************************************************************/
// MAIN
/******************************************************************************/
Game::ObjMemx<Item> Items;
/******************************************************************************/
void InitPre()
{
   App.name("Custom Parameters");
   App.flag=APP_FULL_TOGGLE|APP_MS_EXCLUSIVE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true).ambientPower(0.3f).hpRt(true).viewRange(50);

   Cam.at.set(16,0,16);
   Cam.dist = 10;
   Cam.pitch=-PI_3;
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.scale*=0.8f;

   Physics.create(CSS_NONE, true, "../Installation/PhysX");
   Sky    .atmospheric();
   Sun.image="Env/Sky/sun.gfx"; Sun.light_color=1-D.ambientColor();

   Game::World.init      (               )
              .setObjType(Items, OBJ_ITEM)
              .New       ("world/custom params.world");

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

   // draw item parameters
   REPA(Items)
   {
      Item  &item  =Items[i];                // get i-th Items
      Vec2   screen=PosToScreen(item.pos()); // convert world position to screen position
      D.text(screen, item.name);             // draw item's name
   }
}
/******************************************************************************/
