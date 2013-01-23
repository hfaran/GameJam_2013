/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
ColorPicker cp;
/******************************************************************************/
void InitPre()
{
   App.name("Color Picker");
   App.flag=APP_RESIZABLE|APP_FULL_TOGGLE|APP_MAXIMIZABLE;

   D.mode(800, 600);
   
   Paks.add("../data/engine.pak");
}
Bool Init()
{
   cp.create("Background").setRGB(Vec(0,1,0));
   return true;
}
void Shut()
{
}
/******************************************************************************/
Bool Update()
{
   if(Kb.bp(KB_ESC))return false;
   Gui.update();
   return true;
}
/******************************************************************************/
void Draw()
{
   D  .clear(cp());
   Gui.draw();
}
/******************************************************************************/
