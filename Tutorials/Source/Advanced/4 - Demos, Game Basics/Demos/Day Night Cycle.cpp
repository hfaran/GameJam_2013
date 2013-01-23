/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Mesh terrain; // terrain mesh
Flt  hour   ; // current hour
/******************************************************************************/
void InitPre()
{
   App.name("Day Night Cycle");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.full(true).ambientPower(0.2f).hpRt(true).shadowSoft(1);
}
/******************************************************************************/
Bool Init()
{
   Cam.dist=3;
   Cam.at.set(0,2,0);
   Sky.atmospheric();
   Sun.image="Env/Sky/sun.gfx";

   // load terrain mesh
   terrain.load("obj/terrain/0.mesh");

   // set initial hour
   hour=5;

   // load clouds
   Clouds.layered.set(3, Images("Env/Cloud/0.gfx"));
   REPAO(Clouds.layered.layer).velocity*=4;

   return true;
}
/******************************************************************************/
void Shut()
{
}
/******************************************************************************/
static Vec Col(Byte r, Byte g, Byte b)
{
   return Vec(r,g,b)/255;
}
struct TimeLight
{
   Flt hour;
   Vec horizon, sky;
}tl[]=
{
   { 0, Col( 30, 40, 56), Col( 12, 16, 23)},
   { 4, Col( 54, 77,108), Col( 22, 29, 45)},
   { 6, Col( 97,113,136), Col( 61, 87,124)},
   { 8, Col(110,136,152), Col( 78,107,149)},
   {12, Col(121,142,164), Col( 70,107,161)},
   {15, Col(121,137,154), Col( 58, 95,144)},
   {20, Col(130,130,110), Col( 59, 82,126)},
   {21, Col(111,111,105), Col( 41, 57, 89)},
   {22, Col( 72, 80,101), Col( 26, 30, 41)},
};
static void GetTimeLight(Flt hour, TimeLight tl[], Int tls, Vec &horizon, Vec &sky)
{
   if(tls)
   {
      hour=Frac(hour, 24);
      Int prev; for(prev=tls-1; prev>=0; prev--)if(tl[prev].hour<hour)break; if(prev<0)prev+=tls;
      Int next=(prev+1)%tls;

      Flt prev_hour=tl[prev].hour; if(     hour< prev_hour)     hour+=24;
      Flt next_hour=tl[next].hour; if(next_hour<=prev_hour)next_hour+=24;
      Flt step=LerpRS(prev_hour, next_hour, hour);

      horizon=Lerp(tl[prev].horizon, tl[next].horizon, step);
      sky    =Lerp(tl[prev].sky    , tl[next].sky    , step);
   }else
   {
      horizon.zero();
      sky    .zero();
   }
}
/******************************************************************************/
Bool Update()
{
   if(Kb.bp(KB_ESC))return false;
   CamHandle(0.1f, 100, CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT)); // move camera on right mouse button

   // update hour
   hour+=Time.d()*2;

   // set sun position
   SinCos(Sun.pos.y, Sun.pos.x, hour/24*PI2-PI_2); Sun.pos.z=0;
   Sun.pos*=Matrix3().setRotateY(PI-PI_4);

   // tweak the sun to rise earlier, and set later
   Sun.pos.y+=0.4f;
   Sun.pos.normalize();

   // set sun color
   Sun.light_color=(1-D.ambientColor())*Sat(Cbrt(Sun.pos.y));

   // set sun rays and highlight
   Flt rays=Sqrt(Sun.pos.y);
   Sun.rays_color     =Lerp(0.02f, 0.14f, rays);
   Sun.highlight_front=Lerp(0.80f, 0.20f, rays);

   // set sky color
   Vec horizon, sky; GetTimeLight(hour, tl, Elms(tl), horizon, sky);
   Sky.atmosphericColor(Vec4(horizon,1), Vec4(sky,1));

   // update clouds
   Clouds.layered.update();
   REP(Clouds.layered.layers())Clouds.layered.layer[i].color=ColorBrightness(horizon.max()*(255.0f/164));

   Clouds.layered.set(Ms.b(0) ? 0 : 3);

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE: terrain.draw      (MatrixIdentity); break;
      case RM_SHADOW : terrain.drawShadow(MatrixIdentity); break;
   }
}
void Draw()
{
   Renderer(Render);
   D.text(0, 0.9f, S+"Hour "+(Trunc(hour)%24));
}
/******************************************************************************/
