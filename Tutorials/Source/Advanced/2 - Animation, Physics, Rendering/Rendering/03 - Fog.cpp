/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
MaterialPtr brick,
            glass;
Mesh        box  ,
            ball ;

struct Obj
{
   Vec      pos;
   Material material;

   void create()
   {
      pos     =Random(box.box);
      material=*glass;
      material.color.v3()=ColorHue(RandomF()).asVec();
      material.validate();
   }

   void draw()
   {
      MaterialLock=&material; ball.draw(Matrix(pos));
      MaterialLock=NULL;
   }
}obj[64];

Window  window;
Slider  fog_frac,
        fog_exp ;
Text   tfog_frac,
       tfog_exp ;
/******************************************************************************/
void InitPre()
{
   App.name("Fog");
   App.flag=APP_FULL_TOGGLE;
   Paks.add("../data/engine.pak");

   D.full(true).ambientPower(0).bloomOriginal(0.8f).bloomScale(0.6f).bloomOverbright(true).bloomHalf(false).bloomBlurs(1).viewRange(25);

   // hide mouse
   Ms.hide();
}
/******************************************************************************/
Bool Init()
{
   brick.require("../data/mtrl/brick/0.mtrl");
   glass.require("../data/mtrl/glass/0.mtrl");

   box .parts.New().base.create( Box( 10), VTX_TEX0|VTX_NRM|VTX_TAN).reverse(); // create mesh box, reverse it because it's meant to be viewed from inside
   ball.parts.New().base.create(Ball(0.5), VTX_TEX0|VTX_NRM|VTX_TAN)          ; // create mesh ball

   // set mesh materials, rendering versions and bounding boxes
   box .setMaterial(brick).setRender().setBox();
   ball.setMaterial(glass).setRender().setBox();

   // set objects
   REPAO(obj).create();

   // set camera
   Cam.at   = box.box.cornerLDF()/2;
   Cam.dist = 10;
   Cam.pitch=-PI_4;
   Cam.yaw  = PI_4;

   // set Sky and initial fog parameters
   Sky.atmospheric()
      .atmosphericColor(Vec4(0.3f), Vec4(0.3f))
      .frac(0);

   // add gui fog controls
   Gui   +=window   .create(Rect_C(0, -0.75f, 0.9f, 0.34f), "Fog Parameters").hide();
   window+=tfog_frac.create(Vec2(0.2f, -0.08f), "Fraction"); window+=fog_frac.create(Rect_C(0.2f+window.crect.w()/2, -0.08f, 0.25f, 0.05f), Sky.frac                      ()).desc("Fraction of the Viewport Range where the Fog starts");
   window+=tfog_exp .create(Vec2(0.2f, -0.16f), "Exponent"); window+=fog_exp .create(Rect_C(0.2f+window.crect.w()/2, -0.16f, 0.25f, 0.05f), Sky.atmosphericDensityExponent()).desc("Fog Density");

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
   Gui.update();
   CamHandle(0.01f, 100, Ms.hidden() ? CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT) : 0); // when mouse hidden operate the camera

   if(Kb.bp(KB_TAB))
   {
      Ms    .toggle               (); // toggle mouse  visibility when tab pressed
      window.visibleToggleActivate(); // toggle window visibility when tab pressed
   }

   // set fog parameters
   Sky.frac                      (    fog_frac()     )
      .atmosphericDensityExponent(Pow(fog_exp (), 16));

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         box.draw(MatrixIdentity);
         REPAO(obj).draw();

         LightPoint(40, Vec(0,0,0)).add();
      }break;
   }
}
void Draw()
{
   Renderer(Render);
   Gui.draw();
   if(Ms.hidden())D.text(0, 0.9f, S+"Press Tab to toggle parameters");
}
/******************************************************************************/
