/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/_enums.h"
/******************************************************************************/
STRUCT(Player , Game::Chr)
//{
   Motion attack;

   virtual void animate(); // extend skeleton animation
   virtual Bool update ();

   // io
   virtual void save(File &f);
   virtual Bool load(File &f);
};
/******************************************************************************/
void Player::animate()
{
   super::animate(); // call default animations

   // now the skeleton is animated with default character animations (walking, running, crouching, ..)
   // so after the basic animations we can optionally replace them with custom ones, for example attacking:
   cskel.animate(attack, true); // animate skeleton with 'attack' animation motion, 'true' for replace animation mode
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

   // update animation
   {
      if(Kb.bp(KB_ENTER)) // on enter pressed
         attack.set(cskel, "anim/swing/r-l.anim"); // initialize "right-hand swing to left direction" attack animation

      attack.updateAuto(3,3,1); // update attack animation motion
   }

   return super::update();
}
/******************************************************************************/
void Player::save(File &f)
{
   super::save(f);

   attack.save(f);
}
Bool Player::load(File &f)
{
   if(super::load(f))
   {
      return attack.load(f, cskel);
   }
   return false;
}
/******************************************************************************/
Game::ObjMemx<Game::Static> Statics;
Game::ObjMemx<Game::Item  > Items  ;
Game::ObjMemx<      Player> Players;
/******************************************************************************/
void InitPre()
{
   App.name("Character Animations");
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

   Game::World.init()
              .setObjType(Statics, OBJ_STATIC  )
              .setObjType(Items  , OBJ_ITEM    )
              .setObjType(Players, OBJ_PLAYER  )
              .New       ("world/sample.world");

   Cam.setSpherical(Vec(16,0,16), -PI_4, -0.5f, 0, 10).set();

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
   D.text(0, 0.9f, "Press Enter to play a custom animation");
}
/******************************************************************************/
