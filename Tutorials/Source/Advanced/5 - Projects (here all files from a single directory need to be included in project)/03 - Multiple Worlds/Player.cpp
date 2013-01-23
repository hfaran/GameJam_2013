/******************************************************************************/
#include "stdafx.h"
#include "main.h"
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
      Flt  max=DegToRad(900)*Time.d(),
           dx =Ms.d().x*1.7f,
           dy =Ms.d().y*1.7f;
      angle.x-=Mid(dx,-max,max);
      angle.y+=Mid(dy,-max,max);
   }

   ctrl_pos=ctrl.actor.pos();

   return super::update();
}
/******************************************************************************/
UInt Player::drawPrepare()
{
   Bool hide_head=(View==VIEW_FPP && mesh); // disable drawing head when we're in FPP mode

   if(hide_head)mesh->hide("head"); // hide "head" mesh part in 'mesh'

   UInt modes=super::drawPrepare(); // call default drawing

   if(hide_head)mesh->show("head"); // un-hide "head" mesh part, so other objects which use the same mesh will have the head rendered properly
   return modes;
}
/******************************************************************************/
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
