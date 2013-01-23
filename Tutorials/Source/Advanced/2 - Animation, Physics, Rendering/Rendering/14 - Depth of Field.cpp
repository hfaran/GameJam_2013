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
CheckBox dof_hi,
         dof_half;
ComboBox dof_blurs;
Slider   dof_power,
         dof_range;
Text    tdof_hi,
        tdof_half,
        tdof_blurs,
        tdof_power,
        tdof_range;
/******************************************************************************/
void InitPre()
{
   App.name("Depth of Field");
   App.flag=APP_FULL_TOGGLE;
   Paks.add("../data/engine.pak");

   D.full(true).ambientPower(0).dofMode(DOF_HIGH).dofHalf(false).dofRange(5); // set initial depth of field parameters

   // hide mouse
   Ms.hide();
}
/******************************************************************************/
Bool Init()
{
   brick=Materials.ptrRequire("../data/mtrl/brick/0.mtrl");
   glass=Materials.ptrRequire("../data/mtrl/glass/0.mtrl");

   box .parts.New().base.create( Box(4   ), VTX_TEX0|VTX_NRM|VTX_TAN).reverse(); // create mesh box, reverse it because it's meant to be viewed from inside
   ball.parts.New().base.create(Ball(0.5f), VTX_TEX0|VTX_NRM|VTX_TAN)          ; // create mesh ball

   // set mesh materials, rendering versions and bounding boxes
   box .setMaterial(brick).setRender().setBox();
   ball.setMaterial(glass).setRender().setBox();

   // set random positions inside box
   REPAO(obj).create();

   // set camera
   Cam.at   = box.box.cornerLDF()/2;
   Cam.dist = 10;
   Cam.pitch=-PI_4;
   Cam.yaw  = PI_4;

   // add gui Dof controls
   static CChar8 *blurs[]=
   {
      "1",
      "2",
      "3",
   };
   Gui   +=window    .create(Rect_C(0, -0.7f, 0.9f, 0.58f), "Depth of Field Parameters").hide();
   window+=tdof_hi   .create(Vec2(0.2f, -0.08f), "High Quality"); window+=dof_hi   .create(Rect_C(0.2f+window.crect.w()/2, -0.08f, 0.05f, 0.05f), D.dofMode ()==DOF_HIGH).desc("Prevents foreground objects leaking into background");
   window+=tdof_half .create(Vec2(0.2f, -0.16f), "Half Res"    ); window+=dof_half .create(Rect_C(0.2f+window.crect.w()/2, -0.16f, 0.05f, 0.05f), D.dofHalf ());
   window+=tdof_blurs.create(Vec2(0.2f, -0.24f), "Blurs"       ); window+=dof_blurs.create(Rect_C(0.2f+window.crect.w()/2, -0.24f, 0.25f, 0.05f), blurs, Elms(blurs)).set(D.dofBlurs()-1);
   window+=tdof_power.create(Vec2(0.2f, -0.32f), "Power"       ); window+=dof_power.create(Rect_C(0.2f+window.crect.w()/2, -0.32f, 0.25f, 0.05f), D.dofPower());
   window+=tdof_range.create(Vec2(0.2f, -0.40f), "Range"       ); window+=dof_range.create(Rect_C(0.2f+window.crect.w()/2, -0.40f, 0.25f, 0.05f), D.dofRange()/10);

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

   // set depth of field parameters
   D.dofMode (dof_hi   () ? DOF_HIGH : DOF_ON)
    .dofHalf (dof_half ()   )
    .dofBlurs(dof_blurs()+1 )
    .dofPower(dof_power()   )
    .dofZ    (Cam.dist      )
    .dofRange(dof_range()*10);

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
   if(Ms.hidden())D.text(0, 0.9f, S+"Press Tab to toggle parameters");
   else           D.text(0, 0.9f, S+"Fps "+Time.fps());
}
/******************************************************************************/
