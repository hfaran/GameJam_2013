/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Actor ground,
      ball  ,
      actor ;
/******************************************************************************/
void InitPre()
{
   App.name("Multi shaped actors");
   App.flag=APP_MS_EXCLUSIVE;
   Paks.add("../data/engine.pak");
}
Bool Init()
{
   Cam.dist=4;

   Physics.create(CSS_NONE, true, "../Installation/PhysX");
   ground .create(Box (15, 1, 15, Vec(0, -2, 0)), 0);
   ball   .create(Ball(0.3f, Vec(0, 1.3f, 0)));

   ActorShapes shapes;                             // actor shapes
   shapes.add(Box (0.2f                        )); // add box  to 'shapes'
   shapes.add(Ball(0.2f,    Vec(-0.3f, 0.2f, 0))); // add ball to 'shapes'
   shapes.add(Ball(0.2f,    Vec( 0.3f, 0.2f, 0))); // add ball to 'shapes'
   actor.create(shapes).pos(Vec( 0.0f, 0.3f, 0) ); // create actor from 'shapes'

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
   CamHandle(0.1f, 10, CAMH_ZOOM|CAMH_ROT);

   Physics.startSimulation().stopSimulation();

   return true;
}
/******************************************************************************/
void Draw()
{
   D      .clear();
   Physics.draw ();
}
/******************************************************************************/
