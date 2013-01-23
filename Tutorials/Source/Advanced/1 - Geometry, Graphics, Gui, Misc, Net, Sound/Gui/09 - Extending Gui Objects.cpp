/******************************************************************************/
#include "stdafx.h"
/******************************************************************************

   Because the main methods of Gui Objects are virtual, you can extend them.

/******************************************************************************/
STRUCT(Button2 , Button) // Create a new class extending the default Gui Button
//{
   virtual void update(C GuiPC &gpc); // extend updating object
   virtual void draw  (C GuiPC &gpc); // extend drawing  object
};

void Button2::update(C GuiPC &gpc) // extend updating
{
   super::update(gpc); // call default method
}

void Button2::draw(C GuiPC &gpc) // extend drawing
{
   if(!gpc.hidden && !hidden()) // if parent is not hidden and button isn't hidden too
   {
      D.clip(gpc.clip); // clip display drawing to given clipping rectangle, this is needed for example when object is inside a window, and is partially hidden because the parent's rectangle doesn't fully cover this object

      Color  color=(T() ? BLACK : ColorLerp(ORANGE, RED, lit())); // set different color depending if the button is pushed and highlighted
      D.rect(color, rect+gpc.offset); // draw a background rectangle, moved by 'gpc.offset' (parents offset)

      D.text(rect.center()+gpc.offset, text); // draw button's text, at center of rectangle and again moved by 'gpc.offset'
   }
}
/******************************************************************************/
Button2 button; // create a sample button
/******************************************************************************/
void InitPre()
{
   App.name("Extending Gui Objects");
   App.flag=APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");
   D.mode(800,600);
}
/******************************************************************************/
Bool Init()
{
   Gui+=button.create(Rect_C(0, 0, 0.5f, 0.1f), "New Button"); // add the button to the active desktop
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
