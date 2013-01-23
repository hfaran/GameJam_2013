/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Image logo         , // logo   image
      fade_modifier; // fading texture
Flt   fx_time      ; // effect time
Int   fx_index     ; // effect index
/******************************************************************************/
void InitPre()
{
   App.name("2D Effects");
   App.flag=APP_FULL_TOGGLE;
   Paks.add("../data/engine.pak");
   D.mode(800,600);
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   // import esenthel engine logo
   logo.Import("../data/logo.png", -1, IMAGE_2D);

   // create fading texture
   fade_modifier.create2D(128, 128, IMAGE_A8, 1);
   if(fade_modifier.lock())
   {
      REPD(x, fade_modifier.x())
      REPD(y, fade_modifier.y())fade_modifier.pixel(x, y, Random(256));
      fade_modifier.unlock();
   }
   fade_modifier.blur(5, false); // apply gaussian blur with 5 pixel range and no clamping

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

   // change effect on key pressed, and reset effect time
   if(Kb.bp(KB_0)){fx_index=0; fx_time=0;}
   if(Kb.bp(KB_1)){fx_index=1; fx_time=0;}
   if(Kb.bp(KB_2)){fx_index=2; fx_time=0;}
   if(Kb.bp(KB_3)){fx_index=3; fx_time=0;}
   if(Kb.bp(KB_4)){fx_index=4; fx_time=0;}
   if(Kb.bp(KB_5)){fx_index=5; fx_time=0;}
   if(Kb.bp(KB_6)){fx_index=6; fx_time=0;}

   // increase effect time
   fx_time+=Time.d();

   return true;
}
/******************************************************************************/
void Draw()
{
   D.clear(WHITE);

   if(Kb.b(KB_SPACE))fade_modifier.drawFs();else // if    space pressed then draw fading texture to full screen
   switch(fx_index)                              // if no space pressed then draw effects
   {
      case 0:
      {
         D.text(0, 0, "Press 1,2,3,4,5,6 keys to view different effects");
      }break;

      case 1: // Draw Image with Ripple Effect
      {
         // set ripple effect values
         RippleFx ripple;
         ripple.alpha_add  = fx_time;
         ripple.alpha_scale=-1;

         // draw image with ripple effect
         ripple.draw(logo, &Rect_LU(-1,1,2,2));
      }break;

      case 2: // Draw Ripple from Helper Screen
      {
         // draw to helper screen
         D.fxBegin(true);            // start drawing to helper screen
         logo.draw(Rect_C(0,0,2,2)); // image draw
         C Image &fx=D.fxEnd();      // restore default screen drawing, and access the helper screen

         // set ripple effect values
         RippleFx ripple;
         ripple.alpha_add  = fx_time;
         ripple.alpha_scale=-1;

         // draw helper screen with ripple effect
         ripple.draw(fx);
      }break;

      case 3: // Draw Fade Effect
      {
         // draw to helper screen
         D.fxBegin(true);            // start drawing to helper screen
         logo.draw(Rect_C(0,0,2,2)); // image draw
         C Image &fx=D.fxEnd();      // restore default screen drawing, and access the helper screen

         // fade in helper screen with 'fade_modifier' fading texture
         FadeFx(fx, fx_time/2, &fade_modifier);
      }break;

      case 4: // Titles Effect
      {
         // draw to helper screen
         D.fxBegin(true);                                                       // start drawing to helper screen
         REP(30)D.text(0, D.h()+(Frac(Time.time())-i)*0.08f, "Text Text Text"); // draw text
         C Image &fx=D.fxEnd();                                                 // restore default screen drawing, and access the helper screen

         // set effect values
         TitlesFx titles;

         // draw helper screen with titles effect
         titles.draw(fx);
      }break;

      case 5: // Wave Effect
      {
         logo.drawFs();           // draw logo on full screen
         WaveFx(fx_time/2, 0.8f); // apply wave effect onto the full screen
      }break;

      case 6: // Radial Blur Effect
      {
         logo.drawFs();                    // draw logo on full screen
         RadialBlurFx(Frac(fx_time)*0.3f); // apply radial blur effect onto the full screen
      }break;
   }
}
/******************************************************************************/
