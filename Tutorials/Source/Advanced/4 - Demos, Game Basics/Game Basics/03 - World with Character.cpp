/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/_enums.h"
/******************************************************************************

   In this tutorial is presented how to combine extending base classes with World Manager usage

/******************************************************************************/
STRUCT(Player , Game::Chr) // extend character structure by defining a player class based on the character
//{
   Memx<Game::Item> items;                                                                   // here is the characters inventory, a container of items
   virtual Memx<Game::Obj>* itemContainer() {Memx<Game::Obj> &items=T.items; return &items;} // override default method of character, to return proper item container

           void updateItems(); // update items actions
   virtual Bool update     (); // here we'll update the player
};
/******************************************************************************/
Game::ObjMemx<Game::Static> Statics; // container for static objects
Game::ObjMemx<Game::Item  > Items  ; // container for item   objects
Game::ObjMemx<      Player> Players; // container for player objects
/******************************************************************************/
void Player::updateItems()
{
   if(Kb.bp(KB_1)) // try to pickup an item
      if(Items.elms()) // if world items container has some elements
         itemPickUp(Items[0]); // pick up the first valid item

   if(Kb.bp(KB_2)) // try to drop down an item
      if(items.elms()) // if inventory has some items
         itemDropDown(items[0]); // drop down the first item

   if(!Kb.alt())grabStop();else // if don't want to grab
   {
      if(grab.is()) // if already grabbing
      {
         Vec pos;
         SinCos(pos.z, pos.x, angle.x+PI_2); // set direction according to player angle
         pos *=ctrl.radius()+0.5f;           // set radius    according to player controller radius
         pos.y=ctrl.height()*0.4f;           // set vertical  position
         pos +=T.pos();
         grab.pos(pos);                      // set desired grab position
      }else
      if(Items.elms()) // if isn't grabbing anything check for presence of world items
      {
         grabStart(Items[0]); // grab first present
      }
   }
}
/******************************************************************************/
Bool Player::update()
{
   if(action)
   {
      if(Kb.b(KB_W) || Kb.b(KB_S) || Kb.b(KB_A) || Kb.b(KB_D) || Kb.b(KB_Q) || Kb.b(KB_E))actionBreak();
   }

   if(!action)
   {
      // turn & move
      input.turn.x=Kb.b(KB_Q)-Kb.b(KB_E);
      input.turn.y=Kb.b(KB_T)-Kb.b(KB_G);
      input.move.x=Kb.b(KB_D)-Kb.b(KB_A);
      input.move.z=Kb.b(KB_W)-Kb.b(KB_S);
      input.move.y=Kb.b(KB_SPACE)-Kb.b(KB_LSHIFT);

      // dodge, crouch, walk, jump
      input.dodge = Kb.bd(KB_D)-Kb.bd(KB_A);
      input.crouch= Kb.b (KB_LSHIFT);
      input.walk  = Kb.b (KB_LCTRL );
      input.jump  =(Kb.bp(KB_SPACE ) ? 3.5f : 0);

      // mouse turn
      Flt max=DegToRad(900)*Time.d();
      angle.x-=Mid(Ms.d().x*1.7f, -max, max);
      angle.y+=Mid(Ms.d().y*1.7f, -max, max);
   }

   updateItems();
   return super::update();
}
/******************************************************************************/
void InitPre()
{
   App.name("World with Character");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true).hpRt(true).hdr(true);
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.scale*=0.8f;

   Physics.create(CSS_NONE, true, "../Installation/PhysX");

   Sun.image="Env/Sky/sun.gfx";
   Sky.atmospheric();

   // create the world
   Game::World.init()
              .setObjType(Statics, OBJ_STATIC)
              .setObjType(Players, OBJ_PLAYER) // please note that here we'll use 'Players' memory container for 'OBJ_PLAYER' objects
              .setObjType(Items  , OBJ_ITEM  )
              .New("world/Test001.world");

   Cam.setSpherical(Vec(29,69,21), -PI_4, -0.5f, 0, 10).set(); // set initial camera

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

   Game::World.update(Cam.at); // update world to given position

    // settings
   Cam.yaw  -=Ms.d().x;                         // modify yaw      according to mouse delta x
   Cam.pitch+=Ms.d().y;                         // modify pitch    according to mouse delta y
   Cam.roll +=(Kb.b(KB_Z)-Kb.b(KB_X))*Time.d(); // modify roll     according to Z and X keyboard buttons
   Cam.dist -=Ms.wheel()*40.0f;                  // modify distance according to mouse wheel
   //Clamp(Cam.dist, 0.1f, 10);                   // clamp  distance to minimum and maximum values

   if(Kb.b(KB_LEFT ))Cam.at-=Cam.matrix.x*Time.d(); // move camera left  on left  arrow key
   if(Kb.b(KB_RIGHT))Cam.at+=Cam.matrix.x*Time.d(); // move camera right on right arrow key
   if(Kb.b(KB_UP   ))Cam.at+=Cam.matrix.y*Time.d(); // move camera up    on up    arrow key
   if(Kb.b(KB_DOWN ))Cam.at-=Cam.matrix.y*Time.d(); // move camera down  on down  arrow key

   // camera
   Cam.setSpherical    (Cam.at, Cam.yaw, Cam.pitch, Cam.roll, Cam.dist); // set spherical camera with 'look at' position, angles and distance
   Cam.updateVelocities(                                              ); // after camera settings are up, we need to update camera velocities in order to achieve correct motion blur when enabled
   Cam.set             (                                              ); // set as active camera


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
                     D.text(0, 0.9f, "Press WSAD keys to move, 1/2 to pick up/drop item, Alt to grab");
   if(Players.elms())D.text(0, 0.8f, S+"Items in inventory : "+Players[0].items.elms());
}
/******************************************************************************/
