/******************************************************************************/
#include "stdafx.h"
/******************************************************************************

   FileText is meant for operations performed on a text file

/******************************************************************************/
struct Data // custom Data structure to test text file saving/loading
{
   Flt volume;
   Int graphics_mode;
   Str player_name;

   Bool saveTxt(Str name); // save data to   text file
   Bool loadTxt(Str name); // load data from text file
};
/******************************************************************************/
Bool Data::saveTxt(Str name)
{
   FileText f; // FileText object

   if(f.write(name)) // if file opened successfully
   {
      f.put    ("Volume       = ", volume       );
      f.put    ("GraphicsMode = ", graphics_mode); // please note "GraphicsMode" is used instead of "Graphics Mode" (names are required to don't have spaces)
      f.putName("PlayerName   = ", player_name  ); // FileText::putName will automatically put the value in quotes
      return true; // return success
   }
   return false; // return failure
}

Bool Data::loadTxt(Str name)
{
   // set default values to members in case file not found or file incomplete
   volume=1;
   graphics_mode=5;
   player_name.clear();

   FileText f;

   if(f.read(name)) // if file opened successfully
   {
      for(;f.level();) // process file within its level
      {
         if(f.cur("Volume"      ))volume       =Sat(f.getReal()       );else // if encountered "Volume"       then read a floating point value, saturate it and store into 'value'
         if(f.cur("GraphicsMode"))graphics_mode=Mid(f.getInt (), 0, 10);else // if encountered "GraphicsMode" then read a integer        value, clamp    it and store into 'graphics_mode'
         if(f.cur("PlayerName"  ))player_name  =    f.getName()        ;     // if encountered "PlayerName"   then read a string using 'FileText::getName' method     into 'player_name'
      }
      return true; // return success
   }
   return false; // return failure
}
/******************************************************************************/
void InitPre()
{
   App.name("FileText");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   // set sample data values
   Data data;
   data.volume       =0.5f;
   data.graphics_mode=2;
   data.player_name  ="He-Man";

   data.saveTxt("LocalData/file.txt"); // save data to   text file
   data.loadTxt("LocalData/file.txt"); // load data from text file

   OSLaunch("LocalData/file.txt"); // open the text file in default Windows viewer to check what actually has been saved

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
}
/******************************************************************************/
