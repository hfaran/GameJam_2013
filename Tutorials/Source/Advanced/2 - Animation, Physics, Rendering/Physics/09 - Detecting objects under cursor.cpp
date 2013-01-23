/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
enum ACTOR_GROUPS // actor groups (for example in the game you can specify following groups: door, item, character, ..)
{
   GROUP_BACKGROUND, // background
   GROUP_OBJ       , // objects
};
/******************************************************************************/
Actor ground,
      obj[10];
/******************************************************************************/
void InitPre()
{
   App.name("Detecting objects under cursor");
   Paks.add("../data/engine.pak");
}
Bool Init()
{
   Cam.setSpherical(Vec(0,0,-2), 0, -0.7f, 0, 6).set(); // set camera position and activate it

   Physics.create(CSS_NONE,true,"../Installation/PhysX");

   ground.create(Box(15,1,15,Vec(0,-2,0)), 0).group(GROUP_BACKGROUND); // create ground and set its group to background

   // create random actors
   REPA(obj)
   {
      switch(Random(3))
      {
         case 0: obj[i].create(Box    (RandomF(0.1f, 0.5f),                      Random(Box(10, 1, 10)))); break;
         case 1: obj[i].create(Ball   (RandomF(0.1f, 0.5f),                      Random(Box(10, 1, 10)))); break;
         case 2: obj[i].create(Capsule(RandomF(0.1f, 0.2f), RandomF(0.5f, 1.0f), Random(Box(10, 1, 10)))); break;
      }
      
      obj[i].group(GROUP_OBJ).user(Ptr(i)); // set actor's group to 'object' group and set its user data, in this tutorial it'll be index of the object in the array
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

   Physics.startSimulation().stopSimulation();

   return true;
}
/******************************************************************************/
void Draw()
{
   D      .clear();
   Physics.draw ();

   // we'll now test if mouse points on some actor
   // to do that we'll perform a standard ray testing
   // with initial ray 'start' point located near the camera, and 'end' point located as far as viewport range reaches
   {
      // to obtain 'start' and 'end' point we'll use a special function called 'ScreenToPosDir' which transforms a screen position (Vec2) to world space position and direction
      Vec screen_pos,
          screen_dir;
      ScreenToPosDir(Ms.pos(), screen_pos, screen_dir); // obtain 'screen_pos' and 'screen_dir' from Mouse cursor position

      // now having world space position and direction we can calculate ray 'start' and 'end' positions
      Vec start=screen_pos,
          end  =screen_pos + screen_dir*D.viewRange();

      // having ray positions we can perform a ray test
      PhysHit phys_hit;
      if(Physics.ray(start, end-start, &phys_hit)) // if ray hit something
      {
         D.text(0, 0.9f,   "Ray has hit an actor");
         D.text(0, 0.8f, S+"Actor's group: "            +         phys_hit.group);
         D.text(0, 0.7f, S+"Actor's user data (index): "+(UIntPtr)phys_hit.user );
      }else
      {
         D.text(0, 0.9f, "Ray hasn't hit anything");
      }
   }
}
/******************************************************************************/
