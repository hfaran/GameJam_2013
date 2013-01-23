/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
struct Player
{
   Ball      bounding_ball; // bounding ball covering the character
   MeshPtr   mesh         , // main body mesh
             armor        , // armor     mesh
             pants        , // pants     mesh
             boots        , // boots     mesh
             gauntlets    ; // gauntlets mesh
   CSkeleton cskel        ; // controlled skeleton

   // update
   void update()
   {
      // set skeleton animation
      cskel.clear()
           .animate(L"anim/fist-l.anim", Time.time())
           .animate(L"anim/fist-r.anim", Time.time())
           .animate(L"anim/walk.anim"  , Time.time())
           .updateMatrix(MatrixIdentity)
           .updateVelocities();

      // set bounding ball
      if(mesh)bounding_ball.setAnimated(mesh->box, cskel);else bounding_ball.set(2, cskel.pos());
   }

   // draw
   void drawPrepare()
   {
      if(Frustum(bounding_ball)) // if player is visible in frustum
      {
         // add the meshes to the draw list
         if(armor    )armor    ->draw(cskel);
         if(pants    )pants    ->draw(cskel);
         if(boots    )boots    ->draw(cskel);
         if(gauntlets)gauntlets->draw(cskel);
         if(mesh     )mesh     ->draw(cskel);
      }
   }
   void drawShadow()
   {
      if(Frustum(bounding_ball)) // if player is visible in current shadow frustum
      {
         // add the meshes to the shadow draw list
         if(armor    )armor    ->drawShadow(cskel);
         if(pants    )pants    ->drawShadow(cskel);
         if(boots    )boots    ->drawShadow(cskel);
         if(gauntlets)gauntlets->drawShadow(cskel);
         if(mesh     )mesh     ->drawShadow(cskel);
      }
   }

   void createDefault()
   {
      // set meshes
      mesh     ="obj/chr/warrior/body.mesh";
      armor    ="obj/chr/warrior/armor.mesh";
      pants    ="obj/chr/warrior/pants.mesh";
      boots    ="obj/chr/warrior/boot.mesh";
      gauntlets="obj/chr/warrior/gauntlet.mesh";

      // create skeleton and set initial matrixes
      cskel.create(Skeletons("obj/chr/warrior/body.skel"));
   }

   Player()
   {
      bounding_ball.set(0, Vec(0));
   }
};
/******************************************************************************/
Player player;
/******************************************************************************/
void InitPre()
{
   App.name("Armor Rendering");
   App.flag=APP_FULL_TOGGLE|APP_MS_EXCLUSIVE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true).ambientPower(0.3f).hpRt(true)
    .shadowJitter(true).shadowSoft(1).shadowMapSize(2048)
    .viewFrom(0.005f).viewRange(5);

   Cam.yaw  = 2.7f;
   Cam.pitch=-0.3f;
   Cam.dist = 1.2f;
}
/******************************************************************************/
Bool Init()
{
   // initialize sky & sun
   Sky.atmospheric();
   Sun.image="Env/Sky/sun.gfx";
   Sun.pos=!Vec(1,1,1);
   Sun.light_color=1-D.ambientColor();

   // setup player data
   player.createDefault();

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
   CamHandle(0.01f, 100, CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT));

   player.update();

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE: player.drawPrepare(); break;
      case RM_SHADOW : player.drawShadow (); break;
   }
}
void Draw()
{
   Renderer(Render);
}
/******************************************************************************/
