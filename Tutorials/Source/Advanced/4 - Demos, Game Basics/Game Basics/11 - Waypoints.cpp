/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/_enums.h"
/******************************************************************************/
void InitPre()
{
   App.name("Waypoints");
   App.flag=APP_FULL_TOGGLE|APP_MS_EXCLUSIVE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true).hpRt(true).viewRange(50);

   Cam.at.set(16,0,16);
   Cam.dist = 20;
   Cam.pitch=-PI_3;
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.scale*=0.8f;

   Physics.create(CSS_NONE, true, "../Installation/PhysX");
   Sky    .atmospheric();
   Sun.image="Env/Sky/sun.gfx"; Sun.light_color=1-D.ambientColor();

   Game::World.init(                       )
              .New ("world/waypoints.world");

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

   return true;
}
/******************************************************************************/
void Render()
{
   Game::World.draw();
}
void DrawWaypoints(Str name)
{
   if(Game::Waypoint *waypoint=Game::World.findWaypoint(name)) // if waypoint exists
   {
              waypoint->draw(             ); // draw waypoint
      Vec pos=waypoint->pos (Time.time()*2); // access waypoints position at 'Time.time()*2' length
          pos.draw(RED);                     // draw the position as red dot
   }
}
void Draw()
{
   Renderer(Render);

   // draw waypoints
   // 3 waypoints are stored in World used in this tutorial, they're named as follow: "0", "1", "2"
   SetMatrix(); // first reset matrix
   REP(3)DrawWaypoints(S+i); // draw each waypoint

   D.text(0, 0.9f, "Notice that different Waypoints have different looping modes");
   D.text(0, 0.8f, "Waypoints with their looping modes are set in World Editor");
}
/******************************************************************************/
