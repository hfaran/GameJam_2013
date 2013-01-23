/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
enum COL_TRANS_MODE // color transformation modes
{
   CT_RGB,
   CT_RBG,
   CT_GRB,
   CT_GBR,
   CT_BRG,
   CT_BGR,
   CT_MONO,
   CT_HUE,
   CT_HUE2,
   CT_CUSTOM,
   CT_CUSTOM2,
};
CChar8 *CTText[]= // color transformation description
{
   "RGB",
   "RBG",
   "GRB",
   "GBR",
   "BRG",
   "BGR",
   "MONO",
   "HUE",
   "HUE+",
   "CUSTOM",
   "CUSTOM2",
};
Image  image ; // Esenthel image
Tabs   tabs  ; // transformation tabs
Slider slider; // transformation power/alpha
/******************************************************************************/
void InitPre()
{
   App.name("Color Transformations");
   App.flag=APP_MAXIMIZABLE;
   Paks.add("../data/engine.pak");
   D.mode(800,600);
}
/******************************************************************************/
Bool Init()
{
   image.Import("../data/logo.png", -1, IMAGE_2D, 1);
   Gui+=tabs  .create(Rect_U (0, D.h(), D.w()*2, 0.06f), 0, CTText, Elms(CTText));
   Gui+=slider.create(Rect_LU(-D.w(), D.h()-0.1f, 0.4f, 0.08f)).set(1);
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
void ApplyColorTransformation(Int mode)
{
   ColorMatrix cm;             // color transformation matrix
   Flt         alpha=slider(); // color transformation power

   switch(mode)
   {
      case CT_RGB : cm.setRGB   (             ).draw(alpha); break;
      case CT_RBG : cm.setRBG   (             ).draw(alpha); break;
      case CT_GRB : cm.setGRB   (             ).draw(alpha); break;
      case CT_GBR : cm.setGBR   (             ).draw(alpha); break;
      case CT_BRG : cm.setBRG   (             ).draw(alpha); break;
      case CT_BGR : cm.setBGR   (             ).draw(alpha); break;
      case CT_MONO: cm.setMono  (             ).draw(alpha); break;
      case CT_HUE : cm.setHue   (Time.time()/4).draw(alpha); break;
      case CT_HUE2: ColorTransHB(Time.time()/4, 1,   alpha); break; // use 'ColTransHB' to get correct Hue-Brightness transformation

      case CT_CUSTOM:
      {
         // set custom matrix

         //         dest.r, dest.g, dest.b
         cm.x  .set(1     , 1     , 1     ); // src.r
         cm.y  .set(0     , 0     , 0     ); // src.g
         cm.z  .set(0     , 0     , 0     ); // src.b

         //         add.r , add.g , add.b
         cm.pos.set(0     , 0.2f  , 0     ); // 1

         cm.draw(alpha);
      }break;

      case CT_CUSTOM2:
      {
         // set custom matrix #2

         //         dest.r, dest.g, dest.b
         cm.x  .set(0.5f  , 0.5f  , 0.5f  ); // src.r
         cm.y  .set(0.5f  , 0.5f  , 0.5f  ); // src.g
         cm.z  .set(0.3f  , 0.3f  , 1.0f  ); // src.b

         //         add.r , add.g , add.b
         cm.pos.set(0     , 0     , 0     ); // 1

         cm.draw(alpha);
      }break;
   }
}
void Draw()
{
   D.clear();

   // draw
   image.drawFs();
   Rect(-D.w()/2, -0.7f, D.w()/2, -0.75f).drawShadedX(BLACK, RED  );
   Rect(-D.w()/2, -0.8f, D.w()/2, -0.85f).drawShadedX(BLACK, GREEN);
   Rect(-D.w()/2, -0.9f, D.w()/2, -0.95f).drawShadedX(BLACK, BLUE );

   // transform colors
   ApplyColorTransformation(tabs());

   // draw gui after transformation
   Gui.draw();
}
/******************************************************************************/
