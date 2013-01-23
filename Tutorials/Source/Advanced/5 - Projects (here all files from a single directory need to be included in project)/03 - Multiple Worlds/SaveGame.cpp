/******************************************************************************/
#include "stdafx.h"
#include "main.h"
/******************************************************************************/
SaveGame SG;
/******************************************************************************/
SaveGame::WorldState* SaveGame::findWorldState(Str world_dir)
{
   REPA(world_states)
      if(EqualPath(world_states[i].world_dir, world_dir))
         return &world_states[i];

   return NULL;
}
/******************************************************************************/
void SaveGame::saveCurrentWorld()
{
   Str world_dir=Game::World.worldDir();
   if( world_dir.is()) // if current world is a valid world
   {
      WorldState *ws=findWorldState(world_dir);
      if(!ws) // if specified world has not yet been stored in the save game
      {
         ws=&world_states.New();  // create it
         ws->world_dir=world_dir; // set its name
      }
      ws->world_state.writeMem(); // set the file for writing in memory
      Game::World.save(ws->world_state); // save current world state into the file object
   }
}
void SaveGame::changeWorld(Str world_dir)
{
   // save player data to a temporary file
   File player_data; player_data.writeMem();
   if(Players.elms())
   {
      Players[0].save(player_data);
      // after saving the player, remove it from the object container so that it will not be included in the old world state
      // without this there would be 2 copies of the same player, one in the old world and one in the new world
      Players.removeValid(0);
   }

   // save old world state
   saveCurrentWorld();

   // load new world
   if(WorldState *ws=findWorldState(world_dir)) // if world state of 'world_dir' is already stored in the save game
   {
      ws->world_state.pos(0); // reset file position
      Game::World.load(ws->world_state); // load world from last saved state
   }
   else // if the 'world_dir' was not yet stored in save game
   {
      Game::World.New(world_dir);
   }

   // restore the player in the new world
   if(player_data.size()) // if the file contains any data
   {
      player_data.pos(0); // reset file position to the start
      Game::World.objInject(OBJ_PLAYER, player_data, NULL); // specify NULL to don't change player position after loading
   }
}
/******************************************************************************/
