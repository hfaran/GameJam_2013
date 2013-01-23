/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Memc<Str> FileNames;
/******************************************************************************/
void Drop(Memc<Str> &names, GuiObj *obj, C Vec2 &screen_pos)
{
   FileNames=names; // copy 'names' to global variable 'FileNames'
}
/******************************************************************************/
void InitPre()
{
   App.name("Drag & Drop");
   App.drop=Drop; // enable Drag & Drop by specifying a custom function which will be called when a file is dropped on the window
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;
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
   D.text (0, 0.9f, "Drag & Drop some files to this window");

   FREPA(FileNames)D.text(0, 0.7f-i*0.1f, FileNames[i]); // draw all file names
}
/******************************************************************************/
