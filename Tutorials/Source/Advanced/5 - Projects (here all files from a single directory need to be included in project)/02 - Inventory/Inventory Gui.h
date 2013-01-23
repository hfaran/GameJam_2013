/******************************************************************************/
#define PIXEL_SIZE 0.002 
/******************************************************************************/
struct InventoryGui
{
   Inventory *inv   ; // Inventory which InventoryGui is linked with
   Window     window; // inventory window
   Region     region; // region in which list of items is displayed
   List<Item> list  ; // list of items
   GuiImage   slot_img[SLOT_NUM][2]; // slot images [slot index(SLOTS)][0=background, 1=item]

   // manage
   void create();

   // operations
   void   link(Inventory *inv); // link with 'inv' Inventory
   void unlink(              ) {link(NULL);}

   void setGui(); // set visual gui components

   // update
   void update(Game::Chr &owner); // handle moving items with mouse

   // draw
   void draw();

   InventoryGui() {inv=NULL;}
}extern
   InvGui;
/******************************************************************************/
