/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
Bool InitGame()
{
   return true;
}
void ShutGame()
{
}
/******************************************************************************/
Bool UpdateGame()
{
   if(Kb.bp(KB_ESC))StateMenu.set(1.0f);
   return true;
}
void DrawGame()
{
   D.clear(TURQ);
   D.text (0, 0, "Game");
}
/******************************************************************************/
State StateGame(UpdateGame,DrawGame,InitGame,ShutGame);
/******************************************************************************/