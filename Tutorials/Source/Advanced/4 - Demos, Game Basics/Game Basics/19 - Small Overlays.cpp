/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/_enums.h"
/******************************************************************************/
STRUCT(Decal2 , Decal) // create a helper class which bases on Decal and uses time fading
//{
   Flt time; // time left until overlay fades out

   Decal2()
   {
      time=10; // default time is 10 seconds for an overlay to live
   }

   Bool update()
   {
             time-=Time.d(); // decrease time left
      return time<=0       ; // if there is no time left then return true (which means that the overlay can be deleted)
   }
   void draw(C Matrix &matrix)
   {
      super::drawAnimated(matrix, Sat(time)); // draw the overlay with transparency of 'Saturate(time)' value
   }

   // io
   void save(File &f)
   {
      super::save(f);
      f<<time;
   }
   Bool load(File &f)
   {
      if(super::load(f))
      {
         f>>time;
         return true;
      }
      return false;
   }
};
/******************************************************************************/
STRUCT(Item , Game::Item) // extend items
//{
   Memb<Decal2> decals; // add decal container, which is used for rendering semi transparent images on solid surfaces

   virtual Bool update     (); // extend updating to include 'decals' update
   virtual UInt drawPrepare(); // extend drawing  to include request for the RM_OVERLAY mode
   virtual void drawOverlay(); // extend drawing  to include rendering of 'decals'

   // io
   virtual void save(File &f); // extend saving  to include members saving
   virtual Bool load(File &f); // extend loading to include members loading

   // operations
   void addBulletHole(C Vec &pos, C Vec &surface_normal, C Vec &shot_dir); // helper method for adding a bullet hole (Decal) onto the item mesh
};
/******************************************************************************/
Game::ObjMemx<Item> Items;
/******************************************************************************/
// ITEM
/******************************************************************************/
Bool Item::update()
{
   if(super::update())
   {
      REPA(decals)if(decals[i].update()) // update all overlays
         decals.remove(i, true); // and remove them if they faded out

      return true;
   }
   return false;
}
/******************************************************************************/
UInt Item::drawPrepare()
{
   UInt modes=super::drawPrepare();
   if(decals.elms())modes|=IndexToFlag(RM_OVERLAY); // include request for additional render mode
   return modes;
}
void Item::drawOverlay()
{
   REPAO(decals).draw(matrixScaled()); // draw mesh_overlays with the same matrix used for default item drawing
}
/******************************************************************************/
void Item::save(File &f)
{
   super::save(f);

   decals.save(f);
}
Bool Item::load(File &f)
{
   if(super::load(f))
   {
      return decals.load(f);
   }
   return false;
}
/******************************************************************************/
void Item::addBulletHole(C Vec &pos, C Vec &surface_normal, C Vec &shot_dir)
{
   Matrix bullet_matrix; bullet_matrix.setPosDir(pos, surface_normal).scaleOrn(0.05f);

   // add a decal to the item
   Decal &decal=decals.New();
   decal.material (Materials.ptrRequire("Decal/bullet.mtrl"));
   decal.setMatrix(matrixScaled(), bullet_matrix);

   // add impulse to the actor
   actor.addImpulse(shot_dir * 1.5f, pos);
}
/******************************************************************************/
// MAIN
/******************************************************************************/
void InitPre()
{
   App.name("Small Overlays");
   App.flag=APP_FULL_TOGGLE|APP_MS_EXCLUSIVE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true).hpRt(true).ambientPower(0.3f).viewRange(70);

   Cam.at.set(16,2,12);
   Cam.dist = 0.01f;
   Cam.pitch=-0.2f;
}
/******************************************************************************/
Bool Init()
{
   Physics.create(CSS_NONE, true, "../Installation/PhysX");
   Sky    .atmospheric();
   Sun.image="Env/Sky/sun.gfx"; Sun.light_color=1-D.ambientColor();

   Game::World.init      (               )
              .setObjType(Items, OBJ_ITEM)
              .New       ("world/custom params.world");

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

   // update the camera FPP 'WSAD' style
   if(Kb.b(KB_A     ))Cam.at-=Cam.matrix.x*Time.d()*2;
   if(Kb.b(KB_D     ))Cam.at+=Cam.matrix.x*Time.d()*2;
   if(Kb.b(KB_S     ))Cam.at-=Cam.matrix.z*Time.d()*2;
   if(Kb.b(KB_W     ))Cam.at+=Cam.matrix.z*Time.d()*2;
   if(Kb.b(KB_LSHIFT))Cam.at-=Vec(0,1,0)  *Time.d()*2;
   if(Kb.b(KB_SPACE ))Cam.at+=Vec(0,1,0)  *Time.d()*2;
   CamHandle(0.01f, 0.01f, CAMH_ROT); // allow only rotation

   Game::World.update(Cam.at);

   if(Ms.bp(0)) // on LMB pressed
   {
      // calculate world position and direction vectors
      Vec     pos, dir; ScreenToPosDir(Vec2(0,0), pos, dir);
      PhysHit phys_hit; if(Physics.ray(pos, dir*D.viewRange(), &phys_hit)) // if ray test hit an actor
      {
         if(phys_hit.obj) // if the actor comes from an object
         {
            if(Item *item=CAST(Item, phys_hit.obj)) // if the object is an item
            {
               item->addBulletHole(phys_hit.plane.pos, phys_hit.plane.normal, dir); // call item method to add a bullet hole
            }
         }
         else // the actor doesn't have an object set, so most probably it's a terrain actor
         {
            // add a decal to the world terrain
            Game::World.terrainAddDecal(WHITE, Materials.ptrRequire("Decal/bullet.mtrl"), Matrix().setPosDir(phys_hit.plane.pos, phys_hit.plane.normal).scaleOrn(0.05f));
         }
      }
   }

   return true;
}
/******************************************************************************/
void Render()
{
   Game::World.draw();
}
void Draw()
{
   Renderer(Render);
   D.text(0, 0.9f, "Press LMB to shoot");

   // draw simple crosshair
   D.lineX(WHITE, 0, -0.08f, 0.08f);
   D.lineY(WHITE, 0, -0.08f, 0.08f);
}
/******************************************************************************/
