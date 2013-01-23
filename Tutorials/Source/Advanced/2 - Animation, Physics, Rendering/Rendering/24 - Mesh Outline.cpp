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

   void drawOutline(Flt alpha)
   {
      Color color=ColorAlpha(Color(material.color), alpha);
      ball.drawOutline(color, Matrix(pos));
   }
}obj[32];
/******************************************************************************/
void InitPre()
{
   App.name("Mesh Outline");
   App.flag=APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");
   D.full(true).ambientPower(0);
}
/******************************************************************************/
Bool Init()
{
   brick.require("mtrl/brick/0.mtrl");
   glass.require("mtrl/glass/0.mtrl");

   box .parts.New().base.create( Box(4   ), VTX_TEX0|VTX_NRM|VTX_TAN).reverse(); // create mesh box, reverse it because it's meant to be viewed from inside
   ball.parts.New().base.create(Ball(0.5f), VTX_TEX0|VTX_NRM|VTX_TAN)          ; // create mesh ball

   // set mesh materials, rendering versions and bounding boxes
   box .setMaterial(brick).setRender().setBox();
   ball.setMaterial(glass).setRender().setBox();

   // set objects
   REPAO(obj).create();

   // set camera
   Cam.dist=5;

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
   CamHandle(0.01f, 100, CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT));

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

      // when we wan't to use Mesh Outlining we need to process additional rendering mode 'RM_OUTLINE'
      case RM_OUTLINE:
         if(Kb.b(KB_SPACE)) // outline only when space pressed
         {
            Flt alpha=Frac(Time.time()*2, 2);
            if( alpha>1)alpha=2-alpha;
            REPAO(obj).drawOutline(alpha);
         }
      break;
   }
}
void Draw()
{
   Renderer(Render);
   D.text(0, 0.9f, "Press Space to outline the balls");
}
/******************************************************************************/
