/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
void InitPre()
{
   App.name("Joypad");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color  =BLACK;
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
   return true;
}
/******************************************************************************/
void Draw()
{
   D.clear(WHITE);
   if(Joypads())
   {
      Str buttons; FREP(32)buttons+=Joypad[0].b(i); D.text(Rect_C(0, 0.8f, 0, 0), S+"Buttons:\n"+buttons);
      Rect_C( 0   , 0, 0.5f, 0.5f).draw(BLACK, false); (Joypad[0].dir     *0.25f              ).draw(RED);
      Rect_C( 0.6f, 0, 0.5f, 0.5f).draw(BLACK, false); (Joypad[0].dir_a[0]*0.25f+Vec2(-0.6f,0)).draw(RED);
      Rect_C(-0.6f, 0, 0.5f, 0.5f).draw(BLACK, false); (Joypad[0].dir_a[1]*0.25f+Vec2( 0.6f,0)).draw(RED);
   }else
   {
      D.text(0, 0, S+"No JoyPads detected");
   }
}
/******************************************************************************/
