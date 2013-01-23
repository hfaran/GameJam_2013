/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Byte b1, b2; // bytes
/******************************************************************************/
void InitPre()
{
   App.name("Pak Create");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   // create Pak from one file/directory
   {
      PakCreate("LocalData"); // create Pak from "LocalData" folder, if no 'pak_name' is specified then it will be automatically set (in this case it'll be "LocalData.pak")
   }

   // create Pak from list of files
   {
      Memc<Str> list;
      list.add("LocalData" ); // add "LocalData"  directory to the list
      list.add("stdafx.h"  ); // add "stdafx.h"   file      to the list
      list.add("stdafx.cpp"); // add "stdafx.cpp" file      to the list
      PakCreate(list);        // create Pak from list, 'pak_name' will be automatically set to files folder name, in this case "Tutorials.pak"
   }

   // create Pak from files in memory
   {
      Memb<File> file; // set of file's which we'll create and write to them

      Memb<PakNode> nodes; // lets start with main node
      {
         File &f=file.New();               // add new file to container
         f.writeMem();                     // start writing to memory
         f.putByte(123);                   // write a Byte to that file
         nodes.New().set("File 1.dat", f); // add this file to file nodes
      }
      {
         Memb<PakNode> &dir=nodes.New().set("Directory").children; // add new directory
         {
            File &f=file.New();             // add new file to container
            f.writeMem();                   // start writing to memory
            f.putByte(234);                 // write a Byte to that file
            dir.New().set("File 2.dat", f); // add this file to directory
         }
      }
      PakCreate(nodes, "Pak from memory.pak"); // create Pak from nodes
   }

   // load custom Pak
   {
      Paks.add("Pak from memory.pak"); 

      // from now on any data stored in loaded Pak can be accessed through IO methods
      {
         File f1("File 1.dat");           // open the file
         b1=f1.getByte();                 // read Byte from that file

         File f2("Directory/File 2.dat"); // open the file
         b2=f2.getByte();                 // read Byte from that file
      }
   }
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
   D.text (0, 0, S+b1+"  "+b2); // display bytes
}
/******************************************************************************/
