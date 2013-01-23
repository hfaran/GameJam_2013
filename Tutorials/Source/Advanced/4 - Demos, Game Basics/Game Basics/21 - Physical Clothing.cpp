/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/_enums.h"
/******************************************************************************

   Here we'll present how to enable character physical clothing.

/******************************************************************************/
enum ACTOR_GROUP // first we need to define some new user actor groups, to properly manage collisions between capes, ragdolls, character controllers and other actors.
{
   AG_CAPE             =1, // specify group for capes
   AG_KINEMATIC_RAGDOLL=2, // specify group for kinematic ragdolls
};
/******************************************************************************/
STRUCT(Player , Game::Chr) // extend the character class
//{
   Vec     ctrl_pos         ;
   Cloth   cape             ; // create cape cloth object
   Ragdoll kinematic_ragdoll; // physical clothing requires an additional helper ragdoll
                              // this ragdoll is needed so we can attach the cloth vertexes to the ragdoll bones, for this purpose kinematic mode gives the best results

   // get / set
   virtual Vec    pos   (                ) {return super::pos   (      );}
   virtual Matrix matrix(                ) {return super::matrix(      );}
   virtual void   pos   (C Vec    &pos   ) {       super::pos   (pos   ); ctrl_pos=ctrl.actor.pos();}
   virtual void   matrix(C Matrix &matrix) {       super::matrix(matrix); ctrl_pos=ctrl.actor.pos();}

   virtual void create     (Game::ObjParams &obj); // extend character creation to create the cape and ragdoll
   virtual Bool update     (                    ); // extend updating to include setting ragdoll pose each frame
   virtual UInt drawPrepare(                    ); // extend drawing  to include cloth drawing
   virtual void drawShadow (                    ); // extend drawing  to include cloth drawing
};
/******************************************************************************/
Game::ObjMemx<Game::Static> Statics; // container for static objects
Game::ObjMemx<Game::Item  > Items  ; // container for item   objects
Game::ObjMemx<      Player> Players; // container for player objects
/******************************************************************************/
void Player::create(Game::ObjParams &obj)
{
   super::create(obj);

   // now let's create the cloth and the helper ragdoll

   // let's start by creating the ragdoll
   {
      kinematic_ragdoll.create(cskel, 1, true      )  // create the helper ragdoll as kinematic
                       .group (AG_KINEMATIC_RAGDOLL)  // set desired actor group
                       .ray   (false               ); // disable ray-testing

      // now we would like to create the cloth, however adding the cloth to the character works best when the ragdoll has its pose already set
      // manually setting ragdoll pose is done using 'Ragdoll::fromSkel' method which uses the character controlled skeleton
      // however since we're in 'Player::create' method the skeleton doesn't have a pose set either

      // so first we need to set even a basic skeleton pose
      Matrix character_matrix=obj.matrixFinal();    // get the target character matrix
             character_matrix.normalize      ();    // normalize it because matrixes for 'updateMatrix' method must be normalized
      cskel.clear().updateMatrix(character_matrix); // clearing and updating the matrixes reset the skeleton to its default pose

      // now the skeleton is in its default pose (just the way you can see it in the Mesh Editor), so we can set the ragdoll pose from it
      kinematic_ragdoll.fromSkel(cskel, cskel.vel(), true); // use 'true' for immediate pose setting at creation time, to ensure that the pose will be set immediately, please read comments on 'fromSkel' method for more information about it
   }

   // now both skeleton and ragdoll are in their default pose, so we can proceed to creating the cloth
   {
      // setup cloth parameters
      Cloth::Param param;
      param.flag               =0;
      param.thickness          =0.15f;
      param.bending            =0.6f;
      param.stretching         =0.0f;
      param.damping            =0.6f;
      param.response           =0.0f;
      param.attachment_response=0.0f;

      // create the cloth
      cape.create(*ClothMeshes("obj/item/cloth/cape/0.clothmesh"), obj.matrixFinal(), param); // use ClothMesh file exported from Mesh Editor
      cape.group (AG_CAPE); // set desired actor group

      // now the cloth is created and will be seen on the screen, but still we need to attach it to the ragdoll
      if(ClothMesh   *cloth_mesh=cape.clothMesh    ()) // access cloth mesh from which the cloth has been created
      if(const Vec   *vtx_pos   =cloth_mesh->pos   ()) // access cloth mesh source vertex positions
      if(const Byte  *vtx_flag  =cloth_mesh->flag  ()) // access cloth mesh source vertex flags
      if(const VecB4 *vtx_matrix=cloth_mesh->matrix()) // access cloth mesh source vertex matrix indexes
      {
         REP(cloth_mesh->vtxs()) // iterate through all cloth vertexes
            if(!FlagTest(vtx_flag[i], VTX_FLAG_CLOTH)) // if the vertex cloth shouldn't be physically simulated then attach it
         {
            Int ragdoll_bone_index =kinematic_ragdoll.findBoneIndexFromVtxMatrix(vtx_matrix[i].x); // get ragdoll bone index according to vertex matrix
            if( ragdoll_bone_index!=-1) // if the bone index is valid
            {
               Actor &actor          =kinematic_ragdoll.bone(ragdoll_bone_index).actor; // get ragdoll bone's actor
               Vec    vtx_source_pos =vtx_pos[i]  ; // get original cloth mesh vertex position 
                      vtx_source_pos*=cape.scale(); // scale it according to current cloth instance scale
               Bool   response       =false       , // disable response in attachment
                      local_pos      =true        ; // use ragdoll local space to attach the cloth vertex regardless of the current ragdoll pose

               cape.attachVtx(i, vtx_source_pos, &actor, response, local_pos); // attach the vertex
            }
         }
      }
   }
}
/******************************************************************************/
Bool Player::update()
{
   if(action)
   {
      if(Kb.b(KB_W) || Kb.b(KB_S) || Kb.b(KB_A) || Kb.b(KB_D) || Kb.b(KB_Q) || Kb.b(KB_E))actionBreak();
   }

   if(!action)
   {
      // turn & move
      input.turn.x=Kb.b(KB_Q)-Kb.b(KB_E);
      input.turn.y=Kb.b(KB_T)-Kb.b(KB_G);
      input.move.x=Kb.b(KB_D)-Kb.b(KB_A);
      input.move.z=Kb.b(KB_W)-Kb.b(KB_S);
      input.move.y=Kb.b(KB_SPACE)-Kb.b(KB_LSHIFT);

      // dodge, crouch, walk, jump
      input.dodge = Kb.bd(KB_D)-Kb.bd(KB_A);
      input.crouch= Kb.b (KB_LSHIFT);
      input.walk  = Kb.b (KB_LCTRL );
      input.jump  =(Kb.bp(KB_SPACE ) ? 3.5f : 0);

      // mouse turn
      Flt max=DegToRad(900)*Time.d();
      angle.x-=Mid(Ms.d().x*1.7f, -max, max);
      angle.y+=Mid(Ms.d().y*1.7f, -max, max);
   }

   ctrl_pos=ctrl.actor.pos();

   if(super::update())
   {
      // after calling the default update the character skeleton is animated, so basing on that information we'll set the helper ragdoll to the skeleton pose using 'fromSkel' method
      kinematic_ragdoll.fromSkel(cskel, cskel.vel());
      return true;
   }
   return false;
}
/******************************************************************************/
UInt Player::drawPrepare()
{
   UInt modes=super::drawPrepare();
   cape.drawSkinned (cskel); // draw   normally skinned   part of the cloth (vertexes which aren't simulated physically)
   cape.drawPhysical(     ); // draw physically simulated part of the cloth
   return modes;
}
void Player::drawShadow()
{
   super::drawShadow();
   cape.drawSkinnedShadow (cskel); // draw   normally skinned   part of the cloth (vertexes which aren't simulated physically)
   cape.drawPhysicalShadow(     ); // draw physically simulated part of the cloth
}
/******************************************************************************/
void InitPre()
{
   App.name("Physical Clothing");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");

   Cam.dist=3;

   D.full(true).hpRt(true).ambientPower(0.3f);
}
/******************************************************************************/
Bool Init()
{
   Physics.create(CSS_NONE, true, "../Installation/PhysX");
   Sky    .atmospheric();
   Sun.image="Env/Sky/sun.gfx"; Sun.light_color=1-D.ambientColor();

   // in order to properly synchronize the physical clothing with the character, you must set the following engine options
   Game::World.update_objects_after_physics=false; // force updating the objects before updating physics 
   Physics.timestep(PHYS_TIMESTEP_VARIABLE);       // set variable timestepping when updating physics
   D.sync(true);                                   // enable screen synchronization because variable timestepping works best in a constant framerate

   // set collision groups
   REP(32)if(i!=AG_CAPE)Physics.ignore(i, AG_KINEMATIC_RAGDOLL); // ignore collisions between AG_KINEMATIC_RAGDOLL and all other groups except AG_CAPE
                        Physics.ignore(AG_CAPE, AG_CONTROLLER ); // ignore collisions between capes and character controllers

   // create the world
   Game::World.init()
              .setObjType(Statics, OBJ_STATIC)
              .setObjType(Players, OBJ_PLAYER)
              .setObjType(Items  , OBJ_ITEM  )
              .New("world/sample.world");

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

   // setup the camera
   if(Players.elms())
   {
      Cam.dist=Max(1.0f, Cam.dist*ScaleFactor(Ms.wheel()*-0.1f));
      Cam.setSpherical(Players[0].ctrl_pos+Vec(0,1,0), Players[0].angle.x, Players[0].angle.y, 0, Cam.dist);
      Cam.updateVelocities().set();
   }
   else
   {
      CamHandle(0.1f, 100, CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT));
   }

   return true;
}
/******************************************************************************/
void Render()
{
   Game::World.draw();
}
void Draw()
{
   Renderer(Render);
}
/******************************************************************************/
