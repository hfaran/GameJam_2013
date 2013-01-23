/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
Bool InitMenu()
{
   return true;
}
void ShutMenu()
{
}
/******************************************************************************/
Bool UpdateMenu()
{
   if(Kb.bp(KB_ESC))return false;
   if(Kb.bp(KB_ENTER))StateGame.set(0.5f);
   return true;
}
void DrawMenu()
{
   D.clear(GREY);
   D.text (0,  0   , "Menu");
   D.text (0, -0.3f, "Press Enter to start the game");
   D.text (0, -0.5f, "Press Escape to exit");
}
/******************************************************************************/
State StateMenu(UpdateMenu, DrawMenu, InitMenu, ShutMenu);
/******************************************************************************/
