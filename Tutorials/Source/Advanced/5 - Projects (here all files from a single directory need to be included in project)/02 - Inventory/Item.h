/******************************************************************************/
STRUCT(Item , Game::Item) // extend items to include item Icons, and parameters
//{
   Char     name[32];
   Byte     type;
   Flt      power;
   ImagePtr icon;

   // manage
   void create(Game::ObjParams &obj);

   // draw
   UInt drawPrepare();
   void drawIcon   (C Vec2 &pos);

   // io
   void save(File &f);
   Bool load(File &f);

   Item();
};
/******************************************************************************/
