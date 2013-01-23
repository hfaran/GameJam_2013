/******************************************************************************/
#include "stdafx.h"
/******************************************************************************

   Files are universal and can be used to:
      -read/write from stdio files (typical files on you Hard Drive)
      -read files from Pak         (an archive of multiple files)
      -read/write in   RAM         (without the need of writing files on Disk)

   They also handle encryption/decryption with custom Secure key.

/******************************************************************************/
struct Data
{
   Char text[256];
};

Byte b;
UInt u;
Data data;
/******************************************************************************/
void InitPre()
{
   App.name("File");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   Set(data.text, "Sample text");

   // for testing we'll start with with writing to a file in memory
   {
      File f; // file object

      f.writeMem(        ); // start writing to memory (file will automatically handle the memory allocations)
      f.putByte (     128); // put Byte      '128' to file
      f.putUInt (12345678); // put UInt '12345678' to file
      f<<data;              // put 'data'          to file (its raw memory)
   }
   
   // now let's write all that but this time to a real file on Hard Disk
   {
      File f;

      f.write  ("LocalData/file.dat"); // start writing to file
      f.putByte(     128);
      f.putUInt(12345678);
      f<<data;
   }

   // clear before reading
   data.text[0]=0;

   // when the file is created we can now read it
   {
      File f;
      
        f.read   ("LocalData/file.dat"); // start reading file
      b=f.getByte(); // read Byte   (returns 128)
      u=f.getUInt(); // read UInt   (returns 12345678)
        f>>data    ; // read 'data' (data.text should be now "Sample text")
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
   D.text (0, 0, S+b+" "+u+" "+data.text); // display data obtained from file
}
/******************************************************************************/
