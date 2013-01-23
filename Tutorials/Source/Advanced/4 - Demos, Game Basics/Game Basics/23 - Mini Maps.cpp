/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/_enums.h"
/******************************************************************************/
Flt MapRadius=16;
/******************************************************************************/
void InitPre()
{
   App.name("Mini Maps");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");
   Cam.dist=16;
   Cam.pitch=-1.2f;

   D.full(true).hpRt(true).ambientPower(0.3f);
}
/******************************************************************************/
Bool Init()
{
   Physics.create(CSS_NONE, true, "../Installation/PhysX");
   Sky    .atmospheric();
   Sun.image="Env/Sky/sun.gfx"; Sun.light_color=1-D.ambientColor();

   // create the world
   Game::World.init()
              .New ("world/path.world");

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

   CamHandle(0.1f, 100, CAMH_ZOOM|(Ms.b(0)?CAMH_MOVE_XZ:Ms.b(1)?CAMH_MOVE:CAMH_ROT));
   Game::World.update(Cam.at);

   if(Kb.b(KB_Q))MapRadius/=1+Time.d();
   if(Kb.b(KB_E))MapRadius*=1+Time.d();

   return true;
}
/******************************************************************************/
void DrawMiniMap(C Rect &rect, C Vec2 &world_center_position, Flt radius)
{
   Vec2 world_min=world_center_position-radius, // minimum coordinates of world position drawn on the mini map
        world_max=world_center_position+radius; // maximum coordinates of world position drawn on the mini map

   Vec2 image_min   =Game::World.mini_map.worldToImage(world_min),
        image_max   =Game::World.mini_map.worldToImage(world_max),
        image_center=Game::World.mini_map.worldToImage(world_center_position);

   VecI2 image_mini=Floor(image_min), // minimum indexes of used image maps
         image_maxi=Floor(image_max); // maximum indexes of used image maps

   Vec2 image_size=rect.size()/(image_max-image_min); // size of a single map image on the screen

   // iterate through all image maps to be displayed
   for(Int y=image_mini.y; y<=image_maxi.y; y++)
   for(Int x=image_mini.x; x<=image_maxi.x; x++)
   {
      // calculate screen position of the (x,y) image map
      Vec2 image_pos=rect.center() - image_center*image_size;
           image_pos.x+=x*image_size.x;
           image_pos.y+=y*image_size.y;

      // prepare the screen rectangle of the (x,y) image map
      Rect_LD map_image_rect(image_pos, image_size.x, image_size.y);

      // draw the image
      Image &image=Game::World.mini_map(VecI2(x, y)); // access the image map from the world manager
      if(    image.is())image.drawMask(WHITE, Color(0,0,0,0), map_image_rect, *Images("gfx/mini map alpha.gfx"), rect); // if the image map is valid then draw it on the screen using masked drawing
   }
   
   rect.center().draw(WHITE);
}
/******************************************************************************/
void Render()
{
   Game::World.draw();
}
void Draw()
{
   Renderer(Render);
   
   SetMatrix();
   Box(1, Cam.at).draw(WHITE);
   D.clearZ();
   Matrix(Cam.at).draw();
   
   DrawMiniMap(Rect_RU(D.w(), D.h(), 0.5f, 0.5f).extend(-0.05f), Cam.at.xz(), MapRadius);
   D.text(0, 0.9f, S+"Press Q/E to modify Map Radius: "+MapRadius);
   D.text(0, 0.8f, S+"Press LMB to move camera around");
}
/******************************************************************************/
