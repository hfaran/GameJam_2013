/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
void InitPre()
{
   App.name("Tesselation");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");

   Cam.yaw=PI;
   D.full(true).shadowSoft(1).ambientPower(0.4f).hpRt(true).viewRange(5);
}
/******************************************************************************/
Bool Init()
{
   Sun.image="Env/Sky/sun.gfx";
   Sun.pos.z*=-1;
   Sky.atmospheric();
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

   if(Ms.bp(0))D.tesselation(!D.tesselation());

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         Meshes("obj/chr/green alien/0.mesh")->draw(MatrixIdentity);
      }break;

      case RM_SHADOW:
      {
         Meshes("obj/chr/green alien/0.mesh")->drawShadow(MatrixIdentity);
      }break;
   }
}
void Draw()
{
   Renderer.wire^=Kb.bp(KB_TILDE);
   Renderer(Render);
   if(D.shaderModel()<SM_5)D.text(0, 0.9f,   "Tesselation not available (Shader Model 5.0 required)");
   else                    D.text(0, 0.9f, S+"Tesselation: "+D.tesselation()+" (Press LMB to toggle)");
                           D.text(0, 0.8f,   "Press Tilde to toggle Wireframe");
}
/******************************************************************************/
