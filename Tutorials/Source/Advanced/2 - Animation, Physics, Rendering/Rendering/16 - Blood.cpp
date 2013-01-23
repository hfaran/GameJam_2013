/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
// BLOOD HEADER
/******************************************************************************/
struct BloodFx // Blood Effect
{
   Particles main, single;

   // manage
   BloodFx& del   ();
   BloodFx& create(ImagePtr main, ImagePtr single, Color color, Int elms, C Vec &pos, C Vec &vel, Flt size);

   // update/draw
   Bool update(); // update, returns false when effect faded out and is inactive, true when still active
   void draw  (); // draw  , can be called in following render modes : RM_BLEND, RM_PALETTE

   // io
   void save(File &f); // save binary to   file
   Bool load(File &f); // load binary from file, false on fail
};
/******************************************************************************/
// BLOOD SOURCE
/******************************************************************************/
BloodFx& BloodFx::del()
{
   main  .del();
   single.del();
   return T;
}
BloodFx& BloodFx::create(ImagePtr main, ImagePtr single, Color color, Int elms, C Vec &pos, C Vec &vel, Flt size)
{
   del();

   Vec  dir   =vel;
   Flt  length=dir.normalize();
   Ball ball(0.03f*size, pos);

   if(main)
   {
      T.main.create(main, color, Max(1, elms/10), 0, 0);
      T.main.accel.set(0,-1.5f,0);
      T.main.radius_scale_base=0; // start with zero radius multiplier
      T.main.radius_scale_time=1; // set the change ratio of the radius scale
      T.main.life_total=0.70f;
      T.main.fade_in   =0.02f;
      T.main.fade_out  =0.35f;
      T.main.damping   =0.30f;
      T.main.matrix    =pos; // set position in case the user will try to access 'Particles' position

      REPA(T.main)
      {
         Particle &p=T.main.p[i];
         p.life_max=T.main.life_total;
         p.radius  =RandomF(0.18f, 0.3f)*size;
         p.pos     =Random (ball);
         p.vel     =Random.dir(dir, 0.5f)*(length*RandomF(0.25f, 0.75f));
      }
   }
   if(single)
   {
      T.single.create(single, color, elms, 0, 0);
      T.single.accel.set(0, -1.5f, 0);
      T.single.radius_scale_base=0; // start with zero radius multiplier
      T.single.radius_scale_time=1; // set the change ratio of the radius scale
      T.single.life_total=0.70f;
      T.single.fade_in   =0.02f;
      T.single.fade_out  =0.20f;
      T.single.damping   =0.30f;
      T.single.matrix    =pos; // set position in case the user will try to access 'Particles' position

      REPA(T.single)
      {
         Particle &p=T.single.p[i];
         p.life_max=T.single.life_total;
         p.radius  =RandomF(0.015f, 0.06f)*size;
         p.pos     =Random (ball);
         p.vel     =Random.dir(dir, 0.5f)*(length*RandomF(0.37f, 1));
      }
   }
   return T;
}
/******************************************************************************/
Bool BloodFx::update()
{
   Bool   main_alive=main  .update(),
        single_alive=single.update();
   return main_alive || single_alive;
}
void BloodFx::draw()
{
   main  .draw();
   single.draw();
}
/******************************************************************************/
void BloodFx::save(File &f)
{
   main  .save(f, true);
   single.save(f, true);
}
Bool BloodFx::load(File &f)
{
   return main.load(f)
     && single.load(f);
}
/******************************************************************************/
// MAIN
/******************************************************************************/
Mesh   *chr   ; // character mesh
Image  *main  , // main   blood texture
       *single; // single blood texture
BloodFx blood ; // blood particle effect
/******************************************************************************/
void InitPre()
{
   App.name("Blood Effect");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak", NULL);

   Cam.dist=3;
   D.full(1).ambientPower(0.1f).hpRt(true);
}
/******************************************************************************/
Bool Init()
{
   Sky.atmospheric();

   chr=Meshes("obj/chr/human/0.mesh");

   main  =Images("Obj/Particles/Blood/0.gfx");
   single=Images("Obj/Particles/Blood/1.gfx");

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
   CamHandle(0.1f, 100, CAMH_ZOOM|(Ms.b(1) ? CAMH_MOVE : CAMH_ROT));

   // enable blood effect
   if(Kb.bp(KB_SPACE)) // on space
      blood.create(main, single, Color(128,0,0), 60, Vec(0,1,0), Vec(1.6f,0,0), 1); // create blood effect

   // update blood effect
   blood.update();

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         chr->draw(Matrix(2).rotateY(PI)); // draw character mesh

         LightPoint(5, Vec(0,1,-1)).add();
      }break;

      case RM_BLEND  :
      case RM_PALETTE:
      {
         blood.draw(); // draw Blood Effect in the same modes as Particles are drawn
      }break;
   }
}
void Draw()
{
   Renderer(Render);
   D.text(0, 0.9f, "Press Space to enable Blood Effect");
}
/******************************************************************************/
