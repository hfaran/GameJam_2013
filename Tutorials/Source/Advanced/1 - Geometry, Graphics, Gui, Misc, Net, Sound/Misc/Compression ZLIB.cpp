/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Char8 text[]={"This is a simple text which will be compressed using ZLIB compression. 0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ"};

Byte *  compressed, // buffer where   compressed data will be stored
     *decompressed; // buffer where decompressed data will be stored

Int  src_size=SIZE(text),
     compressed_buf_size,
     compressed_size,
   decompressed_size;
/******************************************************************************/
void InitPre()
{
   App.name("ZLIB Compression");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   // compression
   {
      // for compression we'll need buffer which will hold enough data
      // how much is enough is calculated through 'ZLIBSize' function and giving source size
      compressed_buf_size=ZLIBSize(src_size);

      // create raw memory buffer able to store 'compressed_buf_size' bytes
      Alloc(compressed, compressed_buf_size);

      // compress data
      // before compressing we must set destination buffer size('compressed_size') to maximum memory available
      compressed_size=compressed_buf_size;
      ZLIBCompress(compressed, compressed_size, text, src_size); // now 'compressed' contains compressed data, and 'compressed_size' is the actual size of compressed data
   }
   
   // decompression
   {
      // for decompression we'll need buffer which will hold decompressed data (we know that decompressed size is the size of the source)
      Alloc(decompressed, src_size);

      // decompress data
      // before decompression we must set destination buffer size('decompressed_size') to maximum memory available
      decompressed_size=src_size;
      ZLIBDecompress(decompressed, decompressed_size, compressed, compressed_size); // now 'decompressed' contains decompressed data, and 'decompressed_size' is the actual size of decompressed data
   }
   return true;
}
/******************************************************************************/
void Shut()
{
   // free buffers
   Free(  compressed);
   Free(decompressed);
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

   D.text(0, 0.2f, S+"Source size: "+src_size);
   D.text(0, 0.1f, S+"Compressed size: "+compressed_size);
   D.text(0, 0.0f, S+"Buffer size used for compression: "+compressed_buf_size);

   D.text(0, -0.2f, "Source text:"      ); D.text(Sin(Time.time())*2.5f, -0.3f,                 text);
   D.text(0, -0.5f, "Decompressed text:"); D.text(Sin(Time.time())*2.5f, -0.6f, (Char8*)decompressed);
}
/******************************************************************************/
