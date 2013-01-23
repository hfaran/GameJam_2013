/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
void InitPre()
{
   App.name("Touch Screen");
   Paks.add("../data/engine.pak");

   SimulateTouches(true); // enable simulating the 'Touches' behavior using mouse for systems which don't support touch screens
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
   if(Kb.ctrl())Time.wait(80); // sleep so the screen won't update so frequently, and it's easier to notice the text
   return true;
}
/******************************************************************************/
void Draw()
{
   D.clear(ColorBrightness(WHITE, 0.8f));
   D.text (0, 0.9f, S+"Active Touches: "+Touches.elms()); // draw amount of active touches
   D.text (0, 0.8f, S+"Accelerometer: "+Accelerometer()); // draw accelerometer value

   Str s;
   FREPA(Touches) // draw all touches
   {
      Touch &t=Touches[i];
      Circle(0.04f, t.startPos()).draw(ColorAlpha(GREEN, 0.5f)); // draw initial position
      Circle(0.04f, t.pos     ()).draw(ColorAlpha(RED  , 0.5f)); // draw current position

   	// store information in text
   	s+=S+"Pos:"  +t.pos      ()+" Delta:"+t.d   ()+"\n";
      s+=S+"Start:"+t.startTime()+" Life:" +t.life()+"\n";
      if(t.on())s+="ON,";
      if(t.pd())s+="PD,";
      if(t.rs())s+="RS,";
      if(t.db())s+="DB,";
      s+="\n";
      if(t.tapped())s+="TAPPED";
      s+="\n";
   }
   D.text(Rect(-D.w(), -D.h(), D.w(), 0.7f), s);
   D.text(0, -0.9f, "Hold Control to draw the text slower");
}
/******************************************************************************/
