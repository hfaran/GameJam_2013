/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
PathFind    pf        ; // path finder
Memc<VecI2> path      ; // path
VecI2       start, end; // start and end position of wanted path
/******************************************************************************/
void InitPre()
{
   App.name("Pathfind");
   Paks.add("../data/engine.pak");
}
Bool Init()
{
   Image map;
   if(   map.ImportTry("../data/gfx/map/map.png", -1, IMAGE_SOFT))
   {
      pf.create(map.x(), map.y());
      REPD(y, pf.y())
      REPD(x, pf.x())
      {
         Color color=map.color(x,y); // get pixel color of loaded map
         pf.pixelFlag(x, y, (color!=BLACK) ? PFP_WALKABLE : 0); // set non black color for walkable pixels
      }
   }
   return true;
}
void Shut()
{
}
/******************************************************************************/
VecI2 ScreenToPixel(C Vec2 &screen)
{
   Int size=Max(pf.x(), pf.y())*4/3;
   return VecI2(Round((screen.x+D.h()*3/4)*size/2),
                Round((screen.y+D.h()*3/4)*size/2));
}
/******************************************************************************/
Bool Update()
{
   if(Kb.bp(KB_ESC))return false;

   // set starting position on LMB
   if(Ms.b(0))
   {
      start=ScreenToPixel(Ms.pos());
      pf.find(start, end, path);
   }

   // set ending position on RMB
   if(Ms.b(1))
   {
      end=ScreenToPixel(Ms.pos());
      pf.find(start, end, path);
   }

   return true;
}
/******************************************************************************/
void DrawPixel(Int x, Int y, Color color)
{
   Int size=Max(pf.x(), pf.y())*4/3;
   VI.dot(color, Vec2(x,y)*2/size-D.h()*3/4, 1.0f/size);
}
void Draw()
{
   D.clear(GREY);

   REPD(y, pf.y())
   REPD(x, pf.x())DrawPixel(x, y, FlagTest(pf.pixelFlag(x,y), PFP_WALKABLE) ? WHITE : BLACK); // draw map
   REPA(path     )DrawPixel(path[i].x, path[i].y, YELLOW);                                    // draw path              with yellow color
                  DrawPixel(start  .x, start  .y, GREEN );                                    // draw starting position with green  color
                  DrawPixel(end    .x, end    .y, RED   );                                    // draw ending   position with red    color
   VI.end();

   D.text(0, 0.9f, "Press LMB and RMB to set Start and End position");
}
/******************************************************************************/
