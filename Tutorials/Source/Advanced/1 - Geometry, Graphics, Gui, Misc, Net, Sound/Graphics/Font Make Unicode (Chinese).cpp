/******************************************************************************/
#include "stdafx.h"
/******************************************************************************

   Here is presented the functionality to create fonts with huge amounts of characters.

   Creating huge amounts of characters can take up to a minute.

/******************************************************************************/
// standard ANSI characters
CChar8 *ansi_characters="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!@#$%^&*()[]<>{}`~_-+=;:,.?/|\\'\"";

Str characters;

Char temp[32];
/******************************************************************************/
void InitPre()
{
   App.name("Font Make Unicode");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
void CreateFont()
{
   characters=ansi_characters;

   // include chinese characters
   characters+=LangSpecific(CN);

   // create the font
   Image            temp;
   FontCreateParams params;
   FontMake(temp, S, characters, 32);
   if(FontDefault)
   {
      FontDefault->create(temp, characters, true, params); // create the font from 1 row image
    //FontDefault->save  ("LocalData/ChineseFont.font"  ); // save the created font for future usage
   }
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
   Time.wait(100); // slow down the tutorial so the text will not flicker
   return true;
}
/******************************************************************************/
void Draw()
{
   D.clear(WHITE);
   
   Set(temp, characters()+Random(characters.length())); // set a random part of the character set
   D.text(0, 0, temp);
}
/******************************************************************************/
