/******************************************************************************/
STRUCT(Player , Game::Chr)
//{
   Inventory inv;

   Memx<Game::Obj>* itemContainer(               ) {Memx<Game::Obj> &items=inv.items; return &items;} // get item container, this is required by the engine to point to a valid item container for a character
   void             itemRemoved  (Game::Obj &item) {                       inv.itemRemoved(item)   ;} // this is called when an item is being removed from the character
   void             itemRemoved  (               ) {                       inv.itemRemoved(    )   ;} // this is called when an item has been removed from the character
   void             itemAdded    (Game::Obj &item) {                       inv.itemAdded  (item)   ;} // this is called when an item has been added   to   the character

   // update / draw
   Bool update     ();
   UInt drawPrepare();
   void drawShadow ();

   // io
   void save(File &f);
   Bool load(File &f);

   Player();
};
/******************************************************************************/
