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
}obj[32];

Window   window;
Text    tbloom_org,
        tbloom_scale,
        tbloom_cut, 
        tbloom_overbright,
        tbloom_half,
        tbloom_blurs;
Slider   bloom_org,
         bloom_scale,
         bloom_cut;
CheckBox bloom_overbright,
         bloom_half;
ComboBox bloom_blurs;
/******************************************************************************/
void InitPre()
{
   App.name("Bloom configuration");
   App.flag=APP_FULL_TOGGLE;
   Paks.add("../data/engine.pak");

   D.full(true).ambientPower(0);
}
/******************************************************************************/
Bool Init()
{
   Cam.dist=3;

   brick=Materials.ptrRequire("../data/mtrl/brick/0.mtrl");
   glass=Materials.ptrRequire("../data/mtrl/glass/0.mtrl");

   box .parts.New().base.create( Box(4   ), VTX_TEX0|VTX_NRM|VTX_TAN).reverse(); // create mesh box, reverse it because it's meant to be viewed from inside
   ball.parts.New().base.create(Ball(0.5f), VTX_TEX0|VTX_NRM|VTX_TAN)          ; // create mesh ball

   // set mesh materials, rendering versions and bounding boxes
   box .setMaterial(brick).setRender().setBox();
   ball.setMaterial(glass).setRender().setBox();

   // set random positions inside box
   REPAO(obj).create();

   // add gui bloom controls
   static CChar8 *blurs_t[]=
   {
      "1",
      "2",
      "3",
      "4",
   };

   Gui   +=window.create(Rect_D(0, -D.h(), 0.6f, 0.54f), "Bloom Parameters");
   window+=tbloom_org       .create(Vec2(0.13f, -0.07f), "Origin"    ); window+=bloom_org       .create(Rect_C(0.1f+window.crect.w()/2, -0.07f, 0.25f, 0.05f), D.bloomOriginal  ());
   window+=tbloom_scale     .create(Vec2(0.13f, -0.14f), "Scale"     ); window+=bloom_scale     .create(Rect_C(0.1f+window.crect.w()/2, -0.14f, 0.25f, 0.05f), D.bloomScale     ());
   window+=tbloom_cut       .create(Vec2(0.13f, -0.21f), "Cutoff"    ); window+=bloom_cut       .create(Rect_C(0.1f+window.crect.w()/2, -0.21f, 0.25f, 0.05f), D.bloomCut       ());
   window+=tbloom_overbright.create(Vec2(0.13f, -0.28f), "Overbright"); window+=bloom_overbright.create(Rect_C(0.1f+window.crect.w()/2, -0.28f, 0.05f, 0.05f), D.bloomOverbright());
   window+=tbloom_half      .create(Vec2(0.13f, -0.35f), "Half Res"  ); window+=bloom_half      .create(Rect_C(0.1f+window.crect.w()/2, -0.35f, 0.05f, 0.05f), D.bloomHalf      ());
   window+=tbloom_blurs     .create(Vec2(0.13f, -0.42f), "Blurs"     ); window+=bloom_blurs     .create(Rect_C(0.1f+window.crect.w()/2, -0.42f, 0.25f, 0.05f), blurs_t, Elms(blurs_t)).set(D.bloomBlurs()-1);

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

   Cam.setSpherical(Vec(0,0,0), Time.time()/16, -0.3f, 0, 3).updateVelocities().set(); // set autorotating camera

   // set bloom parameters
   D.bloomOriginal  (bloom_org())
    .bloomScale     (bloom_scale())
    .bloomCut       (bloom_cut())
    .bloomOverbright(bloom_overbright())
    .bloomHalf      (bloom_half())
    .bloomBlurs     (bloom_blurs()+1);

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

         LightPoint(25, Vec(0,3,0)).add();
      }break;
   }
}
void Draw()
{
   Renderer(Render);
   Gui.draw();
}
/******************************************************************************/
