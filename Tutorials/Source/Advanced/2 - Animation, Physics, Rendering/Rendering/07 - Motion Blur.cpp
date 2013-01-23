/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
struct Object
{
   Matrix matrix; // matrix
   Vec       vel, // linear  velocity
         ang_vel; // angular velocity
}object[12];
/******************************************************************************/
Mesh box ,
     ball;
/******************************************************************************/
void InitPre()
{
   App.name("Rendering Motion Blur");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE;
   Paks.add("../data/engine.pak");

   D.full        (true       )
    .ambientPower(0          )
    .motionMode  (MOTION_HIGH)  // enable   motion blur
    .motionSmooth(true       )  // increase number of blurring samples
    .motionScale (0.08f      ); // increase motion blur scale
}
/******************************************************************************/
Bool Init()
{
   Cam.dist=3;

   MaterialPtr material=Materials.ptrRequire("../data/mtrl/brick/0.mtrl");

   box .parts.New().base.create( Box(4   ), VTX_TEX0|VTX_NRM|VTX_TAN).reverse(); // create mesh box, reverse it because it's meant to be viewed from inside
   ball.parts.New().base.create(Ball(0.2f), VTX_TEX0|VTX_NRM|VTX_TAN)          ; // create mesh ball

   // set mesh materials, rendering versions and bounding boxes
   box .setMaterial(material).setRender().setBox();
   ball.setMaterial(material).setRender().setBox();

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
   
   CamHandle(0.1f, 10, CAMH_ZOOM|(Ms.b(1) ? CAMH_MOVE : CAMH_ROT)); // move camera on right mouse button

   // change motion blur mode when keys pressed
   if(Kb.bp(KB_1))D.motionMode(MOTION_NONE);
   if(Kb.bp(KB_2))D.motionMode(MOTION_LOW );
   if(Kb.bp(KB_3))D.motionMode(MOTION_HIGH);

   // update ball object matrixes and calculate velocity changes automatically
   Flt speed=4;
   REPA(object)
   {
      // calculate new matrix
      Vec2   v; SinCos(v.y, v.x, i*PI2/Elms(object) + Time.time()*speed); // calculate sine and cosine of angle
      Matrix new_matrix(Vec(v.x, 0, v.y));                                // create 'new_matrix' with initial position

      // calculate velocity changes according to old and new matrix
      GetVel(object[i].vel, object[i].ang_vel, object[i].matrix, new_matrix);

      // store new matrix
      object[i].matrix=new_matrix;
   }

   return true;
}
/******************************************************************************/
void Render() // rendering method
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         box.draw(MatrixIdentity, Vec(0,0,0));                                      // box is rendered with identity matrix and (0,0,0) velocity
         REPA(object)ball.draw(object[i].matrix, object[i].vel, object[i].ang_vel); // draw ball objects with their matrix and velocities

         LightPoint(25, Vec(0,3,0)).add();
      }break;
   }
}
void Draw()
{
   Renderer(Render);
   D.text(0, 0.9f, S+"Fps "+Time.fps()); // show number of fps
   D.text(0, 0.8f, "Press 1,2,3 keys for different Motion Blur modes");
   switch(D.motionMode())
   {
      case MOTION_NONE: D.text(0, 0.7f, "No Motion Blur"  ); break;
      case MOTION_LOW : D.text(0, 0.7f, "Camera only"     ); break;
      case MOTION_HIGH: D.text(0, 0.7f, "Camera + Objects"); break;
   }
}
/******************************************************************************/
