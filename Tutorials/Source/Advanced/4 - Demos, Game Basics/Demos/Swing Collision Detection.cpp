/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
CSkeleton  cskel;
Mesh      *human, *weapon;
Vec        weapon_start, weapon_end;
Matrix     weapon_matrix;
Tri        weapon_sweep[2];
Memc<Byte> collision;
/******************************************************************************/
void InitPre()
{
   App.name("Collision Detection");
   App.flag=APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true).ambientPower(0.2f).hpRt(true).viewRange(15);
}
/******************************************************************************/
Bool Init()
{
   // camera
   Cam.dist=3;
   Cam.yaw=PI;
   Cam.pitch=-0.4f;
   Cam.setSpherical().set();

   // sun
   Sun.image="Env/Sky/sun.gfx";
   Sun.pos  =!Vec(1,1,3);
   Sun.light_color=1-D.ambientColor();

   // sky
   Sky.atmospheric();

   // character
   human=          Meshes("obj/chr/human/0.mesh");
   cskel.create(Skeletons("obj/chr/human/0.skel"), 1.8f);

   // weapon
   weapon=Meshes("obj/item/weapon/blunt/club/0.mesh");

   return true;
}
/******************************************************************************/
void Shut()
{
}
/******************************************************************************/
void DetectCollision()
{
   collision.clear();

   // calculate new weapon start/end positions
   Vec weapon_start_new=weapon->box.down()*weapon_matrix,
       weapon_end_new  =weapon->box.up  ()*weapon_matrix;

   // construct 2 triangles covering the swept area in current frame
   weapon_sweep[0].set(weapon_start  , weapon_end      , weapon_end_new);
   weapon_sweep[1].set(weapon_end_new, weapon_start_new, weapon_start  );

   if(cskel.skeleton())
      REPA(cskel.skeleton()->bones)
         if(cskel.skeleton()->bones[i].flag&BONE_RAGDOLL)
   {
      Shape shape =ShapeBone(cskel.skeleton()->bones[i]);
            shape*=cskel.bone(i).matrix();

      Bool cuts=false;
      switch(shape.type) // bone can be either a ball or a capsule
      {
         case SHAPE_BALL   : cuts=(Cuts(weapon_sweep[0], shape.ball   ) || Cuts(weapon_sweep[1], shape.ball   )); break;
         case SHAPE_CAPSULE: cuts=(Cuts(weapon_sweep[0], shape.capsule) || Cuts(weapon_sweep[1], shape.capsule)); break;
      }
      if(cuts)collision.add(i); // if collision occurs then add 'i-th' bone to the collision container
   }

   // update weapon positions
   weapon_start=weapon_start_new;
   weapon_end  =weapon_end_new;
}
Bool Update()
{
   if(Kb.bp(KB_ESC))return false;
   Gui.update();

   Flt time=Time.time();

   // animate character
   cskel.clear  ()
        .animate(L"anim/walk.anim", time/2)
        .updateMatrix(MatrixIdentity)
        .updateVelocities();

   // update weapon motion
   weapon_matrix.setPosUp(Vec(0,0,0.5f), Vec(0,0,1))
                .rotateY (time)
                .move    (Vec(0,0.3f,1.2f));

   DetectCollision();

   // camera
   CamHandle(0.01f, 10, CAMH_ZOOM|CAMH_ROT);

   if(Kb.b(KB_SPACE))Time.wait(250); // slow down on space pressed

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         human ->draw(cskel);
         weapon->draw(weapon_matrix);
      }break;
   }
}
void Draw()
{
   Renderer(Render);

   Str text="Collision on:";
   if(cskel.skeleton())
      REPA(cskel.skeleton()->bones)
         if(cskel.skeleton()->bones[i].flag&BONE_RAGDOLL)
   {
      Shape shape =ShapeBone(cskel.skeleton()->bones[i]);
            shape*=cskel.bone(i).matrix();
            shape.draw(ColorAlpha(collision.has(i) ? RED : WHITE, 0.5f));
      if(collision.has(i))text+=S+cskel.skeleton()->bones[i].name+", ";
   }

   REPAO(weapon_sweep).draw(GREEN);

   D.text(0, 0.9f, "Press space to slow down tutorial and visualize 'weapon_sweep' triangles");
   D.text(0, 0.8f, text);
}
/******************************************************************************/
