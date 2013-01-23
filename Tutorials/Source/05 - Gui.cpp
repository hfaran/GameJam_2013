
#include "stdafx.h"

Window window; // gui window
Text   text  ; // gui text
Button button; // gui button

void InitPre()
{
   App.name("GUI");
   Paks.add("../data/engine.pak");
   D.mode(800,600);
}

Bool Init()
{
   Gui   +=window.create(Rect(-0.5f , -0.3f , 0.5f ,  0.2f), "Window Title"); // create window and add it to Gui desktop
   window+=text  .create(Vec2( 0.5f , -0.15f              ), "text"        ); // create text   and add it to 'window', coordinates are relative to parent (its top left corner)
   window+=button.create(Rect( 0.35f, -0.37f, 0.65f, -0.3f), "OK"          ); // create button and add it to 'window', coordinates are relative to parent (its top left corner)
   return true;
}

void Shut()
{
}

Bool Update()
{
   if(Kb.bp(KB_ESC))return false;

   Gui.update(); // update GUI

   return true;
}

void Draw()
{
   D  .clear(WHITE);
   Gui.draw (); // draw GUI
}

