/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Int    value   ; // simple integer value
Button button_a, // gui button
       button_b; // gui button
/******************************************************************************/
void InitPre()
{
   App.name("Buttons");
   Paks.add("../data/engine.pak");
   D.mode(800,600);
}
/******************************************************************************/
void Func(Ptr) // custom function which will be called when button pushed
{
   value++;
}
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   Gui+=button_a.create(Rect_C(-0.3f, 0, 0.45f, 0.08f), "Button Func").func(Func); // create button which will call 'Func' function when pushed
   Gui+=button_b.create(Rect_C( 0.3f, 0, 0.45f, 0.08f), "Button Stay"           ); button_b.mode=BUTTON_STAY; // create button and set a special "stay" mode

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

   if(button_b())value++; // manually check if button is enabled

   return true;
}
/******************************************************************************/
void Draw()
{
   D  .clear(WHITE);
   Gui.draw ();

   D.text(0, 0.6f, S+"value = "     +value     ); // draw 'value' on screen
   D.text(0, 0.5f, S+"button_b() = "+button_b()); // draw state of 'button_b'
}
/******************************************************************************/
