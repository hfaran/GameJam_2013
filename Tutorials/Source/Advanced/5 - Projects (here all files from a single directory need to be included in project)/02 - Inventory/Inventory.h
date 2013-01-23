/******************************************************************************/
enum SLOTS
{
   SLOT_TEMP , // temporary slot (current item moved with mouse cursor)
   SLOT_HEAD , // head      slot
   SLOT_ARM_L, // left  arm slot
   SLOT_ARM_R, // right arm slot
   SLOT_BODY , // body      slot
   SLOT_NUM  , // number of slots
};
/******************************************************************************/
struct Inventory
{
   Memx     <Item> items         ; // all character items are stored in this container
   Reference<Item> slot[SLOT_NUM]; // these are references to items which are placed at certain slots
   InventoryGui   *inv_gui       ; // pointer to InventoryGui linked with Inventory

   // get
   Bool slotCanBePutTo   (Int src, Int dest); // test if slot 'src' can be put to 'dest' slot , 'src dest' are slot indexes (SLOTS)
   Bool slotsCanBeSwapped(Int a  , Int b   ); // test if slot 'a' can be swapped with slot 'b', 'a b'      are slot indexes (SLOTS)

   // operations
   void itemRemoved(Game::Obj &item); // called when an item is being removed
   void itemRemoved(               ); // called when an item has been removed
   void itemAdded  (Game::Obj &item); // called when an item has been added
   void setGui     (               ); // update visual gui components

   // update
   void update(Game::Chr &owner); // handle moving items with mouse and setting equipped item matrixes

   // draw
   void drawPrepare(); // draw equipped items in 3d world
   void drawShadow (); // draw equipped items in 3d world

   // io
   void save(File &f);
   Bool load(File &f);

  ~Inventory();
   Inventory() {inv_gui=NULL;}
};
/******************************************************************************/
