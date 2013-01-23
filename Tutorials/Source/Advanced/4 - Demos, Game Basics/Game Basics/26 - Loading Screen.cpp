/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/_enums.h"
/******************************************************************************/
struct BackgroundLoader
{
   // static functions
   static Bool BackgroundLoad(Thread &thread) // this function will be called in the secondary thread to load the initial world data
   {
      ThreadMayUseGPUData(); // notify that the thread may use GPU data (required for OpenGL renderer)
      Game::World.update(Cam.at); // call the world update which will load the necessary data
      Time.wait(2000); // wait 2 seconds just for the needs of tutorial to make the loading screen not disappear too quickly
      return false; // don't continue the thread
   }

   // members
   Thread thread;

   // methods
   void del() // delete background loader
   {
      Game::World.updateBreak(); // request break of the updating method
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
      D.clear(TURQ);
      D.text (0, 0, S+"Loading... ("+Round(Game::World.updateProgress()*100)+"%)");
   }
}BL;
/******************************************************************************/
Game::ObjMemx<Game::Static> Statics;
Game::ObjMemx<Game::Chr   > Chrs   ;
/******************************************************************************/
void InitPre()
{
   App.name("Loading Screen");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");
   Cam.at.set(16,0,16);
   Cam.pitch=-0.5f;
   Cam.dist=15;

   D.hpRt(true).ambientPower(0.3f);
}
/******************************************************************************/
Bool Init()
{
   Physics.create(CSS_NONE, true, "../Installation/PhysX");
   Sky    .atmospheric();
   Sun.image="Env/Sky/sun.gfx"; Sun.light_color=1-D.ambientColor();

   Game::World.init()
              .setObjType(Statics, OBJ_STATIC)
              .setObjType(Chrs   , OBJ_PLAYER)
              .New("world/sample.world");

   BL.start(); // create background loader

   return true;
}
/******************************************************************************/
void Shut()
{
   BL.del(); // background loader must be manually deleted
}
/******************************************************************************/
void SetCamera()
{
   CamHandle(0.1f, 100, CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT));
}
Bool Update()
{
   if(BL.update())return true; // this needs to be at the start of 'Update' function

   if(Kb.bp(KB_ESC))return false;
   Game::World.update(Cam.at);
   SetCamera();
   return true;
}
/******************************************************************************/
void Render()
{
   Game::World.draw();
}
void Draw()
{
   if(BL.draw())return; // this needs to be at the start of 'Draw' function

   Renderer(Render);
}
/******************************************************************************/
