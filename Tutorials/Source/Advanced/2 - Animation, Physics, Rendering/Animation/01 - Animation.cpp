/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
CSkeleton cskel;
/******************************************************************************/
void InitPre()
{
   App.name("Animation");
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

   // set animations
   {   
      cskel.clear();     // clear controlled skeleton animation
      if(Kb.b(KB_SPACE)) // when space pressed
      {
         cskel.animate(L"../data/anim/walk.anim", Time.time()); // animate with "walk" animation and current time position
      }
      cskel.updateMatrix    (MatrixIdentity); // update controlled skeleton animation matrixes
      cskel.updateVelocities(              ); // update controlled skeleton bone velocities (this is needed for Motion Blur effect)
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
         LightDir(Vec(0,0,-1), 1-D.ambientColor()).add(); // set light

         Meshes("../data/obj/chr/skeleton/0.mesh")->draw(cskel); // get mesh from cache and render it with controlled skeleton matrixes
      }break;
   }
}
void Draw()
{
   Renderer(Render);

   if(Kb.ctrl()) // when control pressed
   {
      if(Renderer.rebuildDepthNeededForDebugDrawing())Renderer.rebuildDepth();
      SetMatrix();     // restore default matrix
      cskel.draw(RED); // draw controlled skeleton
   }

   D.text(0, 0.8f, "Hold space to animate");
}
/******************************************************************************/
