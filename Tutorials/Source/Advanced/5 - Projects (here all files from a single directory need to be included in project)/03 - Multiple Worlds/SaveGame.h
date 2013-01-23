/******************************************************************************/
struct SaveGame
{
   struct WorldState
   {
      Str  world_dir  ;
      File world_state;
   };

   Memb<WorldState> world_states;

   // get / set
   WorldState* findWorldState(Str world_dir);

   // operations
   void saveCurrentWorld();
   void      changeWorld(Str world_dir);
}extern
   SG;
/******************************************************************************/
