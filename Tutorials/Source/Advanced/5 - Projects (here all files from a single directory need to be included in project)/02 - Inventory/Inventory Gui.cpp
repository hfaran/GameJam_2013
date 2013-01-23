/******************************************************************************/
#include "stdafx.h"
#include "main.h"
/******************************************************************************/
InventoryGui InvGui;
/******************************************************************************/
// MANAGE
/******************************************************************************/
void InventoryGui::create()
{
   // window
   Gui+=window.create(Rect_R(D.w(), 0, 1, 1.5f), "Inventory");

   // slots
   {
      // background images
      Flt      w   =0.11f, // default unit width
               s   =0.03f; // default unit spacing
      ImagePtr slot=Images.ptrRequire("gfx/inventory/slot.gfx");
      window+=slot_img[SLOT_HEAD ][0].create(Rect_U (window.crect.w()/2                 ,                              -0.05f, w*2, w*2),slot).desc("Head");
      window+=slot_img[SLOT_BODY ][0].create(Rect_U (window.crect.w()/2                 , slot_img[SLOT_HEAD][0].rect.min.y-s, w*2, w*3),slot).desc("Body");
      window+=slot_img[SLOT_ARM_L][0].create(Rect_RU(slot_img[SLOT_BODY][0].rect.min.x-s, slot_img[SLOT_BODY][0].rect.max.y  , w*2, w*3),slot).desc("Left Arm");
      window+=slot_img[SLOT_ARM_R][0].create(Rect_LU(slot_img[SLOT_BODY][0].rect.max.x+s, slot_img[SLOT_BODY][0].rect.max.y  , w*2, w*3),slot).desc("Right Arm");

      // item images
      REPA(slot_img)if(i!=SLOT_TEMP)
      {
         window+=slot_img[i][1].create(slot_img[i][0].rect).desc(slot_img[i][0].desc()); slot_img[i][1].rect_color.zero();
      }
   }

   // region
   {
      Rect_LU rect(0, 0, window.crect.w(), window.crect.h());
              rect.max.y=rect.min.y+0.7f;
              rect.extend(-0.05f);
      window+=region.create(rect);
   }

   // list
   {
      ListColumn lc[]=
      {
         ListColumn(MEMBER(Item, icon ), 0.2f, L"Icon" ), // 0
         ListColumn(MEMBER(Item, name ), 0.5f, L"Name" ), // 1
         ListColumn(MEMBER(Item, power), 0.2f, L"Power"), // 2
      };
      lc[2].precision=1; // set showing only 1 decimal precision for power float attribute
      region+=list.create(lc, Elms(lc));

      list. cur_mode=LCM_MOUSE;
      list.draw_mode=LDM_RECTS;
   }
}
/******************************************************************************/
// OPERATIONS
/******************************************************************************/
void InventoryGui::link(Inventory *inv)
{
   if(T.inv!=inv)
   {
      T.inv=inv;
      if(inv)inv->inv_gui=this;

      setGui();
   }
}
/******************************************************************************/
void InventoryGui::setGui()
{
   if(inv)
   {
      // set item list
      {
         // here we have to hide the items which are assigned to slots
         // create a 'visible' array which determines visibility of an element (true=visible, false=hidden)
         Mems<Bool> visible; visible.setNum(inv->items.elms());
         REPAO(visible)=true; // set default visibility to enabled
         REPA(inv->slot) // iterate through all slots
         {
            if(inv->slot[i].valid()) // if the slot is valid
            {
               Int index=inv->items.validIndex(&inv->slot[i]()); // get index of a slot item in items container
               if(InRange(index, visible)) // if its valid
                  visible[index]=false; // set visibility for item assigned to a slot to false, to be hidden on the list
            }
         }
         list.setData(inv->items, visible.data()); // set list data from items container and visibility list
      }

      // set slot images
      REP(SLOT_NUM) // for all slots
         if(i!=SLOT_TEMP) // skip temporary slot because it's not drawn using 'Image' class
      {
         GuiImage        &img_back=     slot_img[i][0], // background image, we use it for accessing its rectangle
                         &img_item=     slot_img[i][1]; // item       image
         Reference<Item> &    item=inv->slot    [i]   ; // item at slot

         if(!item.valid())img_item.set(NULL);else // if there is no item then clear the item image slot
         {
            ImagePtr icon=item().icon; // access item's icon
            img_item.set(icon);        // set slot image as the item's icon
            if(icon)                   // set proper scaling
            {
               Vec2 size(icon->x(), icon->y()); size*=PIXEL_SIZE; // set default size
               if(size.x>img_back.rect.w())size*=img_back.rect.w()/size.x; // clamp item size to background slot width
               if(size.y>img_back.rect.h())size*=img_back.rect.h()/size.y; // clamp item size to background slot height
               Rect rect(img_back.rect.center()); rect.extend(size/2);
               img_item.setRect(rect);
            }
         }
      }
   }else
   {
      list.clear();
      REP(SLOT_NUM)slot_img[i][1].set(NULL);
   }
}
/******************************************************************************/
// UPDATE
/******************************************************************************/
void InventoryGui::update(Game::Chr &owner)
{
   if(inv)
   {
      if(Ms.bp(0)) // if mouse button pressed
      {
         if(inv->slot[SLOT_TEMP].valid()) // if we have an item attached with mouse
         {
            if(Gui.ms()==&list) // if mouse cursor is on the list
            {
               inv->slot[SLOT_TEMP].clear(); // clear the slot reference which will result in "putting back the item into the list"
               setGui();                     // update visuals
            }else
            if(Gui.ms()==Gui.desktop() || !Gui.ms()) // if mouse cursor is on the desktop or nothing (this means the game world area)
            {
               owner.itemDropDown(inv->slot[SLOT_TEMP]()); // drop the item onto the world
            }
         }
         else // we don't have an item so we want to get one
         {
            if(Gui.ms()==&list) // from the list
            {
               if(Item *item=list())
               {
                  inv->slot[SLOT_TEMP]=item;
                  setGui();
               }
            }
         }

         REP(SLOT_NUM)
            if(Gui.ms()==&slot_img[i][0] || Gui.ms()==&slot_img[i][1]) // if we want to put the item onto the i-th slot
               if(inv->slotsCanBeSwapped(SLOT_TEMP, i))
               {
                  Swap(inv->slot[SLOT_TEMP], inv->slot[i]); // swap temporary with i-th slot
                  setGui();                                 // update visuals
               }
      }
   }
}
/******************************************************************************/
// DRAW
/******************************************************************************/
void InventoryGui::draw()
{
   if(inv && inv->slot[SLOT_TEMP].valid())inv->slot[SLOT_TEMP]().drawIcon(Ms.pos());
}
/******************************************************************************/
