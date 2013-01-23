/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/_enums.h"
/******************************************************************************

   Here we'll present how to properly use different camera modes

/******************************************************************************/
// Define viewing modes:
enum VIEW_MODE // Viewing Mode
{
   VIEW_FPP, // First Person
   VIEW_TPP, // Third Person
   VIEW_ISO, // Isometric
   VIEW_NUM, // number of view modes
};
UInt View; // current VIEW_MODE
/******************************************************************************/
STRUCT(Player , Game::Chr)
//{
   Vec ctrl_pos;

   // get / set
   virtual Vec    pos   (                ) {return super::pos   (      );}
   virtual Matrix matrix(                ) {return super::matrix(      );}
   virtual void   pos   (C Vec    &pos   ) {       super::pos   (pos   ); ctrl_pos=ctrl.actor.pos();}
   virtual void   matrix(C Matrix &matrix) {       super::matrix(matrix); ctrl_pos=ctrl.actor.pos();}

   virtual Bool update();
   
   virtual UInt drawPrepare(); // extend drawing to disable head rendering in FPP mode

   virtual void save(File &f);
   virtual Bool load(File &f);
};
/******************************************************************************/
Game::ObjMemx<Game::Static> Statics; // container for static objects
Game::ObjMemx<Game::Item  > Items  ; // container for item   objects
Game::ObjMemx<      Player> Players; // container for player objects
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
      if(View!=VIEW_ISO) // don't use mouse turning when in Isometric mode
      {
         Flt max=DegToRad(900)*Time.d();
         angle.x-=Mid(Ms.d().x*1.7f, -max, max);
         angle.y+=Mid(Ms.d().y*1.7f, -max, max);
      }
   }

   ctrl_pos=ctrl.actor.pos();

   return super::update();
}

UInt Player::drawPrepare()
{
   Bool hide_head=(View==VIEW_FPP && mesh); // disable drawing head when we're in FPP mode

   if(hide_head)mesh->hide("head"); // hide "head" mesh part in 'mesh'

   UInt modes=super::drawPrepare(); // call default drawing

   if(hide_head)mesh->show("head"); // un-hide "head" mesh part, so other objects which use the same mesh will have the head rendered properly
   return modes;
}

void Player::save(File &f)
{
   super::save(f);
   f<<ctrl_pos;
}
Bool Player::load(File &f)
{
   if(super::load(f))
   {
      f>>ctrl_pos;
      return true;
   }
   return false;
}
/******************************************************************************/
void InitPre()
{
   App.name("Camera Modes");
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
void UpdateCamera()
{
   // set next camera mode when Tab pressed
   if(Kb.bp(KB_TAB))
   {
      View=(View+1)%VIEW_NUM;

      if(View==VIEW_ISO) // when set to isometric view
      {
         Cam.dist =   10; // set bigger camera distance at start
         Cam.pitch=-PI_4; // set starting camera pitch angle
      }
   }

   // setup the camera
   if(Players.elms()) // if we have at least one player
   {
      // set camera depending on current view mode
      switch(View)
      {
         case VIEW_FPP:
         {
            C OrientP &head=Players[0].cskel.getPoint("head"); // obtain player "head" skeleton point (this was created in Mesh Editor)
            Cam.setPosDir(head.pos, head.dir, head.perp); // set camera from 'head' position, direction and perpendicular to direction
         }break;

         case VIEW_TPP:
         {
            Cam.dist=Max(1.0f, Cam.dist*ScaleFactor(Ms.wheel()*-0.1f)); // update camera distance according to mouse wheel
            Cam.setSpherical(Players[0].ctrl_pos+Vec(0,0.5f,0), Players[0].angle.x, Players[0].angle.y, 0, Cam.dist); // set spherical camera looking at player position with given player angles
         }break;

         default: // VIEW_ISO
         {
            Cam.yaw  -=Ms.d().x; // update camera yaw   angle according to mouse delta x
            Cam.pitch+=Ms.d().y; // update camera pitch angle according to mouse delta y
            Clamp(Cam.pitch, -PI_2, 0); // clamp to possible camera pitch angle
            Cam.dist  =Max(1.0f, Cam.dist*ScaleFactor(Ms.wheel()*-0.1f)); // update camera distance according to mouse wheel
            Cam.setSpherical(Players[0].pos(), Cam.yaw, Cam.pitch, 0, Cam.dist); // set spherical camera looking at player using camera angles
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
Bool Update()
{
   if(Kb.bp(KB_ESC))return false;
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
   D.text  (0, 0.9f, "Press Tab to switch camera modes");
}
/******************************************************************************/
