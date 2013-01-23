/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
struct DisplayOptions
{
   Window   window;
   CheckBox vol,  hp_rt,  hdr;
   Text    tvol, thp_rt, thdr;
   
   void create()
   {
      Gui   +=window.create(Rect_R(D.w(), 0.5, 0.8, 0.35), "Display Options");
      window+=vol   .create(Rect (0.05, -0.10, 0.10, -0.05)); window+=tvol  .create(Vec2(0.45, -0.075), "Volumetric Lighting");
      window+=hp_rt .create(Rect (0.05, -0.18, 0.10, -0.13)); window+=thp_rt.create(Vec2(0.45, -0.155), "High Precision Render Targets");
      window+=hdr   .create(Rect (0.05, -0.26, 0.10, -0.21)); window+=thdr  .create(Vec2(0.45, -0.235), "High Dynamic Range");
   }
   void update()
   {
      D.volLight(vol  ());
      D.hpRt    (hp_rt());
      D.hdr     (hdr  ());
   }
}Opt;
/******************************************************************************/
struct CloudImages
{
   Window window;
   Button generate, save, load;
   Image  cloud, detail;

   static void Generate(CloudImages &ci)
   {
      if(!ci.detail.is()) // if detail image hasn't yet been created
      {
         CreateVolumetricDetail(ci.detail, 128, 16, 128); // create volumetric detail image, this image is used in cloud image creation, and it also can be used in the drawing process for additional quality
         ci.detail.blur(1, false); // blur it
      }

      CreateVolumetricCloud(ci.cloud, &ci.detail, 256, 32, 256, 3, false, 80);
      ci.cloud.lumFromAlphaAndLight(DIRF_RIGHT|DIRF_DOWN|DIRF_FORWARD, 1.0);
      Time    .skipUpdate(); // generating clouds is slow, so don't update timer in this frame
   }
   static void Save(CloudImages &ci) {ci.cloud.save("LocalData/cloud.gfx"); ci.detail.save("LocalData/detail.gfx");}
   static void Load(CloudImages &ci) {ci.cloud.load("LocalData/cloud.gfx"); ci.detail.load("LocalData/detail.gfx");}

   void create()
   {
      Gui   +=window  .create(Rect_LD(-D.w(), -D.h(), 0.5, 0.3f), "Cloud Images");
      window+=generate.create(Rect_C (window.rect.w()  /2, -0.08f, 0.35f, 0.06f), "Generate").func(Generate, T);
      window+=save    .create(Rect_C (window.rect.w()*1/3, -0.18f, 0.15f, 0.06f), "Save"    ).func(Save    , T);
      window+=load    .create(Rect_C (window.rect.w()*2/3, -0.18f, 0.15f, 0.06f), "Load"    ).func(Load    , T);

      Clouds.volumetric.set(&cloud, NULL);
   }
}Ci;
/******************************************************************************/
struct CloudParams
{
   Window    window;
   Slider    dns,  shadow,  col0,  col1,   scale,  curve,  height;
   Text     tdns, tshadow, tcol0, tcol1,  tscale, tcurve, theight;
   CheckBox  detail,  hi_res;
   Text     tdetail, thi_res;
   Button    save  ,  load  ;

   static void Save(CloudParams &cp) {   Clouds.volumetric.save("LocalData/volumetric_clouds");                 }
   static void Load(CloudParams &cp) {if(Clouds.volumetric.load("LocalData/volumetric_clouds"))cp.cloudsToGui();}

   void create()
   {
      Gui+=window.create(Rect_RD(D.w(), -D.h(), 0.6, 0.77), "Cloud Parameters");

      window+=tdns   .create(Vec2(0.15, -0.05), "Density"); window+=dns   .create(Rect_LU(0.3, -0.03, 0.25, 0.05), 3.0/6);
      window+=tshadow.create(Vec2(0.15, -0.10), "Shadow" ); window+=shadow.create(Rect_LU(0.3, -0.08, 0.25, 0.05), 0.5  );
      window+=tcol0  .create(Vec2(0.15, -0.15), "Color0" ); window+=col0  .create(Rect_LU(0.3, -0.13, 0.25, 0.05), 0.38 );
      window+=tcol1  .create(Vec2(0.15, -0.20), "Color1" ); window+=col1  .create(Rect_LU(0.3, -0.18, 0.25, 0.05), 0.7  );

      window+=tscale .create(Vec2(0.15, -0.30), "Scale" ); window+=scale .create(Rect_LU(0.3, -0.28, 0.25, 0.05), 0.5);
      window+=tcurve .create(Vec2(0.15, -0.35), "Curve" ); window+=curve .create(Rect_LU(0.3, -0.33, 0.25, 0.05), 0.5);
      window+=theight.create(Vec2(0.15, -0.40), "Height"); window+=height.create(Rect_LU(0.3, -0.38, 0.25, 0.05), 0.5);

      window+=tdetail.create(Vec2(0.20, -0.50), "Detail"         ); window+=detail.create(Rect(0.40, -0.53, 0.45, -0.48));
      window+=thi_res.create(Vec2(0.20, -0.57), "High Resolution"); window+=hi_res.create(Rect(0.40, -0.60, 0.45, -0.55));

      window+=save.create(Rect_C(0.17, -0.67, 0.18, 0.06), "Save").func(Save, T);
      window+=load.create(Rect_C(0.43, -0.67, 0.18, 0.06), "Load").func(Load, T);
   }
   void cloudsToGui()
   {
      dns   .set(Clouds.volumetric.density/6);
      shadow.set(Clouds.volumetric.shadow);
      col0  .set(Clouds.volumetric.dark_color  .avg());
      col1  .set(Clouds.volumetric.bright_color.avg());

      scale .set(LerpR(0.125f, 0.375f, Clouds.volumetric.scale ));
      curve .set(LerpR(0.000f, 0.060f, Clouds.volumetric.curve ));
      height.set(LerpR(0.000f, 1.540f, Clouds.volumetric.height));

      detail.set(Clouds.volumetric.detail!=NULL);
      hi_res.set(Clouds.volumetric.hi_res      );
   }
   void guiToClouds()
   {
      Clouds.volumetric.density=Lerp(0, 6, dns   ());
      Clouds.volumetric.shadow =Lerp(0, 1, shadow());
      Clouds.volumetric.dark_color  .set(col0());
      Clouds.volumetric.bright_color.set(col1());

      Clouds.volumetric.scale =Lerp(0.125f, 0.375f, scale ());
      Clouds.volumetric.curve =Lerp(0.000f, 0.060f, curve ());
      Clouds.volumetric.height=Lerp(0.000f, 1.540f, height());

      Clouds.volumetric.detail=(detail() ? &Ci.detail : NULL);
      Clouds.volumetric.hi_res=hi_res();
   }
}Cp;
/******************************************************************************/
void InitPre()
{
   App.name("Volumetric Clouds");
   App.flag=APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true).ambientPower(0.3f).hdrExposure(0.7);
}
Bool Init()
{
   Sky.atmospheric();
   Sun.image="Env/Sky/sun.gfx";

   // gui
   Opt.create();
   Ci .create();
   Cp .create();
   return true;
}
void Shut()
{
}
/******************************************************************************/
Bool Update()
{
   if(Kb.bp(KB_ESC))return false;
   CamHandle(0.01f, 500, Ms.hidden() ? CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT) : 0); // when mouse hidden operate the camera

   if(Kb.bp(KB_TAB))
   {
      Ms .              toggle(); // toggle mouse visibility when tab pressed
      Opt.window.visibleToggle();
      Ci .window.visibleToggle();
      Cp .window.visibleToggle();
   }

   Gui.update();
   Opt.update();
   Cp .guiToClouds();

   if(Kb.bp(KB_F10))Renderer.screenShots("screenshot/", "bmp");
   if(Kb.bp(KB_F11))D       .toggle();

   if(!Kb.b(KB_SPACE))Clouds.volumetric.update(Vec2(4,4)); // if space not pressed then update clouds position by given velocity

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE: Meshes("obj/terrain/0.mesh")->draw(MatrixIdentity); break; // draw terrain
   }
}
void Draw()
{
   Renderer(Render);
   Gui.draw();
   D.text(0, 0.9f, S+"Fps "+Time.fps());
   D.text(0, 0.8f, Ms.hidden() ? "Press Tab to toggle parameters" : "Press Tab to operate the camera");
}
/******************************************************************************/
