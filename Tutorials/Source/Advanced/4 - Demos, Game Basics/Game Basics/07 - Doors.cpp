/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/_enums.h"
/******************************************************************************/
STRUCT(Door , Game::Door) // extend doors
//{
   virtual UInt drawPrepare(); // extend default drawing, we'll use it to enable door highlighting
};

STRUCT(Chr , Game::Chr) // extend characters
//{
   virtual Bool update(); // extend default updating, we'll use it to detect if when moving there are some closed doors which we'll open automatically
};
/******************************************************************************/
Game::ObjMemx<      Chr   > Chrs   ;
Game::ObjMemx<      Door  > Doors  ; // container for door objects
Game::ObjMemx<Game::Static> Statics;

Game::Obj *Object; // game object under mouse cursor
/******************************************************************************/
UInt Door::drawPrepare() // extended door drawing
{
   if(Object==this)SetHighlight(Color(30,30,30,0)); // if the objects is the one under cursor, enable highlight before drawing it
   UInt modes=super::drawPrepare(); // default draw
   SetHighlight(); // clear highlight to zero
   return modes;
}
/******************************************************************************/
Bool Chr::update() // extended character updating
{
   if(super::update())
   {
      // we wan't to detect if a player is moving to a destination path, and if on its way there is a closed door
      if(action==Game::ACTION_MOVE_TO) // if we're in automatic action move to
      {
         // setup movement direction
         Vec dir;
         dir.y=0;
         SinCos(dir.z, dir.x, angle.x+PI_2); // 'dir' is the direction where the player is moving at the moment

         // setup a ball at characters position
         Ball ball(ctrl.radius(), pos());

         // sweep for an obstacle
         PhysHit phys_hit; 
         if(Physics.sweep(ball, dir, &phys_hit, IndexToFlag(AG_DOOR))) // test for obstacles in 1 meter range, test door actor groups only
         if(Door *door=CAST(Door, phys_hit.obj))                       // if it's actually a door
         {
            door->open(); // automatically open the door
         }
      }
      return true;
   }
   return false;
}
/******************************************************************************/
void InitPre()
{
   App.name("Doors");
   App.flag=APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true).ambientPower(0.3f).hpRt(true).shadowMapSize(2048).viewRange(50);

   Cam.dist = 10;
   Cam.yaw  =-PI_4;
   Cam.pitch=-PI_3;
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.scale*=0.8;

   Physics.create(CSS_NONE, true, "../Installation/PhysX");
   Sky    .atmospheric();
   Sun.image="Env/Sky/sun.gfx"; Sun.light_color=1-D.ambientColor();

   Game::World.init      (                   )
              .setObjType(Chrs   , OBJ_PLAYER)
              .setObjType(Doors  , OBJ_DOOR  )
              .setObjType(Statics, OBJ_STATIC)
              .New       ("world/door.world" )
              .update    (Cam.at             );

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

   // detect object under cursor
   Vec     pos, dir; ScreenToPosDir(Ms.pos(), pos, dir); // convert screen mouse position to world position and direction
   PhysHit phys_hit;
   if(Physics.ray(pos, dir*D.viewRange(), &phys_hit, ~IndexToFlag(AG_CONTROLLER))) // if ray-test hit something
   {
      Object=phys_hit.obj; // get encountered object

      if(Ms.bp(0)) // if LMB is pressed
      {
         if(Door *door=CAST(Door, Object)) // if current object is of 'Door' type
         {
            door->toggle(); // toggle door
         }
         else // if not then move to that position
         {
            if(Chrs.elms())Chrs[0].actionMoveTo(phys_hit.plane.pos);
         }
      }
   }else
   {
      Object=NULL; // clear object pointer
   }

   // rotate camera
   if(Ms.b(1))
   {
      Cam.yaw  -=Ms.d().x;
      Cam.pitch+=Ms.d().y;
   }
   if(Chrs.elms())Cam.setSpherical(Chrs[0].pos(), Cam.yaw, Cam.pitch, 0, Cam.dist*ScaleFactor(Ms.wheel()*-0.2f)).updateVelocities().set();

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
   D.text(0, 0.92f, "Press LMB on the door to open or close it");
   D.text(0, 0.84f, "Press LMB on the ground to move the player");
   D.text(0, 0.76f, "Press RMB to rotate the camera");
}
/******************************************************************************/
