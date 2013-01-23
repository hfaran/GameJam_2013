/******************************************************************************/
#include "stdafx.h"
/******************************************************************************

   This tutorial shows the basics of compression using 'Compress' and 'Decompress'

   Please note that it may take up to 1 minute until the tutorial starts,
      because of the big source size used for compression testing.

/******************************************************************************/
File src,

     zlib_compressed,
     lzma_compressed,

     zlib_decompressed,
     lzma_decompressed;

Flt zlib_compress_time,
    lzma_compress_time,

    zlib_decompress_time,
    lzma_decompress_time;
/******************************************************************************/
void InitPre()
{
   App.name("Compression");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;
   DefaultTextStyle.scale*=0.9f;

   // read the source file, we'll use "engine.pak" because it's big enough
   src.read("../data/engine.pak");

   // compress the source into 'zlib_compressed' and 'lzma_compressed' files in memory, using different compression algorithms, and measure the time
   zlib_compress_time=Time.curTime(); src.pos(0); zlib_compressed.writeMem(); Compress(src, zlib_compressed, COMPRESS_ZLIB); zlib_compress_time=Time.curTime()-zlib_compress_time;
   lzma_compress_time=Time.curTime(); src.pos(0); lzma_compressed.writeMem(); Compress(src, lzma_compressed, COMPRESS_LZMA); lzma_compress_time=Time.curTime()-lzma_compress_time;

   // decompress the data, and measure the time
   zlib_decompress_time=Time.curTime(); zlib_compressed.pos(0); zlib_decompressed.writeMem(); Decompress(zlib_compressed, zlib_decompressed); zlib_decompress_time=Time.curTime()-zlib_decompress_time;
   lzma_decompress_time=Time.curTime(); lzma_compressed.pos(0); lzma_decompressed.writeMem(); Decompress(lzma_compressed, lzma_decompressed); lzma_decompress_time=Time.curTime()-lzma_decompress_time;

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

   D.text(0, 0.2f, S+"Source Size: "+(src.size()>>10)+"KB");
   D.text(0, 0.0f, S+"ZLIB - Compressed Size: "+(zlib_compressed.size()>>10)+"KB, Compression / Decompression Time: "+zlib_compress_time+" / "+zlib_decompress_time);
   D.text(0,-0.2f, S+"LZMA - Compressed Size: "+(lzma_compressed.size()>>10)+"KB, Compression / Decompression Time: "+lzma_compress_time+" / "+lzma_decompress_time);
}
/******************************************************************************/
