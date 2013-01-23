/******************************************************************************/
#include "stdafx.h"
/******************************************************************************
 
   In this tutorial is presented how to achieve high ambient lighting value with satisfactionary looks

/******************************************************************************/
struct AmbientControls
{
   Window    win;
   Text     tmode, tsoft, tjitter, thalf_res, tpower, tcontrast, trange, tscale, tbias;
   ComboBox  mode,  soft;
   CheckBox  jitter, half_res;
   Slider    power, contrast, range, scale, bias;

   void create()
   {
      static CChar8 *modes[]=
      {
         "off",
         "low",
         "medium",
         "high",
         "ultra",
      };
      static CChar8 *softing[]=
      {
         "0",
         "1",
         "2",
         "3",
      };
      Flt y=0, h=0.062;
      Gui+= win     .create(Rect_RD(D.w(), -D.h(), 0.9f, 0.67f), "Ambient Parameters").hide(); y-=h;
      win+=tmode    .create(Vec2(0.2f,y), "Mode"           ); win+=mode    .create(Rect_C(0.2f+win.crect.w()/2, y, 0.25f, 0.05f), modes  , Elms(modes  )).set(D.ambientMode()); y-=h;
      win+=tsoft    .create(Vec2(0.2f,y), "Soft"           ); win+=soft    .create(Rect_C(0.2f+win.crect.w()/2, y, 0.25f, 0.05f), softing, Elms(softing)).set(D.ambientSoft()); y-=h;
      win+=tjitter  .create(Vec2(0.2f,y), "Jitter"         ); win+=jitter  .create(Rect_C(0.2f+win.crect.w()/2, y, 0.05f, 0.05f), D.ambientJitter  ()     ); y-=h;
      win+=thalf_res.create(Vec2(0.2f,y), "Half Resolution"); win+=half_res.create(Rect_C(0.2f+win.crect.w()/2, y, 0.05f, 0.05f), D.ambientHalfRes ()     ); y-=h;
      win+=tpower   .create(Vec2(0.2f,y), "Power"          ); win+=power   .create(Rect_C(0.2f+win.crect.w()/2, y, 0.25f, 0.05f), D.ambientPower   ()     ); y-=h;
      win+=tcontrast.create(Vec2(0.2f,y), "Contrast"       ); win+=contrast.create(Rect_C(0.2f+win.crect.w()/2, y, 0.25f, 0.05f), D.ambientContrast()/4   ); y-=h;
      win+=trange   .create(Vec2(0.2f,y), "Range"          ); win+=range   .create(Rect_C(0.2f+win.crect.w()/2, y, 0.25f, 0.05f), D.ambientRange   ()/0.4f); y-=h;
      win+=tscale   .create(Vec2(0.2f,y), "3D Scale"       ); win+=scale   .create(Rect_C(0.2f+win.crect.w()/2, y, 0.25f, 0.05f), D.ambientScale   ()/4   ); y-=h;
      win+=tbias    .create(Vec2(0.2f,y), "Bias"           ); win+=bias    .create(Rect_C(0.2f+win.crect.w()/2, y, 0.25f, 0.05f), D.ambientBias    ()     ); y-=h;

      mode.kb_catch=soft.kb_catch=false;
   }
}AC;
/******************************************************************************/
Mesh   background,
       ball,
       box ;
Matrix matrix_ball[25],
       matrix_box [25];
/******************************************************************************/
void InitPre()
{
   App.name("Ambient Occlusion");
   App.flag=APP_FULL_TOGGLE;
   Paks.add("../data/engine.pak");

   D.full(true).ambientMode(AMBIENT_HIGH).ambientSoft(1).ambientJitter(1).ambientHalfRes(1).ambientPower(0.7f); // set initial ambient occlusion parameters

   Ms.hide(); // hide mouse
}
/******************************************************************************/
Bool Init()
{
   Cam.dist=5;

   MaterialPtr brick=Materials.ptrRequire("../data/mtrl/brick/0.mtrl"),
               glass=Materials.ptrRequire("../data/mtrl/glass/0.mtrl");

   background.parts.New().base.create( Box(3   ), VTX_TEX0|VTX_NRM|VTX_TAN).reverse(); // create background box
   box       .parts.New().base.create( Box(0.5f), VTX_TEX0|VTX_NRM|VTX_TAN);           // create box
   ball      .parts.New().base.create(Ball(0.5f), VTX_TEX0|VTX_NRM|VTX_TAN);           // create ball

   background.setMaterial(brick).setRender().setBox();
   box       .setMaterial(glass).setRender().setBox();
   ball      .setMaterial(glass).setRender().setBox();

   // setup random matrixes
   REPA(matrix_ball)matrix_ball[i].setRotateXY(RandomF(PI2), RandomF(PI2)).move(Random(background.box));
   REPA(matrix_box )matrix_box [i].setRotateXY(RandomF(PI2), RandomF(PI2)).move(Random(background.box));

   AC.create();

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
         Ms .toggle               (); // toggle mouse  visibility when tab pressed
      AC.win.visibleToggleActivate(); // toggle window visibility when tab pressed
   }

   // set Ambient Occlusion parameters
   D.ambientMode    ((AMBIENT_MODE)AC.mode    ())
    .ambientSoft    (              AC.soft    ())
    .ambientJitter  (              AC.jitter  ())
    .ambientHalfRes (              AC.half_res())
    .ambientPower   (              AC.power   ())
    .ambientContrast(              AC.contrast()*4)
    .ambientRange   (              AC.range   ()*0.4)
    .ambientScale   (              AC.scale   ()*4)
    .ambientBias    (              AC.bias    ());

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
                    background.draw(MatrixIdentity);
         REPA(matrix_ball)ball.draw(matrix_ball[i]);
         REPA(matrix_box )box .draw(matrix_box [i]);
      }break;
   }
}
void Draw()
{
   Renderer(Render);
   Gui.draw();
   if(Ms.hidden())D.text(0, 0.9f, S+"Press Tab to toggle parameters");
   else           D.text(0, 0.9f, S+"Fps: "+Time.fps());
}
/******************************************************************************/
