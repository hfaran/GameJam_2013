/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Window   window; // gui window
Tabs     tabs  ; // gui tabs
Text     text  ; // gui text
Button   button; // gui button
GuiImage image ; // gui image
/******************************************************************************/
void InitPre()
{
   App.name("Tabs");
   Paks.add("../data/engine.pak");
   D.mode(800,600);
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   CChar8 *tab[]=
   {
      "First",
      "Second",
      "Third",
   };

   Gui   +=window.create(Rect   (-0.5f, -0.4f, 0.5f, 0.4f), "Window with tabs");
   window+=tabs  .create(Rect_LU(0, 0, window.crect.w(), 0.08f), 0, tab, Elms(tab)); // create tabs

   // add some elements to tabs
   {
      tabs.tab(0)+=text  .create(Vec2  (window.crect.w()/2, -window.crect.h()/2            ), "Text"                                  ); // create and add text   to 0th tab
      tabs.tab(1)+=button.create(Rect_C(window.crect.w()/2, -window.crect.h()/2, 0.4f, 0.1f), "Button"                                ); // create and add button to 1st tab
      tabs.tab(2)+=image .create(Rect_C(window.crect.w()/2, -window.crect.h()/2, 0.5f, 0.5f), Images.ptrRequire("LocalData/image.gfx")); // create and add image  to 2nd tab
   }

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
}
/******************************************************************************/
