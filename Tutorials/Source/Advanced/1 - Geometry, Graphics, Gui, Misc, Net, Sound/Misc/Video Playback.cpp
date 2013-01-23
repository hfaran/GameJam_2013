/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Theora video;
/******************************************************************************/
void InitPre()
{
   App.name("Video Playback");
   App.flag=APP_MEM_LEAKS;
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   if(!video.create("../data/video/test.ogv"))Exit("Can't create video"); // create video from Theora File
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

   video.update(StateActive->time()); // update video to current state time

   return true;
}
/******************************************************************************/
void Draw()
{
   D    .clear ();
   video.drawFs(); // draw video
   D    .text  (0, 0.9f, S+video.time()); // draw time position of video
}
/******************************************************************************/
