/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Memc<Str> names;
/******************************************************************************/
void InitPre()
{
   App.name("FileFind");
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
void AddName(C Str &name) // add 'name' to names list
{
   names.add(name); // create new element in names and set it's name
}

void ManualFind(Str path) // manually iterate through directories and files
{
   for(FileFind ff(path); ff();)switch(ff.type) // start looking for files in path, continue while active, and check for encountered type
   {
      case FSTD_DIR : ManualFind(ff.pathName()); break; // if directory encountered start looking inside it
      case FSTD_FILE: AddName   (ff.pathName()); break; // if file      encountered add it to the list
   }
}
/******************************************************************************/
Bool Update()
{
   if(Kb.bp(KB_ESC))return false;

   if(Kb.bp(KB_SPACE))
   {
      names.clear();           // clear elements
      FAll("source", AddName); // use FAll which operates on all files inside given path (AddName will be called multiple times with each file name as a parameter)
   }
   
   if(Kb.bp(KB_ENTER))
   {
      names.clear();        // clear elements
      ManualFind("source"); // manually iterate through all directories and files
   }

   return true;
}
/******************************************************************************/
void Draw()
{
   D.clear(WHITE);
   D.text (0, 0.9f, "Press Space to use FAll, or press Enter for FileFind");

   FREPA(names)D.text(0, 0.8f-i*0.1f, names[i]); // draw all file names
}
/******************************************************************************/
