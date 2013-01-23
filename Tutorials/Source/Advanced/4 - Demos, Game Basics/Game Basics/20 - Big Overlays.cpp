/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/_enums.h"
/******************************************************************************/
// EXPLOSION HEADER
/******************************************************************************/
struct ExplosionFx // Explosion Effect
{
   Particles main,single;

   // manage
   ExplosionFx& del   ();
   ExplosionFx& create(ImagePtr main, ImagePtr single, Color color, Int elms, C Vec &pos, Flt size);

   // update/draw
   Bool update(); // update, returns false when effect faded out and is inactive, true when still active
   void draw  (); // draw  , can be called in following render modes : RM_BLEND, RM_PALETTE

   // io
   void save(File &f); // save binary to   file
   Bool load(File &f); // load binary from file, false on fail
};
/******************************************************************************/
// EXPLOSION SOURCE
/******************************************************************************/
ExplosionFx& ExplosionFx::del()
{
   main  .del();
   single.del();
   return T;
}
ExplosionFx& ExplosionFx::create(ImagePtr main, ImagePtr single, Color color, Int elms, C Vec &pos, Flt size)
{
   del();

   Ball ball(0.3f*size);
   if(main)
   {
      T.main.create(main, color, elms, size, 0); // store 'size' in 'radius'
      T.main.palette(true);
      T.main.accel.set(0, -1.5f, 0);
      T.main.radius_scale_base=0;
      T.main.radius_scale_time=1;
      T.main.life_total=0.50f;
      T.main.fade_in   =0.02f;
      T.main.fade_out  =0.42f;
      T.main.damping   =0.96f;
      T.main.matrix    =pos; // set position in case the user will try to access 'Particles' position

      REPA(T.main)
      {
         Particle &p=T.main.p(i);
         p.life_max=RandomF(0.4f, 1   )*T.main.life_total;
         p.radius  =RandomF(1.7f, 5.5f)*size;
         p.pos     =Random (ball);
         p.vel     =p.pos; p.vel.setLength(RandomF(1, 5)*size);
         p.pos    +=  pos;
      }
   }

   if(single)
   {
      T.single.create(single, ColorMul(color, 1.3f), elms*10, 0, 0);
      T.single.palette(true);
      T.single.accel.set(0, -3.5f, 0);
      T.single.radius_scale_base=0;
      T.single.radius_scale_time=0.6f;
      T.single.life_total=0.5f;
      T.single.fade_in   =0.02f;
      T.single.fade_out  =0.20f;
      T.single.damping   =0.98f;
      T.single.matrix    =pos; // set position in case the user will try to access 'Particles' position

      REPA(T.single)
      {
         Particle &p=T.single.p(i);
         p.life_max=RandomF(0.4f , 1    )*T.single.life_total;
         p.radius  =RandomF(0.03f, 0.10f)*size;
         p.pos     =Random (ball);
         p.vel     =p.pos; p.vel.setLength(RandomF(1, 8)*size);
         p.pos    +=  pos;
      }
   }
   return T;
}
/******************************************************************************/
Bool ExplosionFx::update()
{
   Bool   main_alive=main  .update(),
        single_alive=single.update();
   return main_alive || single_alive;
}
void ExplosionFx::draw()
{
   main  .draw();
   single.draw();
   if(main.is())DrawVelocityBlur(main.fade()*0.4f, Ball(Sqrt(main.radiusScale()*4.5f)*main.radius, main.matrix.pos));
}
/******************************************************************************/
void ExplosionFx::save(File &f)
{
   main  .save(f, true);
   single.save(f, true);
}
Bool ExplosionFx::load(File &f)
{
   return main.load(f)
     && single.load(f);
}
/******************************************************************************/
// MESH OVERLAY
/******************************************************************************/
STRUCT(MeshOverlay2 , MeshOverlay) // create a helper class which bases on MeshOverlay and uses time fading
//{
   Flt time; // time left until overlay fades out

   MeshOverlay2()
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
      super::draw(matrix, Sat(time)); // draw the overlay with transparency of 'Saturate(time)' value
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
// ITEM
/******************************************************************************/
STRUCT(Item , Game::Item) // extend items
//{
   Int                type        ; // ITEM_TYPE
   Memb<MeshOverlay2> mesh_overlay; // add mesh overlay, which is used for rendering semi transparent images on solid surfaces

   virtual void create     (Game::ObjParams &obj); // extend creation to include accessing item type
   virtual Bool update     (                    ); // extend updating to include bomb explosion, and 'mesh_overlay' update
   virtual UInt drawPrepare(                    ); // extend drawing  to include request for the RM_OVERLAY mode
   virtual void drawOverlay(                    ); // extend drawing  to include rendering of 'decals'

   // io
   virtual void save(File &f); // extend saving  to include members saving
   virtual Bool load(File &f); // extend loading to include members loading

   Item();
};
/******************************************************************************/
// MAIN
/******************************************************************************/
Memb<ExplosionFx>   Explosions;
Game::ObjMemx<Item> Items;
/******************************************************************************/
void CreateExplosion(C Vec &pos) // helper function for creating an explosion at given position
{
   // create a new explosion effect
   Explosions.New().create(Images("obj/particles/explosion/0.gfx"), Images("obj/particles/explosion/1.gfx"), Color(96,0,0,0), 32, pos, 3);

   // set overlay parameters
   Flt         overlay_angle   =RandomF(PI2),
               overlay_scale   =RandomF(4,7);
   MaterialPtr overlay_material=Materials.ptrRequire("Decal/bomb.mtrl");

   // add terrain overlay
   Matrix m;
   m.setPosDir (pos, Vec(0,1,0)); // set position and direction
   m.rotateZVec(overlay_angle  ); // rotate along matrix z axis
   m.scaleOrn  (overlay_scale  ); // scale the overlay
   Game::World.terrainAddOverlay(overlay_material, m);

   Memc<Game::Obj*> objects;
   Game::World.objGet(objects, Ball(overlay_scale*SQRT3+1, pos), OBJ_ITEM); // get objects in explosion radius
   REPA(objects)if(Item *item=CAST(Item, objects[i])) // process world items to add overlays and apply forces to actors
   {
      // apply forces to item actors
      Actor &actor=item->actor;
      Vec    dir  =actor.pos()-pos; // get difference between actor position and explosion position
      Flt    dist =dir.normalize(), // normalize direction and store its previous length into 'dist'
             dist2=Sqr(dist);

      if(dist2)actor.addImpulse(30 * dir / dist2); // final equation for impulse = (power) * (normalized direction) / (distance^2)

      // set mesh overlay for item
      if(item->mesh)
      {
         m.orn().setDir    (-dir);
         m.orn().rotateZVec(overlay_angle);
         m.scaleOrn        (overlay_scale);
         if(!item->mesh_overlay.New().createAnimated(*item->mesh, overlay_material, m, &item->matrixScaled()))item->mesh_overlay.removeLast();
      }
   }
}
/******************************************************************************/
// ITEM
/******************************************************************************/
Item::Item()
{
   type=ITEM_NONE;
}
/******************************************************************************/
void Item::create(Game::ObjParams &obj)
{
   super::create(obj);

   if(Param *param=obj.findParam("type"))type=param->asEnum(); // get item type from .obj file
}
/******************************************************************************/
Bool Item::update()
{
   if(super::update())
   {
      if(type==ITEM_MISSILE) // if this is a missile
      {
         Vec move=actor.vel()*(Time.d()*2);
         if(actor.cuts() || actor.sweep(move)) // if the missile actor has encountered an obstacle along the way
         {
            CreateExplosion(pos()); // create explosion at bomb's position
            return false; // return false which will delete the bomb Item
         }
      }

      REPA(mesh_overlay)if(mesh_overlay[i].update()) // update all overlays
         mesh_overlay.remove(i, true); // and remove them if they faded out

      return true;
   }
   return false;
}
/******************************************************************************/
UInt Item::drawPrepare()
{
   UInt modes=super::drawPrepare();
   if(mesh_overlay.elms())modes|=IndexToFlag(RM_OVERLAY); // include request for additional render mode
   return modes;
}
void Item::drawOverlay()
{
   REPAO(mesh_overlay).draw(matrixScaled()); // draw mesh_overlays with the same matrix used for default item drawing
}
/******************************************************************************/
void Item::save(File &f)
{
   super::save(f);

   f<<type;
   mesh_overlay.save(f);
}
Bool Item::load(File &f)
{
   if(super::load(f))
   {
      f>>type;
      return mesh_overlay.load(f);
   }
   return false;
}
/******************************************************************************/
// MAIN
/******************************************************************************/
void InitPre()
{
   App.name("Big Overlays");
   App.flag=APP_FULL_TOGGLE|APP_MS_EXCLUSIVE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true).hpRt(true).ambientPower(0.3f).viewRange(70);

   Cam.at.set(16,0,16);
   Cam.dist = 28;
   Cam.pitch=-PI_6;
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

   CamHandle(0.1f, 100, CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT));

   Game::World.update(Cam.at);

   // update explosions
   REPA(Explosions)if(!Explosions[i].update())Explosions.remove(i);

   if(Kb.bp(KB_SPACE)) // on space pressed
   {
      // create a 'bomb' item
      Game::ObjParams &bomb=*Game::Objs("obj/item/missile/bomb/0.obj");
      Game::World.objCreate(bomb, Matrix().setPosUp(Vec(RandomF(13,19), 14, RandomF(13,19)), Vec(0,-1,0)).scaleOrn(bomb.scale()*2));
   }

   return true;
}
/******************************************************************************/
void Render()
{
   Game::World.draw();

   switch(Renderer())
   {
      case RM_PALETTE:
         REPA(Explosions)Explosions[i].draw(); // draw explosions
      break;
   }
}
void Draw()
{
   Renderer(Render);
   D.text(0, 0.9f, "Press Space to drop a Bomb");
}
/******************************************************************************/
