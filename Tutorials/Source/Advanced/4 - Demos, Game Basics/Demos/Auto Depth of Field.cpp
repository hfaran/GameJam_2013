/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Bool ray_hit    ; // if testing ray hit anything
Vec  ray_hit_pos; // if hit, then this is the hit position

MaterialPtr brick  ,
            glass  ;
Mesh        box    ,
            ball   ,
            capsule;

struct Object // object
{
   Actor actor; // actor
   Mesh *mesh ; // mesh

   void drawPrepare() // draw
   {
      if(mesh)mesh->draw(actor.matrix());
   }
};
Memb<Object> obj; // objects
/******************************************************************************/
void InitPre()
{
   App.name("Auto Depth of Field");
   App.flag=APP_FULL_TOGGLE;
   Paks.add("../data/engine.pak");

   D.full(true).ambientPower(0).viewRange(32);
   Ms.hide();

   // set initial depth of field parameters
   D.dofMode (DOF_HIGH)
    .dofBlurs(1)
    .dofHalf (true)
    .dofRange(6);
}
/******************************************************************************/
Bool Init()
{
   Physics.create(CSS_NONE, true, "../Installation/PhysX");

   // get materials
   brick.require("../data/mtrl/brick/0.mtrl");
   glass.require("../data/mtrl/glass/0.mtrl");

   // shapes used for mesh and actor creation
   Box     s_box    (32,1,32);
   Ball    s_ball   (0.5f);
   Capsule s_capsule(0.3f,2);

   // create meshes
   box    .parts.New().base.create(s_box    , VTX_TEX0|VTX_NRM|VTX_TAN).texScale(Vec2(16));
   ball   .parts.New().base.create(s_ball   , VTX_TEX0|VTX_NRM|VTX_TAN);
   capsule.parts.New().base.create(s_capsule, VTX_TEX0|VTX_NRM|VTX_TAN);

   // set mesh materials, rendering versions and bounding boxes
   box    .setMaterial(brick).setRender().setBox();
   ball   .setMaterial(glass).setRender().setBox();
   capsule.setMaterial(brick).setRender().setBox();

   // create objects
   {
      // ground
      {
         Object &o=obj.New();
         o.mesh=&box;
         o.actor.create(s_box,0).pos(Vec(0, -s_box.h()/2, 0));
      }

      // pillars
      REPD(x, 16)
      REPD(z, 16)
      {
         Object &o=obj.New();
         o.mesh=&capsule;
         o.actor.create(s_capsule, 0).pos(Vec(s_box.lerpX(x/15.0f), s_capsule.h/2-s_capsule.r, s_box.lerpZ(z/15.0f)));
      }
      
      // balls
      REP(100)
      {
         Object &o=obj.New();
         o.mesh=&ball;
         o.actor.create(s_ball).pos(Vec(s_box.lerpX(RandomF()), s_ball.r+s_capsule.h, s_box.lerpZ(RandomF())));
      }
   }

   // set initial camera
   Cam.setAngle(Vec(0,2,0), 0, -0.2f).set();

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

   // update camera
   {
      if(Kb.b(KB_A    ))Cam.matrix.pos-=Cam.matrix.x*Time.d();
      if(Kb.b(KB_D    ))Cam.matrix.pos+=Cam.matrix.x*Time.d();
      if(Kb.b(KB_W    ))Cam.matrix.pos+=Cam.matrix.z*Time.d();
      if(Kb.b(KB_S    ))Cam.matrix.pos-=Cam.matrix.z*Time.d();
      if(Kb.b(KB_SPACE))Cam.matrix.pos+=Cam.matrix.y*Time.d();
      if(Kb.b(KB_LCTRL))Cam.matrix.pos-=Cam.matrix.y*Time.d();
      if(Ms.hidden())
      {
         Cam.yaw  -=Ms.d().x;
         Cam.pitch+=Ms.d().y;
      }
      Cam.setAngle(Cam.matrix.pos, Cam.yaw, Cam.pitch).updateVelocities().set();
   }

   // toggle mouse
   if(Kb.bp(KB_TAB))Ms.toggle();

   // test ray
   {
      Flt     new_z; // new Z focus
      PhysHit phys_hit;
      Vec     pos, dir;

      if(Ms.hidden())
      {
         pos=Cam.matrix.pos;
         dir=Cam.matrix.z;
      }else
      {
         ScreenToPosDir(Ms.pos(), pos, dir);
      }

      if(Physics.ray(pos, dir*D.viewRange(), &phys_hit)) // cast a ray from camera and check if it hits something
      {
         ray_hit    =true;
         ray_hit_pos=phys_hit.plane.pos;
         new_z      =phys_hit.dist; // set new z focus to ray hit distance
      }else
      {
         ray_hit=false;
         new_z  =D.viewRange(); // set new z focus to viewport range
      }

      // set new depth of field z focus
      Flt old_z=D.dofZ(); AdjustValTime(old_z, new_z, 0.001f); D.dofZ(old_z);
   }

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         REPAO(obj).drawPrepare();

         LightDir(!Vec(1,-1,1)).add();
      }break;
   }
}
void Draw()
{
   Renderer(Render);

   if(ray_hit)
   {
      if(Renderer.rebuildDepthNeededForDebugDrawing())Renderer.rebuildDepth();
      SetMatrix();
      Ball(0.1f, ray_hit_pos).draw();
   }

   D.text(0, 0.9f, "Press Tab to toggle mouse, WSAD+Space+Control to move camera");
}
/******************************************************************************/
