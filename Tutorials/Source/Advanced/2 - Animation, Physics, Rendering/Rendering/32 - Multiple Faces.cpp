/******************************************************************************/
#include "stdafx.h"
/******************************************************************************

   This tutorial will present how to render body mesh with multiple faces,
      where each face uses its own skeleton for facial animation.

   Details:
      -Body mesh and skeleton DO NOT need facial bones (only Face mesh and skeleton need them)
      -Face mesh and skeleton DO NOT need body   bones (only Body mesh and skeleton need them)
      -Multiple faces can have totally different skeletons (different amount of bones and names)
      -Body and Face skeletons must have 1 shared bone located at the same original position (in bind pose)
      -The shared bone can be for example "Head" bone

   Required Bones:
      Body Skeleton - shared "Head" bone, body bones (arms, legs, ..)
      Face Skeleton - shared "Head" bone, face bones (lips, eyes, ..)

   How this works:
      -We will first fully animate the body skeleton
      -We will get the final transformation matrix of the shared "Head" bone in the body skeleton
      -Basing on that transformation we will animate the face skeleton

/******************************************************************************/
struct Player
{
   MeshPtr   mesh_body, // body mesh
             mesh_face; // face mesh
   CSkeleton skel_body, // body skeleton
             skel_face; // face skeleton
   Int       body_head_index, // index of "head" bone in body skeleton
             face_head_index; // index of "head" bone in face skeleton

   // update
   void update()
   {
      // calculate animation parameters
      Flt time =Time.time(),
          blink=Frac(time, 1); // blink every      1   second
          blink/=0.1f;         // blink duration = 0.1 seconds
      if( blink>1)blink=2-blink;
   
      // fully animate body skeleton
      skel_body.clear()
           .animate(L"anim/fist-l.anim", time)
           .animate(L"anim/fist-r.anim", time)
           .animate(L"anim/walk.anim"  , time/5)
           .updateMatrix(MatrixIdentity)
           .updateVelocities();

      // get body head transformation
      Matrix head_matrix=skel_body.bone(body_head_index).matrix();

      // animate face skeleton
      skel_face.clear() // prepare for animations
               .animate(L"anim/face/blink.anim", time, blink, true) // animate using prepared animations
               .forceMatrix(face_head_index, head_matrix, false) // force "face head" bone transformation to be the same as "body head"
               .updateMatrix(MatrixIdentity) // build all transformation matrixes
               .updateVelocities(); // update velocities
   }

   // draw
   void drawPrepare()
   {
      if(mesh_body)mesh_body->draw(skel_body);
      if(mesh_face)mesh_face->draw(skel_face);
   }
   void drawShadow()
   {
      if(mesh_body)mesh_body->drawShadow(skel_body);
      if(mesh_face)mesh_face->drawShadow(skel_face);
   }

   void setFace(Int face)
   {
      mesh_face=S+"obj/chr/human/face "+face+".mesh";
      skel_face.create(mesh_face->skeleton());
      face_head_index=skel_face.getBoneI("head"); // get index of "head" bone in face skeleton
   }
   void createDefault()
   {
      // set mesh and skeleton
      mesh_body       =          "obj/chr/human/0.mesh"  ; mesh_body->hide("head"); // set mesh and hide any "head" parts if present
      skel_body.create(Skeletons("obj/chr/human/0.skel"));
      body_head_index=skel_body.getBoneI("head"); // get index of "head" bone in body skeleton

      setFace(0);
   }

   Player()
   {
      body_head_index=-1;
      face_head_index=-1;
   }
};
/******************************************************************************/
Player player;
/******************************************************************************/
void InitPre()
{
   App.name("Multiple Faces");
   App.flag=APP_FULL_TOGGLE|APP_MS_EXCLUSIVE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true).ambientPower(0.3f).hpRt(true)
    .shadowJitter(true).shadowSoft(1).shadowMapSize(2048)
    .viewFrom(0.005f).viewRange(5);

   Cam.yaw =PI;
   Cam.dist=0.3f;
   Cam.at.set(0,0.4f,0);
}
/******************************************************************************/
Bool Init()
{
   // initialize sky & sun
   Sky.atmospheric();
   Sun.image="Env/Sky/sun.gfx";
   Sun.pos=!Vec(1,1,1);
   Sun.light_color=1-D.ambientColor();

   // setup player data
   player.createDefault();

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
   CamHandle(0.01f, 100, CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT));

   // change faces when keys pressed
   if(Kb.bp(KB_1))player.setFace(0);
   if(Kb.bp(KB_2))player.setFace(1);

   player.update();

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE: player.drawPrepare(); break;
      case RM_SHADOW : player.drawShadow (); break;
   }
}
void Draw()
{
   Renderer(Render);
   D.text(0, 0.9f, "Press 1,2 keys to change faces");
}
/******************************************************************************/
