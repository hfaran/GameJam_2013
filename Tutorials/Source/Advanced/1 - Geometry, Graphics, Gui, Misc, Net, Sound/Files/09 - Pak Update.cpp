/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Str data1, data2, data3;
/******************************************************************************/
void InitPre()
{
   App.name("Pak Update");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   // first create a Pak from files in memory
   {
      Memb<PakFileData> files; // set of file's which we'll create and write to them

      // add "file1.dat"
      {
         PakFileData &pfd=files.New();
         pfd.name="file1.dat";               // set the file location inside the pak
         pfd.file.writeMem();                // open file data for writing
         pfd.file.putStr("Original Data 1"); // write some text to the file
         pfd.modify_time_utc.getUTC();       // set file modification time
      }
      
      // add "file2.dat"
      {
         PakFileData &pfd=files.New();
         pfd.name="file2.dat";               // set the file location inside the pak
         pfd.file.writeMem();                // open file data for writing
         pfd.file.putStr("Original Data 2"); // write some text to the file
         pfd.modify_time_utc.getUTC();       // set file modification time
      }

      // add "Folder/file3.dat"
      {
         PakFileData &pfd=files.New();
         pfd.name="Folder/file3.dat";        // set the file location inside the pak
         pfd.file.writeMem();                // open file data for writing
         pfd.file.putStr("Original Data 3"); // write some text to the file
         pfd.modify_time_utc.getUTC();       // set file modification time
      }
      
      PakCreate(files, "Pak from memory.pak"); // create the pak
   }
   
   // Update the Pak
   {
      Memb<PakFileData> files;

      // don't modify "file1.dat"

      // remove "file2.dat"
      {
         PakFileData &pfd=files.New();
         pfd.name="file2.dat";         // set the file location inside the pak
         pfd.mode=PakFileData::REMOVE; // specify that the file shouldn't exist
      }
      
      // update "Folder/file3.dat"
      {
         PakFileData &pfd=files.New();
         pfd.name="Folder/file3.dat";       // set the file location inside the pak
         pfd.file.writeMem();               // open file data for writing
         pfd.file.putStr("Updated Data 3"); // write some text to the file
         pfd.modify_time_utc.getUTC();      // set file modification time
      }

      Pak pak;  pak.load(   "Pak from memory.pak"); // load the source pak
      PakUpdate(pak, files, "Pak from memory.pak"); // update the pak
   }

   // load data from Pak
   {
      Paks.add("Pak from memory.pak");

      File f;
      if(f.readTry(       "file1.dat"))f.getStr(data1);
      if(f.readTry(       "file2.dat"))f.getStr(data2);
      if(f.readTry("Folder/file3.dat"))f.getStr(data3);
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
   D.text (0,  0.1f, data1);
   D.text (0,  0.0f, data2);
   D.text (0, -0.1f, data3);
}
/******************************************************************************/
