/******************************************************************************/
#include "../../../../../data/enum/_enums.h"
#include "Player.h"
#include "Teleport.h"
#include "SaveGame.h"
/******************************************************************************/
// Define viewing modes:
enum VIEW_MODE // Viewing Mode
{
   VIEW_FPP, // First Person
   VIEW_TPP, // Third Person
   VIEW_NUM, // number of view modes
};
extern UInt View; // current VIEW_MODE
/******************************************************************************/
extern Game::ObjMemx<Player> Players;
/******************************************************************************/
