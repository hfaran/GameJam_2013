/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
// Define extended version of default 'Button'
/******************************************************************************/
STRUCT(Button2 , Button)
//{
   virtual void update(C GuiPC &gpc);
   virtual void draw  (C GuiPC &gpc);
};
void Button2::update(C GuiPC &gpc)
{
   super::update(gpc);
}
void Button2::draw(C GuiPC &gpc)
{
   if(!gpc.hidden && !hidden())
   {
      D.clip(gpc.clip);

      Color  color=(T() ? BLACK : ColorLerp(ORANGE, RED, lit()));
      D.rect(color, rect+gpc.offset);

      D.text(rect.center()+gpc.offset, text);
   }
}
/******************************************************************************/
GuiObjs gui_objs;
Button2 button;
/******************************************************************************/
void InitPre()
{
   App.name("Extending and Loading Gui Objects");
   App.flag=APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");
   D.mode(800,600);
}
/******************************************************************************/
Bool Init()
{
   Gui.kb_lit.zero();

   // replace default 'Button' class with extended 'Button2'
   gui_objs.replaceButton<Button2>();

   // load the gui objects
   if(gui_objs.load("gui/obj/player name.gobj"))
   {
      Gui+=gui_objs;
      return true;
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
   D  .clear(BLACK);
   Gui.draw ();
}
/******************************************************************************/
