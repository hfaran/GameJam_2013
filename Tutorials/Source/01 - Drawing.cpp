/******************************************************************************/
#include "stdafx.h"
/******************************************************************************

   Here are presented the most basic drawing functions

/******************************************************************************/
void InitPre()
{
   App.name("Drawing");
   Paks.add("_Assets/engine.pak");
}
/******************************************************************************/
Bool Init()
{
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
   D.clear(WHITE); // clear to white
   
   // draw texts
   {   
      D.text(0, 0, "Default text settings"); // default text draw

      TextStyle ts; // text style, contains information about font size, aligning, color, ..

      ts.color=BLACK; // set black color
      ts.scale*=1.4f; // change scale
      D.text(ts, 0, -0.2f, "Bigger");
      
      ts.scale/=1.4f;    // reset scale
      ts.align.set(1,0); // change aligning
      D.text(ts, 0, -0.4f, "Right Aligned");

      ts.color=GREEN;     // change color to green
      ts.align.set(-1,0); // change aligning
      D.text(ts, 0, -0.6f, "Colored Left Aligned");

      ts.align.set(0,0);                // reset aligning
      ts.color=ColorHue(Time.time()/3); // assign color to Color Hue according to current time
      D.text(ts, 0, -0.8f, "Multi colored");
   }

   // draw shapes
   {
      Rect  (0.5f, 0.5f, 0.6f, 0.6f ).draw(BLUE ); // draw blue rectangle at given coordinates
      Circle(0.1f, Vec2(-0.5f, 0.5f)).draw(RED  ); // draw red circle with 0.1 radius at (-0.5,0.5) position
      Vec2  (0.0, 0.5f              ).draw(BLACK); // draw black dot at (0,0.5) position
      Edge2 (-0.4f, 0.5f, 0.4f, 0.6f).draw(GREEN); // draw green line
      Tri2  (Vec2(-0.5f, -0.4f), Vec2(-0.4f, -0.2f), Vec2(-0.4f, -0.4f)).draw(YELLOW); // draw yellow triangle from given points
   }
}
/******************************************************************************/
