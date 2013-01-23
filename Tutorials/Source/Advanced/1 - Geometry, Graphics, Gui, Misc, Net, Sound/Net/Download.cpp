/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Image    image   ;
Download download;
/******************************************************************************/
void InitPre()
{
   App.name("Downloading file");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   download.create("http://www.esenthel.com/download/logo.gfx"); // create downloader

   return true;
}
/******************************************************************************/
void Shut()
{
   download.del(); // delete downloader
}
/******************************************************************************/
Bool Update()
{
   if(Kb.bp(KB_ESC))return false;

   if(download.state()==DWNL_DONE) // if all data downloaded successfully
   {
      // data can be accessed freely through Download::buf,size methods
      // for example download.buf()[0] is the first Byte of data
      // we'll use the downloaded data to load an image from it

      if(!image.is()) // check if we haven't already created it
      {
         File f(download.data(), download.size()); // create a file from memory data
         image.load(f);                            // load an image from the file
      }
   }
   return true;
}
/******************************************************************************/
void Draw()
{
   D.clear(WHITE);

   if(image.is())     // when image is downloaded and created
      image.drawFs(); // draw it

   switch(download.state())
   {
      case DWNL_NONE    : D.text(0,  0   ,   "Nothing"                                           ); break;
      case DWNL_WAIT    : D.text(0,  0   ,   "Awaiting for connection.."                         ); break;
      case DWNL_DOWNLOAD: D.text(0,  0   , S+"Downloading.. "+download.done()+'/'+download.size()); break;
      case DWNL_DONE    : D.text(0, -0.9f,   "Done"                                              ); break;
      case DWNL_ERROR   : D.text(0,  0   ,   "Error encountered"                                 ); break;
   }
}
/******************************************************************************/
