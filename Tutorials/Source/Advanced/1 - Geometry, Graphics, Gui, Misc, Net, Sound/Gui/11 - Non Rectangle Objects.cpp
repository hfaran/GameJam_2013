/******************************************************************************/
#include "stdafx.h"
/******************************************************************************

   Here we're going to present a method of using gui objects which are not rectangle shaped.
      We'll achieve this affect by overriding the virtual 'test' method which tests
      if specified position intersects with the object.

   It is important to note that any custom shape we want the object to have,
      it must be contained fully inside the regular rectangle,
      in other words - the custom shape must not exceed the size of the rectangle.

/******************************************************************************/
STRUCT(GuiCircle , GuiCustom) // Create a new class extending the default Gui Button
//{
   virtual GuiObj* test(C GuiPC &gpc, C Vec2 &pos, GuiObj* &mouse_wheel); // test if 'pos' screen position intersects with the object, by returning pointer to object or its children upon intersection and NULL in case no intersection, 'mouse_wheel' may be modified upon intersection either to the object or its children or NULL
   virtual void    draw(C GuiPC &gpc); // extend drawing
};

GuiObj* GuiCircle::test(C GuiPC &gpc, C Vec2 &pos, GuiObj* &mouse_wheel) // extend updating
{
   Circle circle(rect.size().min()*0.5f, rect.center()+gpc.offset); // specify screen space circle
   return Cuts(pos, circle) ? super::test(gpc, pos, mouse_wheel) : NULL; // proceed with testing only if the position intersects with the circle, in other case return NULL to specify that the position does not intersect with any object
}

void GuiCircle::draw(C GuiPC &gpc)
{
   if(!gpc.hidden && !hidden()) // if parent is not hidden and button isn't hidden too
   {
      D.clip(gpc.clip); // clip display drawing to given clipping rectangle, this is needed for example when object is inside a window, and is partially hidden because the parent's rectangle doesn't fully cover this object

      Circle circle(rect.size().min()*0.5f, rect.center()+gpc.offset); // specify screen space circle
      circle.draw(GREEN);

      (rect+gpc.offset).draw(RED, false); // draw the rectangle coverage just for debugging purposes
   }
}
/******************************************************************************/
GuiCircle circle; // create circle object
/******************************************************************************/
void InitPre()
{
   App.name("Non Rectangle Objects");
   App.flag=APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");
   D.mode(800,600);
}
/******************************************************************************/
Bool Init()
{
   Gui+=circle.create(Rect_C(0, 0, 0.5f, 0.5f));
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
   D.text(0, D.h()-0.1f, S+"Mouse Cursor on the Circle Object: "+(Gui.ms()==&circle));
}
/******************************************************************************/
