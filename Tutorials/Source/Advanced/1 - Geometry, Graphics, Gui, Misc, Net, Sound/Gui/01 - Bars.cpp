/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Window   window  ; // gui window
SlideBar slidebar; // gui slidebar
Progress progress; // gui progressbar
Slider   slider  ; // gui slider
/******************************************************************************/
void InitPre()
{
   App.name("Bars");
   Paks.add("../data/engine.pak");
   D.mode(800,600);
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   Gui   +=window  .create(Rect(-0.5f, -0.4f, 0.5f,  0.40f), "Window with bars");
   window+=slidebar.create(Rect( 0.1f, -0.2f, 0.9f, -0.12f)); // create slidebar
   window+=progress.create(Rect( 0.1f, -0.4f, 0.9f, -0.32f)); // create progressbar
   window+=slider  .create(Rect( 0.1f, -0.6f, 0.9f, -0.52f)); // create slider

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

   progress.set(slidebar()); // set progress bar according to slidebar value

   return true;
}
/******************************************************************************/
void Draw()
{
   D  .clear(WHITE);
   Gui.draw();
   D.text(0, -0.7f, S+"SlideBar "   +slidebar()); // draw slidebar    value
   D.text(0, -0.8f, S+"ProgressBar "+progress()); // draw progressbar value
   D.text(0, -0.9f, S+"Slider "     +slider  ()); // draw slider      value
}
/******************************************************************************/
