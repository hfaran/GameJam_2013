/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
void InitPre()
{
   App.name("Colors HSB");
   Paks.add("../data/engine.pak");
   D.mode(800,600);
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
   D.clear(WHITE);

   REPD(x, 1024)
   {
      Flt   s0= x   /1024.0f           , // calculate step0 (fraction)
            s1=(x+1)/1024.0f           , // calculate step1 (fraction)
            x0=-1+s0*2                 , // calculate x0 position
            x1=-1+s1*2                 ; // calculate x1 position
      Color c0=ColorHue(s0            ), // calculate color0 from hue value
            c1=ColorHSB(s0, 1, 1      ), // calculate color1 from hue saturation brightness value
            c2=ColorHue(c1, Ms.pos().x); // calculate color2 from color1 modified by hue from mouse x position

      // draw rectangles with different colors
      D.rect(c0, x0,  0.1f, x1,  0.2f);
      D.rect(c1, x0, -0.1f, x1,  0.0f);
      D.rect(c2, x0, -0.3f, x1, -0.2f);
   }
}
/******************************************************************************/
