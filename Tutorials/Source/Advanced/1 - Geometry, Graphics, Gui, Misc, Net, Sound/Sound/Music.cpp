/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
// music themes, each for storing tracks
MusicTheme mt_battle , // this is battle theme used for playing when battles
           mt_explore, // exploring theme
           mt_calm   ; // calm theme
/******************************************************************************/
void InitPre()
{
   App.name("Music");
   DataPath("../data");
   Paks.add("engine.pak");
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;
   
   if(!mt_battle.songs()) // create 'mt_battle' theme if not yet created
   {
      mt_battle+="music/battle0.ogg"; // add "battle0.ogg" track to 'mt_battle' theme
      mt_battle+="music/battle1.ogg"; // add "battle1.ogg" track to 'mt_battle' theme
   }
   if(!mt_explore.songs()) // create 'mt_explore' theme if not yet created
   {
      mt_explore+="music/explore.ogg"; // add "explore.ogg" track to 'mt_explore' theme
   }
   if(!mt_calm.songs()) // create 'mt_calm' theme if not yet created
   {
      mt_calm+="music/calm.ogg"; // add "calm.ogg" track to 'mt_calm' theme
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
   if(Kb.c('1'))Music.play(mt_battle );
   if(Kb.c('2'))Music.play(mt_explore);
   if(Kb.c('3'))Music.play(mt_calm   );
   if(Kb.c('4'))Music.play(NULL      );
   return true;
}
/******************************************************************************/
void Draw()
{
   D.clear(WHITE);

   if(Music.theme()) // if any theme playing
   {
      D.text(0, 0.2f, S+"song: "+Music.name());
      D.text(0, 0.0f, S+"time " +Music.time()+" / "+Music.length()+" length");
   }else
   {
      D.text(0, 0, "No theme playing");
   }
   D.text(0, -0.2f, "Press 1-battle, 2-explore, 3-calm, 4-none");
}
/******************************************************************************/
