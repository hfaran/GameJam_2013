/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/_enums.h"
/******************************************************************************

   In this tutorial is presented how to use lighting created in World Editor
   Here 2 light sources are used:
      1st is a static light defined in a constant position in the World Editor
      2nd is a light stored in a candle object, as a sub-object

/******************************************************************************/
STRUCT(Item , Game::Item) // extend game items
//{
   Game::ObjLightPoint light_point; // point light

   virtual void create     (Game::ObjParams &obj); // extend creation
   virtual UInt drawPrepare(       );              // extend drawing
   virtual void save       (File &f);              // extend saving
   virtual Bool load       (File &f);              // extend loading
};
/******************************************************************************/
void Item::create(Game::ObjParams &obj)
{
   super::create(obj); // default create

   // add custom children
   for(Game::ObjParamsPtr cur=&obj; cur; cur=cur->base()) // check 'obj' and all its bases
      FREPA(cur->sub_objs) // for each sub-object in 'cur'
      {
         Game::ObjParams &o=cur->sub_objs[i];

         switch(Game::ObjType(o.type())) // check the type of sub-object
         {
            case OBJ_LIGHT_POINT: // if it's light, then create our member from its parameters
            {
               light_point.create(o);
            }break; 
         }
      }
}
/******************************************************************************/
UInt Item::drawPrepare()
{
   light_point.drawPrepare(actor.matrix()); // draw light with item's matrix
   
   return super::drawPrepare(); // default draw
}
/******************************************************************************/
void Item::save(File &f)
{
   super::save(f); // save default data

   light_point.save(f); // save point light
}
Bool Item::load(File &f)
{
   if(super::load(f)) // load default data
   {
      light_point.load(f); // load point light
      return true;
   }
   return false;
}
/******************************************************************************/
Game::ObjMemx<Item               > Items;
Game::ObjMemx<Game::Static       > Statics;
Game::ObjMemx<Game::ObjLightPoint> ObjLightPoints; // container for point light objects
/******************************************************************************/
void InitPre()
{
   App.name("Lights");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true);

   Cam.dist =10;
   Cam.yaw  =-PI_4;
   Cam.pitch=-0.5f;
   Cam.at.set(16,0,16);
}
/******************************************************************************/
Bool Init()
{
   Physics.create(CSS_NONE, true, "../Installation/PhysX");

   Game::World.init      (                               )
              .setObjType(Items         , OBJ_ITEM       )
              .setObjType(Statics       , OBJ_STATIC     )
              .setObjType(ObjLightPoints, OBJ_LIGHT_POINT)
              .New       ("world/lights.world"           )
              .update    (Cam.at                         );

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
}
/******************************************************************************/
