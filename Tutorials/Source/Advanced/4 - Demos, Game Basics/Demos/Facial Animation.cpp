/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
CSkeleton cskel;
Mesh     *mesh;

Window   window;
Text     text  [5];
Slider   slider[4];
CheckBox blink;
/******************************************************************************/
void InitPre()
{
   App.name("Facial Animation");
   App.flag=APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true).ambientPower(0.2f).shadowSoft(1).hpRt(true).viewRange(8);
}
/******************************************************************************/
Bool Init()
{
   // camera
   Cam.dist=0.4;
   Cam.at.set(0, 0.75f, 0);
   Cam.yaw=PI;
   Cam.setSpherical().set();

   // sun
   Sun.image="Env/Sky/sun.gfx";
   Sun.pos=!Vec(1,1,3);
   Sun.light_color=1-D.ambientColor();

   // sky
   Sky.atmospheric();

   // character
   mesh =          Meshes("obj/chr/human/0.mesh");
   cskel.create(Skeletons("obj/chr/human/0.skel"), 1.8f);

   // gui
   Gui   +=window .create(Rect_R(D.w(), 0, 0.49f, 0.50f),"Expressions");
   window+=text[0].create(Vec2(0.13f, -0.05f), "Angry"     ); window+=slider[0].create(Rect_L(0.26f, -0.05f, 0.2f , 0.05f));
   window+=text[1].create(Vec2(0.13f, -0.13f), "Astonished"); window+=slider[1].create(Rect_L(0.26f, -0.13f, 0.2f , 0.05f));
   window+=text[2].create(Vec2(0.13f, -0.21f), "Sad"       ); window+=slider[2].create(Rect_L(0.26f, -0.21f, 0.2f , 0.05f));
   window+=text[3].create(Vec2(0.13f, -0.29f), "Smile"     ); window+=slider[3].create(Rect_L(0.26f, -0.29f, 0.2f , 0.05f));
   window+=text[4].create(Vec2(0.13f, -0.37f), "Blink"     ); window+=blink    .create(Rect_L(0.26f, -0.37f, 0.05f, 0.05f), true);

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
   Gui.update();

   // animate
   Flt time=Time.time();
   cskel.clear  ()
        .animate(L"anim/face/angry.anim"     , time, slider[0]())
        .animate(L"anim/face/astonished.anim", time, slider[1]())
        .animate(L"anim/face/sad.anim"       , time, slider[2]())
        .animate(L"anim/face/smile.anim"     , time, slider[3]());

   if(blink())
   {
      Flt blend=Frac(time, 3); // blink every      3   seconds
          blend/=0.1f;         // blink duration = 0.1 seconds
      if( blend>1)blend=2-blend;
      cskel.animate(L"anim/face/blink.anim", time, blend, true);
   }

   cskel.updateMatrix(MatrixIdentity)
        .updateVelocities();

   if(!Gui.ms() || Gui.ms()==Gui.desktop())CamHandle(0.01f, 10, CAMH_ZOOM | (Ms.b(0)?CAMH_ROT:0));

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE: mesh->draw      (cskel); break;
      case RM_SHADOW : mesh->drawShadow(cskel); break;
   }
}
void Draw()
{
   Renderer(Render);
   Gui.draw();
}
/******************************************************************************/
