/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
struct Player
{
   Mesh     *mesh ;
   CSkeleton cskel;

   void update()
   {
      cskel.clear()
           .animate(L"anim/fist-l.anim", Time.time())
           .animate(L"anim/fist-r.anim", Time.time())
           .animate(L"anim/walk.anim"  , Time.time())
           .updateMatrix(MatrixIdentity)
           .updateVelocities();
   }

   void drawPrepare()
   {
      if(mesh)mesh->draw(cskel);
   }
   void drawOverlay()
   {
      if(mesh)
      {
         cskel.setMatrix(); // first setup rendering matrixes
         MeshLod &lod=mesh->getDrawLod(cskel.matrix()); // get desired mesh lod level
         REPA(lod)lod.parts[i].drawOverlay(*Images("obj/chr/warrior/tattoo/tattoo.gfx")); // apply an overlay for all MeshPart's
      }
   }

   void createDefault()
   {
      mesh =          Meshes("obj/chr/warrior/body.mesh");
      cskel.create(Skeletons("obj/chr/warrior/body.skel"));
   }

   Player()
   {
      mesh=NULL;
   }
};
/******************************************************************************/
Player player;
/******************************************************************************/
void InitPre()
{
   App.name("Tattoo");
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
      case RM_OVERLAY: player.drawOverlay(); break;
   }
}
void Draw()
{
   Renderer(Render);
}
/******************************************************************************/
