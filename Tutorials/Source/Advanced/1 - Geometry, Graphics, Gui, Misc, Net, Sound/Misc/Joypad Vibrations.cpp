/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Vec2 force(0,0);
/******************************************************************************/
static void SetVibration(C Vec2 &new_force)
{
   if(Joypads()>=1 && Joypad[0].supportsVibrations())
   {
      force=new_force;
      Clamp(force.x, -1, 1);
      Clamp(force.y, -1, 1);

      Joypad[0].vibration(force);
   }
}
/******************************************************************************/
void InitPre()
{
   App.name("Joypad Vibrations (Force Feedback)");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color  =BLACK;
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
   if(Kb.bp(KB_ESC))return false;

   if(Kb.f(KF_LEFT ))SetVibration(force+Vec2(-0.2f,  0   ));
   if(Kb.f(KF_RIGHT))SetVibration(force+Vec2( 0.2f,  0   ));
   if(Kb.f(KF_DOWN ))SetVibration(force+Vec2( 0   , -0.2f));
   if(Kb.f(KF_UP   ))SetVibration(force+Vec2( 0   ,  0.2f));

   return true;
}
/******************************************************************************/
void Draw()
{
   D.clear(WHITE);
   
   if( Joypads()<=0                  )D.text(0, 0, "No Joypads Detected");else
   if(!Joypad[0].supportsVibrations())D.text(0, 0, "Joypad #0 doesn't support Vibrations");else
   {
      D.text(0, 0.9f, "Press Keyboard Arrow Keys to Change Applied Force");
      D.text(0, 0   , S+"Applied Force: "+force);
   }
}
/******************************************************************************/
