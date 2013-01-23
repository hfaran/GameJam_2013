/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/_enums.h"
/******************************************************************************/
STRUCT(Player , Game::Chr)
//{
   virtual void animate(); // extend skeleton animation
   virtual Bool update ();

   // optionally you can override default 'face blend' value by defining your own function
   virtual Flt animateFaceBlend();
};
/******************************************************************************/
void Player::animate()
{
   super::animate(); // call default animations

   // facial animation
   {
      Flt blend=animateFaceBlend(); // get facial animation blending value
      // this is a factor which determines the intensity of facial animations
      // for example if the character is near the camera the blend is full (1.0) so facial animations will be performed normally
      // and if the character is far away so face isn't visible very well, blending value will be disabled (0.0)
      // disabling facial animations speeds up rendering process, so its visible only in some distance from camera

      if(blend>0)
      {
         Flt time       =Time.time(),
             smile_blend=1;

         cskel.animate(L"anim/face/smile.anim", time, smile_blend*blend);
      }
   }
}
Flt Player::animateFaceBlend()
{
   Flt distance        =Dist(pos(), Cam.matrix.pos), // distance from camera
       full_blend_range=1   ,                        // custom range where the facial animation should be full     (1   meter)
         no_blend_range=1.5f;                        // custom range where the facial animation should be disabled (1.5 meter)

   return LerpRS(no_blend_range, full_blend_range, distance); // this will returned a saturated value 0..1
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

   return super::update();
}
/******************************************************************************/
Game::ObjMemx<Game::Static> Statics; // container for static objects
Game::ObjMemx<      Player> Players; // container for player objects
/******************************************************************************/
void InitPre()
{
   App.name("Character Facial Animation");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true).shadowSoft(1).shadowJitter(true).shadowMapSize(2048).ambientPower(0.3f).hpRt(true).viewRange(20);
}
/******************************************************************************/
Bool Init()
{
   Physics.create(CSS_NONE, true, "../Installation/PhysX");
   Sky    .atmospheric();
   Sun.image="Env/Sky/sun.gfx"; Sun.light_color=1-D.ambientColor();

   // create the world
   Game::World.init()
              .setObjType(Statics, OBJ_STATIC )
              .setObjType(Players, OBJ_PLAYER )
              .New       ("world/sample.world");

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

   if(Players.elms())Cam.setSpherical(Players[0].cskel.getPoint("Head").pos, Players[0].angle.x+PI, -Players[0].angle.y, 0, Max(0.1f, Cam.dist*ScaleFactor(Ms.wheel()*-0.2f))).updateVelocities().set();

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

   if(Players.elms())D.text(0, 0.9f, S+"Facial blending factor for Players[0] is: "+Players[0].animateFaceBlend());
}
/******************************************************************************/
