/******************************************************************************/
#include "stdafx.h"
#include "main.h"
/******************************************************************************/
Game::ObjMemx<Teleport    > ObjParticles;
Game::ObjMemx<Game::Static> Statics;
Game::ObjMemx<Player      > Players;

UInt View=VIEW_TPP;
/******************************************************************************/
void InitPre()
{
   App.name("Multiple Worlds");
   App.flag=APP_FULL_TOGGLE|APP_MS_EXCLUSIVE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true).hpRt(true).viewRange(70);
   Cam.dist=3;
}
/******************************************************************************/
Bool Init()
{
   Physics.create(CSS_NONE, true, "../Installation/PhysX");
   Sky    .atmospheric();
   Sun.image="Env/Sky/sun.gfx";

   Game::World.init      (                           )
              .setObjType(Players     , OBJ_PLAYER   )
              .setObjType(Statics     , OBJ_STATIC   )
              .setObjType(ObjParticles, OBJ_PARTICLES)
              .New       ("World/teleport 1.world");

   return true;
}
/******************************************************************************/
void Shut()
{
}
/******************************************************************************/
void UpdateCamera()
{
   // set next camera mode when Tab pressed
   if(Kb.bp(KB_TAB))
   {
      View=(View+1)%VIEW_NUM;
   }

   // setup the camera
   if(Players.elms()) // if we have at least one player
   {
      // set camera depending on current view mode
      switch(View)
      {
         case VIEW_FPP:
         {
            C OrientP &head=Players[0].cskel.getPoint("head"); // obtain player "head" skeleton point (this was created in Model Editor)
            Cam.setPosDir(head.pos, head.dir, head.perp); // set camera from 'head' position, direction and perpendicular to direction
         }break;

         default: // VIEW_TPP
         {
            Cam.dist=Max(1.0f, Cam.dist*ScaleFactor(Ms.wheel()*-0.1f)); // update camera distance according to mouse wheel
            Cam.setSpherical(Players[0].ctrl_pos+Vec(0,0.5,0), Players[0].angle.x, Players[0].angle.y, 0, Cam.dist); // set spherical camera looking at player position with given player angles
         }break;
      }

      // after setting camera position and angles:
      Cam.updateVelocities().set(); // update camera velocities and activate it
   }
   else // when no player on the scene
   {
      CamHandle(0.1f, 100, CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT)); // default camera handling actions
   }
}
/******************************************************************************/
void DetectTeleport() // detect if the player is inside a teleporter and change the world
{
   if(Players.elms() && ObjParticles.elms()) // if world has at least one player and particle object
      if(Dist(Players[0].pos(), ObjParticles[0].pos())<=1.5f) // if player stepped on a particle
         if(ObjParticles[0].target_world.is()) // if particle string has a valid text value
   {
      SG.changeWorld(ObjParticles[0].target_world);
   }
}
/******************************************************************************/
Bool Update()
{
   if(Kb.bp(KB_ESC))return false;

   DetectTeleport();
   Game::World.update(Cam.at);
   UpdateCamera();

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
   D.text(0, 0.9f, "Walk into the particles to teleport to a different world");
   D.text(0, 0.8f, S+"Current World: \""+Game::World.worldDir()+'"');
}
/******************************************************************************/
