/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Mesh      box;
Particles particle[4]; // particles
/******************************************************************************/
void InitPre()
{
   App.name("Particles");
   App.flag=APP_FULL_TOGGLE|APP_MS_EXCLUSIVE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true).ambientPower(0);
   Cam.dist=2;
}
/******************************************************************************/
Bool Init()
{
   // create mesh box
   box.parts.New().base.create(Box(4), VTX_TEX0|VTX_NRM|VTX_TAN).reverse();
   box.setMaterial(Materials.ptrRequire("mtrl/brick/0.mtrl")).setRender().setBox();

   // create particles

   // we'll start with a fire-like particle
   {
      ImagePtr image   ="Obj/Particles/Fire/0.gfx";
      Color    color   =Color(138,0,0,0);
      Int      elms    =32;
      Flt      radius  =0.048f,
               life_avg=1.31f;

      particle[0].create(image, color, elms, radius, life_avg) // create particles
                 .source(Ball(0.05f));       // set generation source to a Ball
      particle[0].palette(true);             // enable palette mode
      particle[0].growth_factor=2.013f;      // set growth factor
      particle[0].vel_random=0.084f;         // set random velocity
      particle[0].ang_vel=5.73f;             // set random velocity
      particle[0].accel.set(0,0.447f,0);     // set particles acceleration
      particle[0].matrix.pos.set(-0.5f,0,0); // move source to the left
      particle[0].reset();                   // reset every single particle for random position and other values according to the parameters we've specified above
   }

   // let's set particle[1] with different settings, to make it magic-like
   {
      ImagePtr image   ="Obj/Particles/Ice/0.gfx";
      Color    color   =Color(0,0,40,0);
      Int      elms    =100;
      Flt      radius  =0.05f,
               life_avg=1.0f;

      particle[1].create(image, color, elms, radius, life_avg) // create particles
                 .source(Capsule(0.1f,0.4f)); // set generation source to a Capsule
      particle[1].palette(true);              // enable palette mode
      particle[1].func =PARTICLE_SIN;         // change particles opacity function to sine based
      particle[1].reset();                    // reset every single particle
      
   }

   // and now particle[2], fountain-like
   {
      ImagePtr image   ="Obj/Particles/Water/drop.gfx";
      Color    color   =Color(20,120,200,150); // particle[2] will be rendered in a different way (alpha blended) so make sure to set up alpha value in the color
      Int      elms    =500;
      Flt      radius  =0.02f,
               life_avg=1.0f;

      particle[2].create(image, color, elms, radius, life_avg) // create particles
                 .source(Vec(0,0,0));       // set generation source to a single point
      particle[2].accel.set(0,-2,0);        // enable acceleration to simulate gravity
      particle[2].vel_random=1.0;           // set random   velocity
      particle[2].vel_constant.set(2,2,0);  // set constant velocity vector
      particle[2].damping=0.98f;            // enable damping
      particle[2].matrix.pos.set(0.5f,0,0); // move source to the right
      particle[2].reset();                  // reset every single particle
   }

   // particle[3], smoke-like
   {
      ImagePtr image   ="Obj/Particles/Ice/0.gfx";
      Color    color   =Color(0,0,0,100); // particle[3] will be rendered in a different way (alpha blended) so make sure to set up alpha value in the color
      Int      elms    =40;
      Flt      radius  =0.15f,
               life_avg=1.5f;

      particle[3].create(image, color, elms, radius, life_avg) // create particles
                 .source(Vec(0,0,0));     // set generation source to a single point
      particle[3].vel_random=0.2f;        // set random velocity
      particle[3].matrix.pos.set(-1,0,0); // move source to the left
      particle[3].reset();                // reset every single particle
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
   CamHandle(0.01f, 100, CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT));

   // modify particle[3] position
   Vec2 pos; SinCos(pos.y, pos.x, Time.time()+PI);
   particle[3].matrix.pos.set(pos.x, 0, pos.y);

   // update particles
   REPA(particle)particle[i].update();

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         box.draw(MatrixIdentity);

         LightPoint(30, Vec(0,3,0)).add();
      }break;
   
      // particles are rendered in RM_PALETTE or RM_BLEND modes
      // the first  one (RM_PALETTE) is for objects rendered using color palette, and take their colors from 'Renderer.color_palette' texture (you can assign this texure to your custom version with command "Renderer.color_palette=Images(..)")
      // the second one (RM_BLEND  ) is for objects rendered with standard alpha blending
      // rendering mode detection is done automatically inside Particles::draw method
      case RM_BLEND  :
      case RM_PALETTE:
      {
         REPAO(particle).draw();
      }break;
   }

}
void Draw()
{
   Renderer(Render);
}
/******************************************************************************/
