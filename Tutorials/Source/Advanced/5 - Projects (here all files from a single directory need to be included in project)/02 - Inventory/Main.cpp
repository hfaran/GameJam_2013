/******************************************************************************/
#include "stdafx.h"
#include "main.h"
/******************************************************************************/
Game::Obj            *Lit    ; // highlighted world object
Game::ObjMemx<Item  > Items  ;
Game::ObjMemx<Player> Players;
/******************************************************************************/
void InitPre()
{
   App.name("Inventory");
   App.flag=APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true).hpRt(true).viewRange(70);

   Cam.at.set(16,0,16);
   Cam.dist = 10;
   Cam.pitch=-PI_6;
}
/******************************************************************************/
Bool Init()
{
   Physics.create(CSS_NONE, true, "../Installation/PhysX");
   Sky    .atmospheric();
   Sun.image="Env/Sky/sun.gfx";

   Game::World.init      (                   )
              .setObjType(Players, OBJ_PLAYER)
              .setObjType(Items  , OBJ_ITEM  )
              .New       ("world/sample.world");

   InvGui.create();

   // modify default gui visuals
   Gui.style_window.blur_color.set(102,34,34,255);
   Gui.style_window.back_color.set(204,68, 0,255);

   Gui.style_region.back_color.set(0,0,0,102);

   Gui.kb_lit.zero();

   return true;
}
/******************************************************************************/
void Shut()
{
}
/******************************************************************************/
void GetWorldObjectUnderCursor()
{
   Lit=NULL;
   if(!Gui.ms() || Gui.ms()==Gui.desktop())
   {
      Vec     pos, dir; ScreenToPosDir(Ms.pos(), pos, dir);
      PhysHit phys_hit; if(Physics.ray(pos, dir*D.viewRange(), &phys_hit, ~IndexToFlag(AG_CONTROLLER)))Lit=phys_hit.obj;
   }
}
/******************************************************************************/
Bool Update()
{
   if(Kb.bp(KB_ESC))return false;

   if(Players.elms())Cam.at=Players[0].pos();
   CamHandle(0.1f, 100, CAMH_ZOOM|(Ms.b(1)?CAMH_ROT:0));

   if(Kb.bp(KB_F2))Game::World.save("LocalData/save.sav");
   if(Kb.bp(KB_F3))Game::World.load("LocalData/save.sav");

   Game::World.update(Cam.at);
   Gui.update();
   GetWorldObjectUnderCursor();

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
   Gui   .draw();
   InvGui.draw();
}
/******************************************************************************/
