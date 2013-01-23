/******************************************************************************/
#include "stdafx.h"
/******************************************************************************

   Pak is an archive file type, it contains multiple files inside.
   Once you load a Pak you can access all the files inside it.

/******************************************************************************/
Image *image;
/******************************************************************************/
void InitPre()
{
   App.name("Paks");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   // 'sample.pak' used in this tutorial has only one folder 'folder' and one file 'image.gfx' located in 'folder' like this:
   // "folder/image.gfx"

   // add the Pak file to the global database
   Paks.add("LocalData/sample.pak");

   // from now on any data stored in loaded Pak can be accessed through IO methods
   {
      // you can open files stored in that Pak:
      {
         File f;
         f.read   ("folder/image.gfx"); // start reading file
         f.getByte(                  ); // get Byte from that file
      }
      // you can access resources through cache:
      {
         image=Images("folder/image.gfx");
      }
   }

   // Paks can be easily made using "Converter.exe" tool located in "Tools" folder
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
