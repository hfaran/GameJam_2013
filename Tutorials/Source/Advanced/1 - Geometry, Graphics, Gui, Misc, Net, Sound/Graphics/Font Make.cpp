/******************************************************************************/
#include "stdafx.h"
/******************************************************************************

   Creating Font is split into few parts:
      1. Call 'FontMake' to generate an image of all desired characters
      2. Optionally you can save the image on disk, customize it and load before the next step
      3. Use 'Font::create' method to create a font from the image

   Once you have created a Font, you can save it using 'Font::save',
      and then in the game load it using 'Font::load' or by using Font cache - 'Fonts'

   Loading already created font ('Font::load' or 'Fonts') is much faster than creating a font ('Font::create')

/******************************************************************************/
Font   *font  ; // new font
Button  button;
/******************************************************************************/
void InitPre()
{
   App.name("Font Make");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
void CreateFont()
{
   CChar *characters=L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!@#$%^&*()[]<>{}`~_-+=;:,.?/|\\'\""; // these are all the basic characters you may want to include in your font
   // please note that you don't have to worry about accidentally including the same character multiple times, because the engine will only generate 1 copy of a single character
   // for example you can freely set 'characters' to "0123ABC01" and the engine will generate only "0123ABC"

   Image    temp;
   FontMake(temp, "Times New Roman", characters, 64); // create a image of desired characters created from the font "Times New Roman" located in your "Windows/Fonts" folder (64 pixel size)

   // here you may optionally perform some modifications to the image, for example edit the image in external programs

   // after the image is ready, create the final font from it
   FontCreateParams params;
   Font font;
   if(font.create(temp, characters, true, params  )) // create font from the image and use exactly the same character set 'characters' which were used in the 'FontMake' function
      font.save  ("LocalData/Times New Roman.font"); // save it, to load it later in your application
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   CreateFont(); // create the new font and save it to disk

   font=Fonts("LocalData/Times New Roman.font"); // load the font

   // now let's change Gui's default font for buttons
   Gui.ts_button.font=font;                                    // assign the new font
   Gui+=button.create(Rect_C(0, -0.3f, 0.4f, 0.1f), "Button"); // create a sample button to view the changes

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
void Draw()
{
   D  .clear(WHITE);
   Gui.draw ();

   TextStyle ts;
   ts.font  =font ; // set font parameter to the newly created font
   ts.color =BLACK; // set black color
   ts.shadow=    0; // set no shadow

   D.text(    0, 0.2f, "This is the default font!"); // draw some text with the default       font
   D.text(ts, 0, 0   , "This is the new font!"    ); // draw some text with the newly created font, please note the first parameter 'ts' is used to specify the new settings
}
/******************************************************************************/
