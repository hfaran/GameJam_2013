/******************************************************************************/
#include "stdafx.h"
/******************************************************************************

   Image Mask can be used for example for drawing pregenerated world mini maps
      onto the circular mini map gui element on the screen.

/******************************************************************************/
Image logo;
/******************************************************************************/
void InitPre()
{
   App.name("Image Mask");
   App.flag=APP_RESIZABLE|APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");
}
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;
   logo.Import("logo+alpha.png", -1, IMAGE_2D, 0);
   return true;
}
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
   D   .clear(WHITE);
   logo.drawMask(WHITE, Color(0,0,0,0), Rect_C(0,0,2,1), *Images("gfx/alpha.gfx"), Rect_C(Ms.pos(),1,0.75f));
   D   .text(0, 0.9f, "Please move the mouse around the screen");
}
/******************************************************************************/
