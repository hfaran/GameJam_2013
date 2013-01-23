/******************************************************************************/
STRUCT(Teleport , Game::ObjParticles)
//{
   Str target_world;

   virtual void create(Game::ObjParams &obj);

   virtual void save(File &f);
   virtual Bool load(File &f);
};
/******************************************************************************/
