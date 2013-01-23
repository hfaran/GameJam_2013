/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Sound sound; // water sound
/******************************************************************************/
void InitPre()
{
   App.name("Sound");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;
   return true;
}
/******************************************************************************/
void Shut()
{
}
/******************************************************************************/
Bool Update()
{
   if(Kb.bp(KB_ESC  ))return false;
   if(Kb.bp(KB_SPACE))SoundPlay("../data/sound/metal.ogg"); // play sound on space

   if(Kb.bp(KB_ENTER)) // toggle water on enter
   {
      if(sound.playing()) // if already playing
      {
         sound.del(); // delete sound
      }else
      {
         sound.play("../data/sound/water.ogg", true); // play with loop option enabled
      }
   }

   // change volumes on mouse wheel
   {
      if(Ms.wheel()>0)sound.volume(sound.volume()+0.1f);else
      if(Ms.wheel()<0)sound.volume(sound.volume()-0.1f);
   }

   // change speed on LMB/RMB
   {
      if(Ms.b(0))sound.speed(sound.speed()-Time.d()*0.4f);
      if(Ms.b(1))sound.speed(sound.speed()+Time.d()*0.4f);
   }
   return true;
}
/******************************************************************************/
void Draw()
{
   D.clear(WHITE);
   D.text (0,  0.2f,   "Press space to play 'metal'");
   D.text (0,  0.0f, S+"Press enter to stop/play looped 'water' (playing: "     +sound.playing()+')');
   D.text (0, -0.2f, S+"Use mouse wheel to change water sound volume (current: "+sound.volume ()+')');
   D.text (0, -0.4f, S+  "Press LMB/RMB to change water sound speed (current: " +sound.speed  ()+')');
}
/******************************************************************************/
