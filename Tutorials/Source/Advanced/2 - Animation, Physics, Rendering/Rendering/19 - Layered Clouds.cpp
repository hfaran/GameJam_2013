/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
void InitPre()
{
   App.name("Layered Clouds");
   App.flag=APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true);
}
Bool Init()
{
   Sky   .atmospheric();                             // sky
   Sun   .image="Env/Sky/sun.gfx";                   // sun
   Clouds.layered.set(3, Images("Env/Cloud/0.gfx")); // clouds
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

   if(Kb.bp(KB_F11))D.toggle();

   if(!Kb.b(KB_SPACE))Clouds.layered.update(); // if space not pressed then update clouds position

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
   D.text(0, 0.9f, S+"Fps "+Time.fps());
}
/******************************************************************************/
