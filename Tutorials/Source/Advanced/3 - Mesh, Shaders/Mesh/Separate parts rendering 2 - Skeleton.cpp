/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Mesh     *mesh;
Skeleton *skel;

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
   CamHandle(0.01f, 10, CAMH_ZOOM|(Ms.b(1) ? CAMH_MOVE : CAMH_ROT));

    rifle_angle+=(Kb.b(KB_S)-Kb.b(KB_W))*Time.d(); Clamp(rifle_angle, -0.5f, 0.1f);
   turret_angle+=(Kb.b(KB_E)-Kb.b(KB_Q))*Time.d();

   return true;
}
/******************************************************************************/
void DrawTank(C Matrix &world_matrix)
{
   // access skeleton bones
   OrientP &rifle =skel->getBone("rifle" );
   OrientP &turret=skel->getBone("turret");

   // set local transformations
   Matrix  rifle_transform;  rifle_transform.setTransformAtPos(rifle .pos, Matrix3().setRotateX( rifle_angle));
   Matrix turret_transform; turret_transform.setTransformAtPos(turret.pos, Matrix3().setRotateY(turret_angle));

   // convert local transformations to global matrixes (which is equal to 'local transformation' * 'parent matrix')
   Matrix turret_matrix=turret_transform* world_matrix,
           rifle_matrix= rifle_transform*turret_matrix;

   // draw the mesh parts separately
   mesh->hideAll().show("rifle" ).draw( rifle_matrix);
   mesh->hideAll().show("turret").draw(turret_matrix);
   mesh->hideAll().show("hull"  ).draw( world_matrix);
   mesh->showAll();
}
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         DrawTank(MatrixIdentity);

         LightDir(Cam.matrix.z).add();
      }break;
   }
}
void Draw()
{
   Renderer(Render);
   D.text(0, 0.9f, S+ "Rifle Angle : "+ rifle_angle+" (W/S to change)");
   D.text(0, 0.8f, S+"Turret Angle : "+turret_angle+" (Q/E to change)");
}
/******************************************************************************/
