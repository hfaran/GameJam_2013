/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Flt   angle   ; // car wheel angle
Actor car     , // car    actor
      ground  ; // ground actor
Wheel wheel[4]; // wheels
/******************************************************************************/
void InitPre()
{
   App.name("Vehicle");
   App.flag=APP_FULL_TOGGLE;
   Paks.add("../data/engine.pak");

   D.mode(800,600).viewRange(200);
}
Bool Init()
{
   Cam.dist=10;
   Physics.create(CSS_NONE, true, "../Installation/PhysX");
   ground .create(Box_U(100,1,100), 0);
   ground .pos   (ground.pos()-Vec(0,3,0));

   // create car
   car.create     (Box(2,1,4));                      // create actor
   car.massCenterL(car.massCenterL()-Vec(0,0.8f,0)); // lower mass center

   // create car wheels
   Wheel::Param wp; // wheel parameters
   wheel[0].create(car, Matrix().setPos(Vec( 1, -0.5f,  1.5f)), wp); // left -front
   wheel[1].create(car, Matrix().setPos(Vec(-1, -0.5f,  1.5f)), wp); // right-front
   wheel[2].create(car, Matrix().setPos(Vec( 1, -0.5f, -1.5f)), wp); // left -rear
   wheel[3].create(car, Matrix().setPos(Vec(-1, -0.5f, -1.5f)), wp); // right-rear

   return true;
}
void Shut()
{
}
/******************************************************************************/
Bool Update()
{
   if(Kb.bp(KB_ESC))return false;
   CamHandle(0.1f, 200, CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT));

   Physics.startSimulation().stopSimulation();

   if(Kb.bp(KB_ENTER))car.pos(Vec(0,3,0)); // reset car position on ENTER key

   // adjust car controls
   {
      AdjustValTime(angle, (Kb.b(KB_D)-Kb.b(KB_A))*PI_4, 0.01f); // adjust wheel angle

      Flt accel=(Kb.b(KB_W)-Kb.b(KB_S))*1600, // acceleration
          brake= Kb.b(KB_SPACE)        *3200; // brakes

      wheel[0].angle(angle).accel(accel).brake(brake); // set angle, acceleration and brakes to left -front wheel
      wheel[1].angle(angle).accel(accel).brake(brake); // set angle, acceleration and brakes to right-front wheel
   }

   return true;
}
void Draw()
{
   D      .clear();
   Physics.draw ();

   car.massCenterW().draw(RED); // draw mass center
}
/******************************************************************************/
