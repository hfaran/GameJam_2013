/******************************************************************************/
#include "stdafx.h"
/******************************************************************************

   To be able to see Unicode texts, you need to first create a font containing the desired characters

   In this tutorial we'll create a one font containing all the characters,
      but for final applications you may consider creating separate fonts with different encodings.

/******************************************************************************/
// standard ANSI characters
CChar8 *ansi_characters="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!@#$%^&*()[]<>{}`~_-+=;:,.?/|\\'\"";

// Hello in different languages
CChar *hello_russian =L"привет";
CChar *hello_japanese=L"こんにちは";
CChar *hello_chinese =L"喂";
CChar *hello_korean  =L"안녕하세요";
CChar *hello_greek   =L"γεια σου";
CChar *hello_arabic  =L"ابحرم";
/******************************************************************************/
void InitPre()
{
   App.name("Font Make Unicode");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
void CreateFont()
{
   // first we need to create a set of all characters we want to include in the font
   // so we'll simply use the characters that will be displayed later
   Str characters=S+ ansi_characters
                   +hello_russian
                   +hello_japanese
                   +hello_chinese
                   +hello_korean
                   +hello_greek
                   +hello_arabic;

                  Image            temp;
                  FontCreateParams params;
                  FontMake           (temp, S, characters, 64       ); // create an image of desired characters (64 pixel size)
   if(FontDefault)FontDefault->create(temp, characters, true, params); // use the engine's default font (FontDefault) and create it from the saved image
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   CreateFont(); // create the font

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
   D.text (0,  0.3f, "Hello");
   D.text (0,  0.2f, hello_russian );
   D.text (0,  0.1f, hello_japanese);
   D.text (0,  0.0f, hello_chinese );
   D.text (0, -0.1f, hello_korean  );
   D.text (0, -0.2f, hello_greek   );
   D.text (0, -0.3f, hello_arabic  );
}
/******************************************************************************/
