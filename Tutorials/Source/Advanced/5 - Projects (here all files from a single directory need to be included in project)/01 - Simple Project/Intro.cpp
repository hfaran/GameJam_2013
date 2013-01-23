/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
Bool InitIntro()
{
   return true;
}
void ShutIntro()
{
}
/******************************************************************************/
Bool UpdateIntro()
{
   if(StateActive->time()>3 || Kb.bp(KB_ESC))
      StateMenu.set(1.0);
   return true;
}
void DrawIntro()
{
   D.clear(BLACK);
   D.text (0, 0, "Intro");
}
/******************************************************************************/
State StateIntro(UpdateIntro,DrawIntro,InitIntro,ShutIntro);
/******************************************************************************/
