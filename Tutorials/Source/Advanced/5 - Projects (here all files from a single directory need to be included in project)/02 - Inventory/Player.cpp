/******************************************************************************/
#include "stdafx.h"
#include "main.h"
/******************************************************************************/
Player::Player()
{
   InvGui.link(&inv); // when creating a player automatically link him with InvGui
}
/******************************************************************************/
// UPDATE
/******************************************************************************/
Bool Player::update()
{
   if(super::update())
   {
      // detect if the player wants to pickup an item
      if(Lit && Ms.bp(0)) // if hase highlighted object and mouse button pressed
         if(Item *item=CAST(Item, Lit)) // if the object is an item
            itemPickUp(*item); // pick it up

      // update the inventory
      inv.update(T);

      return true;
   }
   return false;
}
/******************************************************************************/
// DRAW
/******************************************************************************/
UInt Player::drawPrepare()
{
             inv.drawPrepare();
   return super::drawPrepare();
}
void Player::drawShadow()
{
             inv.drawShadow();
   return super::drawShadow();
}
/******************************************************************************/
// IO
/******************************************************************************/
void Player::save(File &f)
{
   super::save(f);
      inv.save(f);
}
Bool Player::load(File &f)
{
   if(super::load(f))
   {
      return inv.load(f);
   }
   return false;
}
/******************************************************************************/
