/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/_enums.h"
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
   
   virtual UInt drawPrepare(); // extend drawing to include requesting for RM_OVERLAY mode
   virtual void drawOverlay(); // extend drawing to include rendering shadow

   virtual void save(File &f);
   virtual Bool load(File &f);
};
STRUCT(Item , Game::Item)
//{
   virtual UInt drawPrepare(); // extend drawing to include requesting for RM_OVERLAY mode
   virtual void drawOverlay(); // extend drawing to include rendering shadow
};
/******************************************************************************/
Game::ObjMemx<Game::Static> Statics;
Game::ObjMemx<      Item  > Items  ;
Game::ObjMemx<      Player> Players;

Decal blob_shadow; // decal used for drawing the fast shadow
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

   ctrl_pos=ctrl.actor.pos();

   return super::update();
}

UInt Player::drawPrepare()
{
   return super::drawPrepare() | IndexToFlag(RM_OVERLAY);
}
void Player::drawOverlay()
{
   blob_shadow.matrix.setPosDir(T.pos()-Vec(0, ctrl.height()/2, 0), Vec(0,1,0)).scaleOrn(0.9f);
   blob_shadow.drawStatic(0.4f);
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
UInt Item::drawPrepare()
{
   return super::drawPrepare() | IndexToFlag(RM_OVERLAY);
}
void Item::drawOverlay()
{
   if(mesh)
   {
      blob_shadow.matrix.setPosDir(mesh->box.center()*matrix(), Vec(0,1,0)).scaleOrn(0.6f);
      blob_shadow.drawStatic(0.4f);
   }
}
/******************************************************************************/
void InitPre()
{
   App.name("Fast Shadows");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");
   Cam.dist=3;

   D.full(true).hpRt(true).ambientPower(0.3f);

   D.shadowMode(SHADOW_NONE); // disable high quality shadows
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

   // setup blob shadow decal
   blob_shadow.terrain_only=true;
   blob_shadow.material(Materials.ptrRequire("Decal/shadow.mtrl"));

   return true;
}
/******************************************************************************/
void Shut()
{
}
/******************************************************************************/
void SetCamera()
{
   if(Players.elms())
   {
      Cam.dist=Max(1.0f, Cam.dist*ScaleFactor(Ms.wheel()*-0.1));
      Cam.setSpherical(Players[0].ctrl_pos+Vec(0,0.5,0), Players[0].angle.x, Players[0].angle.y, 0, Cam.dist);
      Cam.updateVelocities().set();
   }
   else
   {
      CamHandle(0.1f, 100, CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT));
   }
}
Bool Update()
{
   if(Kb.bp(KB_ESC))return false;
   Game::World.update(Cam.at);

   SetCamera();

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
