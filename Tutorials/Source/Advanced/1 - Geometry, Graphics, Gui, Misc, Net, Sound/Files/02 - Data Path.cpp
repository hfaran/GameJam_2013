/******************************************************************************/
#include "stdafx.h"
/******************************************************************************

   To work easily with relative paths - 'DataPath' is used
   when DataPath is set it is treated as additional path for your files/resources

   For Example you have all your data stored in 'd:/game/data', like:
      "d:/game/data/gfx/.."
      "d:/game/data/anim/.."
      "d:/game/data/obj/.."

   and you'd like to access them more simply:
      "gfx/.."
      "anim/.."
      "obj/.."

   To do so, all you have to do is set DataPath to "d:/game/data"

/******************************************************************************/
Image *image;
/******************************************************************************/
void InitPre()
{
   App.name("Data Path");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   // in this sample we'll load "image.gfx" which is located in "LocalData" folder where the tutorials are ("LocalData/image.gfx")
   DataPath("LocalData");     // setup additional Search Path to "LocalData"
   image=Images("image.gfx"); // load image, DataPath will be used automatically

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
   image->draw(Rect(-0.5f, -0.5f, 0.5f, 0.5f));
}
/******************************************************************************/
