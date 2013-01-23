/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/_enums.h"
/******************************************************************************/
Game::ObjMemx<Game::Chr   > Chrs   ;
Game::ObjMemx<Game::Static> Statics;

Decal decal; // decal pointing character destination target
/******************************************************************************/
void InitPre()
{
   App.name("Pathfind");
   App.flag=APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true).hpRt(true).viewRange(50);

   Cam.dist = 10;
   Cam.yaw  =-PI_4;
   Cam.pitch=-PI_3;
}
/******************************************************************************/
Bool Init()
{
   Physics.create(CSS_NONE, true, "../Installation/PhysX");
   Sky    .atmospheric();
   Sun.image="Env/Sky/sun.gfx"; Sun.light_color=1-D.ambientColor();

   decal.terrain_only=true;
   decal.color.set(1,1,0,1);
   decal.material(Materials.ptrRequire("Decal/star.mtrl"));

   Game::World.init      (                   )
              .setObjType(Chrs   , OBJ_PLAYER)
              .setObjType(Statics, OBJ_STATIC)
              .New       ("world/path.world" )
              .update    (Cam.at             );

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

   Game::World.update(Cam.at);

   // move player
   if(Ms.bp(0) && Chrs.elms()) // on LMB pressed
   {
      Vec     pos, dir; ScreenToPosDir(Ms.pos(), pos, dir); // convert screen mouse position to world position and direction
      PhysHit phys_hit;
      if(Physics.ray(pos, dir*D.viewRange(), &phys_hit)) // if ray-test hit something
      {
         Chrs[0].actionMoveTo(phys_hit.plane.pos); // order character to move to that location

         decal.matrix.setPosDir(phys_hit.plane.pos, Vec(0,1,0));
      }
   }

   // rotate camera
   if(Ms.b(1))
   {
      Cam.yaw  -=Ms.d().x;
      Cam.pitch+=Ms.d().y;
   }
   if(Chrs.elms())Cam.setSpherical(Chrs[0].pos(), Cam.yaw, Cam.pitch, 0, Cam.dist*ScaleFactor(Ms.wheel()*-0.2f)).updateVelocities().set();

   // rotate decal around its z axis
   decal.matrix.rotateZVec(Time.d());

   return true;
}
/******************************************************************************/
void Render()
{
   Game::World.draw();
   
   switch(Renderer())
   {
      case RM_BLEND:
      {
         if(Chrs.elms())
            if(Chrs[0].action==Game::ACTION_MOVE_TO)decal.drawStatic();
      }break;
   }
}
void Draw()
{
   Renderer(Render);

   // show world path
   if(Kb.b(KB_SPACE))Game::World.path().draw(64, 0.5f); // if space pressed

   // show player path
   if(Chrs.elms())
   {
      Memc<Vec> &path=Chrs[0].path; if(path.elms())
      {
         D.clearZ();
         SetMatrix();
         VI.color(YELLOW); REPA(path         )VI.dot (path[i]           ); VI.end();
         VI.color(YELLOW); REP (path.elms()-1)VI.line(path[i], path[i+1]); VI.end();
      }
   }

   // informations
   D.text(0, 0.9f, "Press LMB to move player, RMB to rotate camera");
   D.text(0, 0.8f, "Press Space to show World Path Meshes");
}
/******************************************************************************/
