/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/_enums.h"
/******************************************************************************/
STRUCT(Chr , Game::Chr)
//{
   Int move_to; // index of Waypoint point to move to

   virtual void create(Game::ObjParams &obj); // override default creation method to setup custom default animations after character creation

   virtual Bool update(); // override default updating to set movement commands
   
   Chr();
};
/******************************************************************************/
Game::ObjMemx<Game::Static> Statics ;
Game::ObjMemx<      Chr   > Chrs    ;
Game::Waypoint             *waypoint; // pointer to waypoint stored in world
/******************************************************************************/
Chr::Chr()
{
   move_to=0;
}
void Chr::create(Game::ObjParams &obj)
{
   super::create(obj); // call default creation

   // now when object has been created we can optionally override its default animations
   // for example change it's walking animation
   // to achieve this we need to change the default animation from character skeleton animation cache 'Chr::sac'
   // on the test map used with this tutorial, there are 2 characters: human and a skeleton
   // we'll replace the animation only for the skeleton character leaving human character with its default animation

   // detect if current character is a skeleton
   Bool is_skeleton=false;
   for(Game::ObjParamsPtr cur=&obj; cur; cur=cur->base()) // iterate through all ObjParams bases
      if(CChar *name=Game::Objs.name(cur))  // if received the name of the object
         if(Contains(name, "skeleton")) // if the object name contains "skeleton" word
   {
      is_skeleton=true;
      break;
   }
   
   // replace the animation
   if(is_skeleton) // only for the skeleton
   {
      sac.walk=&cskel.getSkelAnim("anim/custom/scary walk.anim"); // this will load the animation, cache it, and set into character's animation
   }
   
   move_walking=true; // order the characters to always move by walking instead of running
}
Bool Chr::update()
{
   if(super::update())
   {
      if(!action) // if not performing any action
      {
         if(waypoint && waypoint->points.elms()) // if have waypoint with some points in it
         {
            move_to=(move_to+1)%waypoint->points.elms(); // set variable to next point index
            actionMoveTo(waypoint->points[move_to].pos); // order the character to move to the point position
         }
      }
      return true;
   }
   return false;
}
/******************************************************************************/
void InitPre()
{
   App.name("Character Default Animations");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true).shadowMode(SHADOW_NONE).ambientPower(0.3f).hpRt(true);
}
/******************************************************************************/
Bool Init()
{
   Physics.create(CSS_NONE, true, "../Installation/PhysX");
   Sky    .atmospheric();
   Sun.image="Env/Sky/sun.gfx"; Sun.light_color=1-D.ambientColor();

   // create the world
   Game::World.init()
              .setObjType(Statics, OBJ_STATIC     )
              .setObjType(Chrs   , OBJ_CHR        )
              .New       ("world/animations.world");

   // access the Waypoint
   waypoint=Game::World.getWaypoint("0"); // load waypoint named "0" stored in current world

   Cam.setSpherical(Vec(17,2,12), -0.4f, -0.9f, 0, 11).set(); // set initial camera

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
   CamHandle(0.1f, 100, CAMH_ZOOM|(Ms.b(1) ? CAMH_MOVE : CAMH_ROT));

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
