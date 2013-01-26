#include "stdafx.h"
#include "main.hpp"

#ifndef BACKGROUNDLOADER_H
#define BACKGROUNDLOADER_H

struct BackgroundLoader
{
   // static functions
   static Bool BackgroundLoad(Thread &thread) // this function will be called in the secondary thread to load the initial world data
   {
	  Time.wait(loadTime); // wait 2 seconds just for the needs of tutorial to make the loading screen not disappear too quickly
	  return false; // don't continue the thread
   }

   // members
   Thread thread;
   Image eLogo;
   static const int loadTime=2000; //The time the esenthel logo (and other logos) show in ms

   // methods
   void del() // delete background loader
   {
	  thread.del();
   }
   void start() // start background loader
   {
	  thread.create(BackgroundLoad); // start loader thread
   }
   Bool update() // update background loader
   {
	  if(!thread.created())return false; // not loading anything
	  customUpdate(); // call custom updating method
	  return true; // loading
   }
   Bool draw() // draw background loader
   {
	  if(!thread.created())return false; // not loading anything
	  customDraw(); // call custom drawing method
	  if(!thread.active()) // thread is created but finished processing
	  {
		 thread.del(); // delete the thread
		 Renderer.setFade(1.0f); // enable screen fading from loading screen to the game
	  }
	  return true; // loading or finishing
   }

   void customUpdate() // you can modify this method and perform custom updating of the loading screen
   {
	  Time.wait(1000/24); // limit main thread speed to 24 fps, to give more cpu power for background thread
   }
   void customDraw() // you can modify this method and perform custom drawing of the loading screen
   {
		D.clear(BLACK);
		eLogo.draw(Rect((Flt) -RES_X/RES_Y, -1.0f, (Flt) RES_X/RES_Y, 1.0f));
   }
};


#endif