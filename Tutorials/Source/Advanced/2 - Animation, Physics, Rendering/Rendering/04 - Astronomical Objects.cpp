/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
void InitPre()
{
   App.name("Astronomical Objects");
   App.flag=APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true).hpRt(true);

   Cam.pitch=0.5f;
}
Bool Init()
{
   Sky.atmospheric(); // sky

   // sun
   {
      Sun.image="Env/Sky/sun.gfx"; // set image
      Sun.pos=!Vec(1,1,1);         // set custom position
   }

   // moon
   {
      Astro &moon=Astros.New();         // create a new astronomical object in 'Astros' container
      moon.image="Env/Sky/moon.gfx";    // set image
      moon.pos =!Vec(-1,1,1);           // set custom position
      moon.size*=0.6f;                  // decrease default size
      moon.blend=false;                 // disable blending, and thus set adding mode
      moon.image_color.set(64,64,64,0); // set image color
      moon.light_color.zero();          // disable light casting
   }

   return true;
}
void Shut()
{
}
/******************************************************************************/
Bool Update()
{
   if(Kb.bp(KB_ESC))return false;
   CamHandle(0.01f, 500, CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT));

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE: Meshes("obj/terrain/0.mesh")->draw(MatrixIdentity); break; // draw terrain
   }
}
void Draw()
{
   Renderer(Render);
}
/******************************************************************************/
