/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
CSkeleton cskel;
Flt       blend; // blending value (0..1)
Bool      walk ; // if want to walk
/******************************************************************************/
void InitPre()
{
   App.name("Animation Blending");
   App.flag=APP_MS_EXCLUSIVE;
   Paks.add("../data/engine.pak");
   D.ambientPower(0.1f);
}
/******************************************************************************/
Bool Init()
{
   Cam.dist=2;
   Cam.yaw =PI;

   cskel.create(Skeletons("../data/obj/chr/skeleton/0.skel"), 1.7f); // create controlled skeleton from skel file, with 1.7 meter height
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
   CamHandle(0.1f, 10, CAMH_ROT|CAMH_ZOOM);
   if(Kb.bp(KB_SPACE))walk^=1; // change 'walk' when space pressed

   // adjust blend value
   if(walk)
   {
      blend+=Time.d()*2;
      if(blend>1)blend=1;
   }else
   {
      blend-=Time.d()*2;
      if(blend<0)blend=0;
   }

   // set animations
   {   
      cskel.clear  (); // clear controlled skeleton animation
      cskel.animate(L"../data/anim/walk.anim", Time.time(),   blend); // animate with "walk" animation, current time position and "  blend" blending weight
      cskel.animate(L"../data/anim/run.anim" , Time.time(), 1-blend); // animate with "run"  animation, current time position and "1-blend" blending weight
      cskel.updateMatrix(MatrixIdentity).updateVelocities();          // update controlled skeleton animation and velocities
   }

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         Meshes("../data/obj/chr/skeleton/0.mesh")->draw(cskel); // get mesh from cache and render it with controlled skeleton

         LightDir(Vec(0,0,-1), 1-D.ambientColor()).add();
      }break;
   }
}
void Draw()
{
   Renderer(Render);

   if(Kb.ctrl()) // when control pressed
   {
      SetMatrix();     // restore default matrix
      cskel.draw(RED); // draw controlled skeleton
   }

   D.text(0, 0.8f, S+"Press space to toggle blending (Walk: "+walk+", Blend: "+blend+')');
}
/******************************************************************************/
