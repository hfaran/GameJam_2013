/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Sound sound;      // sound
Vec   pos(0,0,3); // sound position
/******************************************************************************/
void InitPre()
{
   App.name("Sound 3D");
   App.flag=APP_MS_EXCLUSIVE;
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   sound.play("../data/sound/water.ogg", pos, 1, true); // play looped 3D sound, volume=1, position='pos'
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
   CamHandle(0.1f, 10, CAMH_ZOOM|CAMH_ROT);

   // update all 3D sound positions
   {
      sound.pos(pos); // since 'pos' is the same in each frame, the sound position doesn't need to be updated, but let's do it anyway
   }

   // update listener parameters
   {
      Listener.orn(Cam.matrix.z, Cam.matrix.y)  // set listener orientation (from camera)
              .pos(Cam.matrix.pos            ); // set listener position    (from camera)
   }
   return true;
}
/******************************************************************************/
void Draw()
{
   D.clear(WHITE);

   SetMatrix();
   Ball(1, pos).draw(BLACK); // draw ball at sound position
}
/******************************************************************************/
