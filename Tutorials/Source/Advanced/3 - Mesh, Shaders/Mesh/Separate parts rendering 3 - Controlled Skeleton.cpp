/******************************************************************************/
#include "stdafx.h"
/******************************************************************************

   In this tutorial we'll render separate parts with matrixes automatically calculated by Controlled Skeleton

   CSkeleton will handle transforming child matrixes by parent matrixes

   All we need to do is:
      1. Create CSkeleton on the base of the object's Skeleton
      2. Animate the CSkeleton
      3. Render the parts using CSkeleton bone matrixes

   As for animating the CSkeleton we'll use CSkelBone's relative rotation - CSkelBone::rot which is an 'AxisRoll' object

   AxisRoll contains of:
      struct AxisRoll
      {
         Vec axis;
         Flt roll;
      };
      Where 'axis' direction determines the axis of rotation
            'axis' length    determines how much we'll rotate the object around 'axis' direction
            'roll' is an additional parameter, which determines the angle of rotation around original bone direction

   AxisRoll members when calling CSkeleton::clear() are reset {axis=Vec(0,0,0); roll=0;}

   After reseting the members we'll adjust their values according to our desired angles

/******************************************************************************/
 Mesh     *mesh;
 Skeleton *skel;
CSkeleton cskel;

Flt  rifle_angle=0,
    turret_angle=0;
/******************************************************************************/
void InitPre()
{
   App.name("Separate parts rendering");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.mode(800,600).ambientPower(0.5f);
   Cam.dist=3;
}
/******************************************************************************/
Bool Init()
{
   mesh=Meshes   ("Obj/vehicle/tank/0.mesh");
   skel=Skeletons("Obj/vehicle/tank/0.skel");

   cskel.create(skel); // create a controlled skeleton

   return true;
}
/******************************************************************************/
void Shut()
{
}
/******************************************************************************/
void AnimateTank(C Matrix &world_matrix) // animate the tank skeleton
{
   cskel.clear(); // reset the rotation values
   cskel.getBone("rifle" ).rot.axis.x+= rifle_angle; // adjust rotation in X axis                        by  'rifle_angle'
   cskel.getBone("turret").rot.roll  +=turret_angle; // adjust rotation around bone's original direction by 'turret_angle'
   cskel.updateMatrix(world_matrix); // calculate the final matrixes
}
/******************************************************************************/
Bool Update()
{
   if(Kb.bp(KB_ESC))return false;
   CamHandle(0.01f, 10, CAMH_ZOOM|(Ms.b(1) ? CAMH_MOVE : CAMH_ROT));

    rifle_angle+=(Kb.b(KB_S)-Kb.b(KB_W))*Time.d(); Clamp(rifle_angle, -0.5f, 0.1f);
   turret_angle+=(Kb.b(KB_E)-Kb.b(KB_Q))*Time.d();

   AnimateTank(MatrixIdentity);

   return true;
}
/******************************************************************************/
void DrawTank()
{
   // draw the mesh parts separately
   // use calculated skeleton bone transformation matrixes
   mesh->hideAll().show("rifle" ).draw(cskel.getBone("rifle" ).matrix());
   mesh->hideAll().show("turret").draw(cskel.getBone("turret").matrix());
   mesh->hideAll().show("hull"  ).draw(cskel.                  matrix());
   mesh->showAll();
}
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         DrawTank();
         LightDir(Cam.matrix.z).add();
      }break;
   }
}
/******************************************************************************/
void Draw()
{
   Renderer(Render);

   D.text(0, 0.9f, S+ "Rifle Angle : "+ rifle_angle+" (W/S to change)");
   D.text(0, 0.8f, S+"Turret Angle : "+turret_angle+" (Q/E to change)");
}
/******************************************************************************/
