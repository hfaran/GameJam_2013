/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/_enums.h"
/******************************************************************************

   In this tutorial is presented how to extend a Game Object class.
   But unlike in previous examples, we won't base on Character/Item or other classes.
   This time we'll extend the most base class - Game::Obj.

/******************************************************************************/
STRUCT(NewObj , Game::Obj) // extend Game Object
//{
   Vec     position; // the class will contain only position
   MeshPtr mesh    ; // and mesh

   // provide necessary methods required by Game::Obj :
      virtual void create(Game::ObjParams &obj); // extend default creation

      virtual Vec  pos(          ) {return position    ;} // get position
      virtual void pos(C Vec &pos) {     T.position=pos;} // set position

      virtual Matrix matrix(                ) {return position           ;} // get matrix
      virtual void   matrix(C Matrix &matrix) {     T.position=matrix.pos;} // set matrix

      virtual Bool update     () {return true;} // object update
      virtual UInt drawPrepare();               // object draw prepare
      virtual void drawShadow ();               // object draw shadow

   // io methods
   virtual void save(File &f);
   virtual Bool load(File &f);

   // constructor
   NewObj();
};
/******************************************************************************/
NewObj::NewObj() // initialize values in constructor
{
   position.zero();
}
void NewObj::create(Game::ObjParams &obj_params)
{
   // now setup custom parameters from 'obj_params'
   position=obj_params.matrixFinal().pos; // obtain our 'position' member from 'obj_params'
   mesh    =obj_params.mesh       ()    ; // set 'mesh'
}
/******************************************************************************/
UInt NewObj::drawPrepare()
{
   if(mesh && Frustum(mesh->box,matrix()))mesh->draw(matrix());
   return 0; // return 0 because no additional rendering modes are required
}
void NewObj::drawShadow()
{
   if(mesh && Frustum(mesh->box, matrix()))mesh->drawShadow(matrix());
}
/******************************************************************************/
void NewObj::save(File &f)
{
   super::save(f); // default save

   f<<position;           // save custom parameters
   f.putStr(mesh.name()); // save mesh path
}
Bool NewObj::load(File &f)
{
   if(super::load(f)) // if default load was successful
   {
      f>>position;              // load custom parameters
      mesh.require(f.getStr()); // load mesh
      return true; // return success
   }
   return false; // return failure
}
/******************************************************************************/
// MAIN
/******************************************************************************/
Game::ObjMemx<NewObj> NewObjs; // container for objects
/******************************************************************************/
void InitPre()
{
   App.name("Game::Obj class");
   App.flag=APP_FULL_TOGGLE|APP_MS_EXCLUSIVE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true).ambientPower(0.3f).hpRt(true).viewRange(50);

   Cam.at.set(16,0,16);
   Cam.dist = 10;
   Cam.pitch=-PI_3;
}
/******************************************************************************/
Bool Init()
{
   Physics.create(CSS_NONE, true, "../Installation/PhysX");
   Sky    .atmospheric();
   Sun.image="Env/Sky/sun.gfx"; Sun.light_color=1-D.ambientColor();

   Game::World.init      (                 )
              .setObjType(NewObjs, OBJ_ITEM) // use OBJ_ITEM type because objects in the world we're using are saved with OBJ_ITEM type
              .New       ("world/custom params.world");

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
void Draw()
{
   Renderer(Render);
}
/******************************************************************************/
