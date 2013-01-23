/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
void InitPre()
{
   App.name("Random");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   Cam.dist=2;
   return true;
}
/******************************************************************************/
void Shut()
{
}
/******************************************************************************/
Bool Update()
{
   if(Kb.bp(KB_ESC  ))return false;
   if(Kb.b (KB_SPACE))CamHandle(0.1f, 10, CAMH_ZOOM|CAMH_ROT);
   return true;
}
/******************************************************************************/
void Draw()
{
   if(Kb.b(KB_SPACE)) // when space on draw random points in 3D
   {
      D.clear(BLACK);
      // here we're rendering 10 000 dots, such big number is more efficient to draw through VertexIndexBuffer (for more information about it check its tutorial in "advanced\graphics")
                VI.color(ColorAlpha(TURQ, 0.15f));  // set color
      REP(10000)VI.dot  (Random(Capsule(0.5f, 2))); // draw dot at random position in capsule
                VI.end  ();                         // finish buffered drawing
   }else
   {
      D.clear(WHITE);
      REP(1000)D.dot(BLUE, Random(Circle(0.5f                ))); // draw 1000 random points inside circle
      REP(1000)D.dot(RED , Random(Rect  (0.5f, -0.5f, 1, 0.5f))); // draw 1000 random points inside rectangle
      D.text(0, 0.7f, S+Random(4)+"  "+Random(10,15)+"  "+RandomF()+"  "+RandomF(10,15));
   }
   D.text(0, 0.9f, "Press Space to draw Random 3D Points");
}
/******************************************************************************/
