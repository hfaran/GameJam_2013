/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Int      lights=3    ; // number of lights (1..3)
Material material[3] ; // light materials
Mesh     box         , // mesh box
         ball        , // mesh ball
         light       , // mesh light
         ball_stencil; // mesh ball (shadow stencil version)
Vec      ball_pos[8] , // ball  positions
        light_pos[3] ; // light positions
/******************************************************************************/
void SetShader()
{
   box .setShader();
   ball.setShader();
}
/******************************************************************************/
void InitPre()
{
   App.name("Lights Rendering Demo");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE;
   Paks.add("../data/engine.pak");

   D.set_shader=SetShader;
   D.full(true).ambientPower(0).bumpMode(BUMP_RELIEF).shadowSoft(1).shadowJitter(true);
}
/******************************************************************************/
Bool Init()
{
   // create materials
   MaterialPtr brick=Materials.ptrRequire("../data/mtrl/brick/0.mtrl");
   REPA(material)
   {
      Material &m=material[i];
      m.reset();
      m.glow=0.5f;
      m.ambient=1;
      switch(i)
      {
         case 0: m.color.v3().set(1   , 1, 0.5f); break;
         case 1: m.color.v3().set(0.5f, 1, 1   ); break;
         case 2: m.color.v3().set(0.5f, 1, 0.5f); break;
      }
      m.validate();
   }

   // create meshes
   box  .parts.New().base.create( Box(1    ), VTX_TEX0|VTX_NRM|VTX_TAN).reverse      (       ); // create mesh box , reverse it because it's meant to be viewed from inside
   ball .parts.New().base.create(Ball(0.15f), VTX_TEX0|VTX_NRM|VTX_TAN).weldVtxValues(VTX_POS); // create mesh ball, weld it's vertex positions (stencil shadows are very sensitive to even the smallest irregularities)
   light.parts.New().base.create(Ball(0.04f));

   // set mesh materials, rendering versions and bounding boxes
   box  .setMaterial( brick      ).setRender().setBox();
   ball .setMaterial( brick      ).setRender().setBox();
   light.setMaterial(&material[0]).setRender().setBox();

   // create stencil shadow version of ball mesh
   ball_stencil.create(ball).toStencilShadow();

   // set random positions
   REPA(ball_pos)ball_pos[i]=Random(Box(0.9f));

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
   CamHandle(0.01f, 10, CAMH_ZOOM|(Ms.b(1)?CAMH_MOVE:CAMH_ROT));

   // update light positions when space is not pressed
   if(!Kb.b(KB_SPACE))
   {
      static Flt t; t+=Time.d()/2;
      light_pos[0].set(0, 0, 0.3f); light_pos[0]*=Matrix3().setRotateX(t).rotateY(t/2   ).rotateZ(t/3   );
      light_pos[1].set(0, 0, 0.6f); light_pos[1]*=Matrix3().setRotateX(t).rotateY(t/1.5f).rotateZ(t/2.5f);
      light_pos[2].set(0, 0, 0.5f); light_pos[2]*=Matrix3().setRotateX(t).rotateY(t/1.3f).rotateZ(t/3.2f);
   }

   // change settings
   if(Kb.c('1'))lights=1;
   if(Kb.c('2'))lights=2;
   if(Kb.c('3'))lights=3;

   if(Kb.c('q'))D.shadowMode(SHADOW_NONE   );
   if(Kb.c('w'))D.shadowMode(SHADOW_MAP    );
   if(Kb.c('e'))D.shadowMode(SHADOW_STENCIL);

   if(Kb.c('a'))D.shadowSoft(0);
   if(Kb.c('s'))D.shadowSoft(1);

   if(Kb.c('z'))D.bumpMode(BUMP_FLAT    );
   if(Kb.c('x'))D.bumpMode(BUMP_NORMAL  );
   if(Kb.c('c'))D.bumpMode(BUMP_PARALLAX);
   if(Kb.c('v'))D.bumpMode(BUMP_RELIEF  );

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         // solid objects
                       box .draw(MatrixIdentity);
         REPA(ball_pos)ball.draw(Matrix(ball_pos[i]));

         // lights
         switch(lights)
         {
            case 1:
               LightPoint(0.50f, light_pos[0], Vec(1)).add();
            break;

            case 2:
               LightPoint(0.45f, light_pos[0], Vec(1,0.5f,0)).add();
               LightPoint(0.45f, light_pos[1], Vec(0,0.5f,1)).add();
            break;

            case 3:
               LightPoint(0.40f, light_pos[0], Vec(1,0.3f,0)).add();
               LightPoint(0.40f, light_pos[1], Vec(0,0.3f,1)).add();
               LightPoint(0.40f, light_pos[2], Vec(0,0.4f,0)).add();
            break;
         }

         // light meshes
                                                  light.draw(Matrix(light_pos[0]));
         if(lights>=2){MaterialLock=&material[1]; light.draw(Matrix(light_pos[1])); MaterialLock=NULL;} // use 'MaterialLock' instead of Mesh::setMaterial because it's faster
         if(lights>=3){MaterialLock=&material[2]; light.draw(Matrix(light_pos[2])); MaterialLock=NULL;}
      }break;

      case RM_SHADOW:
      {
         REPA(ball_pos)ball.drawShadow(Matrix(ball_pos[i]));
      }break;

      case RM_STENCIL_SHADOW:
      {
         REPA(ball_pos)ball_stencil.drawStencilShadow(Matrix(ball_pos[i]));
      }break;
   }
}
void Draw()
{
   Renderer(Render);
}
/******************************************************************************/
