/******************************************************************************/
#include "stdafx.h"
#include "main.h"
/******************************************************************************/
Inventory::~Inventory()
{
   if(inv_gui)inv_gui->unlink();
}
/******************************************************************************/
// GET
/******************************************************************************/
Bool Inventory::slotCanBePutTo(Int src, Int dest)
{
   if(!InRange(src, SLOT_NUM) || !InRange(dest, SLOT_NUM))return false;

   if(slot[src].valid())switch(dest)
   {
      case SLOT_HEAD:
      case SLOT_BODY:
         if(slot[src]().type==ITEM_WEAPON)return false;
      break;
   }

   return true;
}
Bool Inventory::slotsCanBeSwapped(Int a, Int b)
{
   return slotCanBePutTo(a, b) && slotCanBePutTo(b, a);
}
/******************************************************************************/
// OPERATIONS
/******************************************************************************/
void Inventory::itemRemoved(Game::Obj &item) // when an item is being removed from a character
{
   // perform check if it is a equipped item
   REPA(slot) // for all slots
      if(slot[i]==item) // if i-th slot is set to item which is being removed
         slot[i].clear(); // clear the slot so it can no longer be referenced to the removed item
}
void Inventory::itemRemoved(               ) {setGui();} // when item has been removed from a character
void Inventory::itemAdded  (Game::Obj &item) {setGui();} // when item has been added to a character
void Inventory::setGui     (               ) {if(inv_gui)inv_gui->setGui();}
/******************************************************************************/
// UPDATE
/******************************************************************************/
void Inventory::update(Game::Chr &owner)
{
   if(inv_gui)inv_gui->update(owner);

   // set matrixes for items in hands
   if(slot[SLOT_ARM_L].valid())
      if(C OrientP *point=owner.cskel.findPoint("HandL"))
         slot[SLOT_ARM_L]().matrix(Matrix().setPosDir(point->pos, point->perp, point->dir));

   if(slot[SLOT_ARM_R].valid())
      if(C OrientP *point=owner.cskel.findPoint("HandR"))
         slot[SLOT_ARM_R]().matrix(Matrix().setPosDir(point->pos, point->perp, point->dir));
}
/******************************************************************************/
// DRAW
/******************************************************************************/
void Inventory::drawPrepare()
{
   // draw items in hands
   if(slot[SLOT_ARM_L].valid())slot[SLOT_ARM_L]().drawPrepare();
   if(slot[SLOT_ARM_R].valid())slot[SLOT_ARM_R]().drawPrepare();
}
void Inventory::drawShadow()
{
   // draw items in hands
   if(slot[SLOT_ARM_L].valid())slot[SLOT_ARM_L]().drawShadow();
   if(slot[SLOT_ARM_R].valid())slot[SLOT_ARM_R]().drawShadow();
}
/******************************************************************************/
// IO
/******************************************************************************/
void Inventory::save(File &f)
{
   FREPA(slot) // for all slots
      f.putInt(items.validIndex(&slot[i]())); // store the valid index of i-th slot item in 'items' container
}
Bool Inventory::load(File &f)
{
   FREPA(slot) // for all slots
   {
      Int item_index=f.getInt(); // read index of i-th slot in 'items' container

      if(InRange(item_index, items)) // if the index is in valid range
         slot[i]=items[item_index];  // set the slot to point to requested item
      else
         slot[i].clear(); // clear the item reference in i-th slot
   }
   setGui();
   return true;
}
/******************************************************************************/
