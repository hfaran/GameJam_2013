/******************************************************************************/
#include "stdafx.h"
#include "../../../../../data/enum/_enums.h"
/******************************************************************************

   This tutorial presents how to display mini maps of Worlds which aren't
      currently loaded by the World Manager.

/******************************************************************************/
Flt           MapScale   =1;
Vec2          MapPosition=0;
Rect_C        MapRect(0, 0, 2, 1.5f);
Game::MiniMap MiniMap;
/******************************************************************************/
void InitPre()
{
   App.name("Mini Maps");
   App.flag=APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");
}
/******************************************************************************/
Bool Init()
{
   MiniMap.load("world/path.world"); // load world mini map
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

   // drag map
   if(Ms.b(0))MapPosition-=Ms.dc()*MapScale;

   // zoom map
   if(Ms.wheel())
   {
      Vec2 delta_old=(Ms.pos()-MapRect.center())*MapScale;

      MapScale*=ScaleFactor(Ms.wheel()*-0.2f);

      Vec2 delta_new=(Ms.pos()-MapRect.center())*MapScale;

      MapPosition+=delta_old-delta_new;
   }

   return true;
}
/******************************************************************************/
void DrawMiniMap(C Rect &rect, C Vec2 &center, Flt scale)
{
   D.clip(&rect);

   rect.draw(BLACK);

   Vec2 image_min=center-scale*rect.size()/2,
        image_max=center+scale*rect.size()/2;

   VecI2 image_mini=Floor(image_min), // minimum indexes of used image maps
         image_maxi=Floor(image_max); // maximum indexes of used image maps

   Vec2 image_size=rect.size()/(image_max-image_min); // size of a single map image on the screen

   // iterate through all image maps to be displayed
   for(Int y=image_mini.y; y<=image_maxi.y; y++)
   for(Int x=image_mini.x; x<=image_maxi.x; x++)
   {
      // calculate screen position of the (x,y) image map
      Vec2 image_pos=rect.center() - center*image_size;
           image_pos.x+=x*image_size.x;
           image_pos.y+=y*image_size.y;

      // prepare the screen rectangle of the (x,y) image map
      Rect_LD map_image_rect(image_pos, image_size.x, image_size.y);

      // draw the image
      Image &image=MiniMap(VecI2(x,y)); // access the image map from the mini map
      if(    image.is())image.draw(map_image_rect);
   }

   rect.center().draw(WHITE);

   D.clip();
}
/******************************************************************************/
void Render()
{
   Game::World.draw();
}
void Draw()
{
   D.clear(BLUE);
   DrawMiniMap(MapRect, MapPosition, MapScale);
}
/******************************************************************************/
