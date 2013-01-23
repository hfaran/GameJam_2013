/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
void InitPre()
{
   App.name("Mouse Cursor");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;
   return true;
}
/******************************************************************************/
void Shut()
{
}
/******************************************************************************/
Bool Update()
{
   if(Kb.bp(KB_ESC))return false;

   if(Kb.bp(KB_1))Ms.cursor(Images("../data/gfx/cursor/0.gfx")); // if '1' pressed change cursor to '0.gfx'
   if(Kb.bp(KB_2))Ms.cursor(Images("../data/gfx/cursor/1.gfx")); // if '2' pressed change cursor to '1.gfx'

   return true;
}
/******************************************************************************/
void Draw()
{
   D.clear(WHITE);
   D.text (0, 0, "Press '1' or '2' to change mouse cursor");
}
/******************************************************************************/
