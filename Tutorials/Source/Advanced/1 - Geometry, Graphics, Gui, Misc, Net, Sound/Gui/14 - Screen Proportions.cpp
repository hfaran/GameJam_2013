/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Image    logo;
GuiImage image[4];
Flt      screen_changed_time;
/******************************************************************************/
void ScreenChanged(Flt old_width, Flt old_height)
{
   screen_changed_time=1;
}
void InitPre()
{
   App.name("Understanding Changing Screen Proportions");
   App.flag=APP_RESIZABLE;
   App.x=0;
   App.y=0;
   DataPath("../Data");
   Paks.add("engine.pak");
   D.mode(800,600);
   D.screen_changed=ScreenChanged;
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   // import logo image
   logo.Import("logo.png", -1, IMAGE_2D, 0);

   // add gui image to 4 screen corners
   Gui+=image[0].create(Rect_LU(-D.w(),  D.h(), 0.6f, 0.3f), &logo);
   Gui+=image[1].create(Rect_RU( D.w(),  D.h(), 0.6f, 0.3f), &logo);
   Gui+=image[2].create(Rect_RD( D.w(), -D.h(), 0.6f, 0.3f), &logo);
   Gui+=image[3].create(Rect_LD(-D.w(), -D.h(), 0.6f, 0.3f), &logo);

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

   screen_changed_time-=Time.d();

   if(Ms.bp(0))D.mode(D.x()-32, D.y()); // decrease screen size on LMB
   if(Ms.bp(1))D.mode(D.x()+32, D.y()); // increase screen size on RMB

   return true;
}
/******************************************************************************/
void Draw()
{
   D  .clear(WHITE);
   Gui.draw ();

   // draw a black cross on the screen center
   D.lineX(BLACK, 0, -0.1f, 0.1f);
   D.lineY(BLACK, 0, -0.1f, 0.1f);

   // draw "screen changed" info
   TextStyle ts=DefaultTextStyle; ts.color=ColorAlpha(ts.color, screen_changed_time);
   D.text(ts, 0, 0.15f, "Screen Changed");

   // draw informations
   D.text(0,  0.9f, S+"Press LMB/RMB to change screen size");
   D.text(0,  0.8f, S+"Mouse Position: "+Ms.pos());
   D.text(0, -0.4f, S+"You can see that horizontal range of the screen changes ("+D.w()+')');
   D.text(0, -0.5f, S+"While vertical range of the screen remains constant ("    +D.h()+')');
   D.text(0, -0.6f, S+"Gui Objects on the desktop are set relatively to the desktop center.");
}
/******************************************************************************/
