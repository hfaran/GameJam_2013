/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
TextLine textline; // Gui TextLine
/******************************************************************************/
void InitPre()
{
   App.name("Calculator");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   Gui+=textline.create(Rect(-0.5f, 0, 0.5f, 0.1f)); // add textline to gui desktop
   textline.kbSet();                                 // set keyboard focus

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
   Gui.update();
   return true;
}
/******************************************************************************/
void Draw()
{
   D  .clear(WHITE);
   Gui.draw ();
   D.text(0, -0.4f, S+CalculateR(textline())); // draw calculated value from 'textline' as Real
}
/******************************************************************************/
