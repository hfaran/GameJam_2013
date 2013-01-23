/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/_enums.h"
/******************************************************************************

   Here we'll present how to detect camera collisions

/******************************************************************************/
STRUCT(Player , Game::Chr)
//{
   virtual Bool update();
};
/******************************************************************************/
Camera desired_camera; // create a helper desired camera
/******************************************************************************/
Game::ObjMemx<Game::Static> Statics;
Game::ObjMemx<Game::Item  > Items  ;
Game::ObjMemx<      Player> Players;
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
   }

   return super::update();
}
/******************************************************************************/
void InitPre()
{
   App.name("Camera Collisions");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true).ambientPower(0.3f).hpRt(true);
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
              .setObjType(Players, OBJ_PLAYER)
              .setObjType(Items  , OBJ_ITEM  )
              .New("world/sample.world");

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

   // setup the camera
   if(Players.elms()) // if we have at least one player
   {
      // first setup the desired camera as in the previous tutorials
      desired_camera.yaw  -=Ms.d().x;           // update camera yaw   angle according to mouse delta x
      desired_camera.pitch+=Ms.d().y;           // update camera pitch angle according to mouse delta y
      Clamp(desired_camera.pitch, -PI_2, PI_4); // clamp to possible camera pitch angle
      desired_camera.dist=Max(1.0f, desired_camera.dist*ScaleFactor(Ms.wheel()*-0.2f)); // update camera distance according to mouse wheel
      desired_camera.at  =Players[0].pos();
      desired_camera.setSpherical(); // set as spherical from current values, this will set the camera's matrix (desired_camera.matrix)

      // now what we'll do is cast a small sized Ball from starting position to target camera destination
      // we'll stop the ball at first contact point, and set camera at that place

      // create a helper ball which will be used for collision detection
      Ball ball(0.1f, desired_camera.at); // we place it at starting point (where the camera is looking at)

      // now we'll move the ball in the direction where the camera should be
      Physics.move(ball, desired_camera.matrix.pos-ball.pos); // use physics movement to move the ball as far as it can go without any collisions

      // now the ball.pos is located at either maximum movement distance or at nearest collision point
      // having ball's position we can now set the final camera position
      Cam.setPosDir(ball.pos, desired_camera.matrix.z, desired_camera.matrix.y); // we'll use 'desired_camera.matrix' directions which were set in 'setSpherical' camera method

      Cam.updateVelocities().set(); // update camera velocities and activate it
   }
   else // when no player on the scene
   {
      CamHandle(0.1f, 100, CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT)); // default camera handling actions
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
   D.text(0, 0.9f, "Try to move the camera around");
   D.text(0, 0.8f, "and see how it gets blocked by the terrain");
}
/******************************************************************************/
