/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
MusicTheme mt_battle  , // music themes
           mt_explore ;
Sound      sound      ; // sound
Slider     vol_fx     , // volume bars
           vol_music  ,
           vol_ambient;
Text      tvol_fx     , // volume text
          tvol_music  ,
          tvol_ambient;
/******************************************************************************/
void InitPre()
{
   App.name("Volume groups");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   Gui.ts_text.scale*=1.5f; // make all Text objects bigger

   if(!mt_battle.songs()) // create mt_battle theme
   {
      mt_battle+="../data/music/battle0.ogg"; // add "battle0.ogg" track to 'mt_battle' theme
      mt_battle+="../data/music/battle1.ogg"; // add "battle0.ogg" track to 'mt_battle' theme
   }
   if(!mt_explore.songs()) // create mt_explore theme
   {
      mt_explore+="../data/music/explore.ogg"; // add "explore.ogg" track to 'mt_explore' theme
   }
   
   Music  .play(mt_battle ); // play theme as music
   Ambient.play(mt_explore); // play theme as ambient
   sound  .play("../data/sound/water.ogg", true); // play looped sound

   // create gui sliders
   Gui+=vol_fx     .create(Rect(-0.1f, 0.30f, 0.3f, 0.40f), SoundVolume.fx     ()); Gui+=tvol_fx     .create(Vec2(-0.3f, 0.35f), "Fx"     );
   Gui+=vol_music  .create(Rect(-0.1f, 0.15f, 0.3f, 0.25f), SoundVolume.music  ()); Gui+=tvol_music  .create(Vec2(-0.3f, 0.20f), "Music"  );
   Gui+=vol_ambient.create(Rect(-0.1f, 0.00f, 0.3f, 0.10f), SoundVolume.ambient()); Gui+=tvol_ambient.create(Vec2(-0.3f, 0.05f), "Ambient");

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
   Gui.update();

   // set new volumes
   SoundVolume.fx     (vol_fx     ());
   SoundVolume.music  (vol_music  ());
   SoundVolume.ambient(vol_ambient());
   return true;
}
/******************************************************************************/
void Draw()
{
   D  .clear(WHITE);
   Gui.draw ();
}
/******************************************************************************/
