/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
MeshBase A, B, csg; // meshes

SHAPE_TYPE a_type   =SHAPE_BOX    , // A mesh type
           b_type   =SHAPE_CAPSULE; // B mesh type
Int        operation=SEL_SUB      ; // csg operation

Material material_red  ,
         material_green;
/******************************************************************************/
Str OperationText()
{
   switch(operation)
   {
      case SEL_SUB : return S+ShapeTypeName(a_type)+'-'+ShapeTypeName(b_type);
      case SEL_SUBR: return S+ShapeTypeName(b_type)+'-'+ShapeTypeName(a_type);
      case SEL_AND : return S+ShapeTypeName(a_type)+'&'+ShapeTypeName(b_type);
      case SEL_ADD : return S+ShapeTypeName(a_type)+'+'+ShapeTypeName(b_type);
      default      : return S+ShapeTypeName(a_type)+'?'+ShapeTypeName(b_type);
   }
}
/******************************************************************************/
void Create(MeshBase &mshb, SHAPE_TYPE type)
{
   switch(type)
   {
      case SHAPE_BOX    : mshb.create(Box    (      0.8f), VTX_NRM); break;
      case SHAPE_BALL   : mshb.create(Ball   (      1.0f), VTX_NRM); break;
      case SHAPE_CAPSULE: mshb.create(Capsule(0.6f, 3.0f), VTX_NRM); break;
      case SHAPE_TUBE   : mshb.create(Tube   (0.6f, 3.0f), VTX_NRM); break;
   }
}
/******************************************************************************/
void InitPre()
{
   App.name("Constructive Solid Geometry (CSG)");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");

   D.mode(1024,768).ambientPower(0);
   Cam.dist =4;
   Cam.pitch=-0.6f;
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.scale*=0.7f;
   Create(A, a_type);
   Create(B, b_type);
   material_red  .reset(); material_red  .color.v3().set(1.0f, 0.5f, 0.5f); material_red  .validate();
   material_green.reset(); material_green.color.v3().set(0.5f, 1.0f, 0.5f); material_green.validate();
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
   CamHandle(0.01f, 10, CAMH_ZOOM|(Ms.b(1) ? CAMH_MOVE : CAMH_ROT));

   // change operation type on 1,2,3,4 keys
   if(Kb.b(KB_1))operation=SEL_SUB ; // subtract         (A-B)
   if(Kb.b(KB_2))operation=SEL_SUBR; // subtract reverse (B-A)
   if(Kb.b(KB_3))operation=SEL_AND ; // and              (A&B)
   if(Kb.b(KB_4))operation=SEL_ADD ; // add              (A+B)

   // change A mesh type on q,w,e,r keys
   if(Kb.b(KB_Q))a_type=SHAPE_BOX    ;
   if(Kb.b(KB_W))a_type=SHAPE_BALL   ;
   if(Kb.b(KB_E))a_type=SHAPE_CAPSULE;
   if(Kb.b(KB_R))a_type=SHAPE_TUBE   ;

   // change B mesh type on a,s,d,f keys
   if(Kb.b(KB_A))b_type=SHAPE_BOX    ;
   if(Kb.b(KB_S))b_type=SHAPE_BALL   ;
   if(Kb.b(KB_D))b_type=SHAPE_CAPSULE;
   if(Kb.b(KB_F))b_type=SHAPE_TUBE   ;

   // create A b meshes
   Create(A, a_type);
   Create(B, b_type);
   
   // move B mesh
   Flt t=Time.time();
   B.move(Vec(Cos(t), 0, Sin(t)));

   // perform Csg 'operation' on 'A B' meshes and store result into 'csg' mesh
   Csg(A, B, operation, &csg);

   return true;
}
/******************************************************************************/
void Draw()
{
   D.clear(TURQ);
   LightDir(Cam.matrix.z).set();
   
   SetMatrix();

   if(Ms.b(0)) // when mouse button pressed draw original meshes
   {
      A.drawAuto(&material_red  );
      B.drawAuto(&material_green);
   }
   else // if not then draw Csg output
   {
      csg.drawAuto(NULL);
   }

   Flt y=0.94f, h=0.06f;
   D.text(0, y, "Press 1,2,3,4 to change operation"  ); y-=h;
   D.text(0, y, "Press q,w,e,r to change A mesh type"); y-=h;
   D.text(0, y, "Press a,s,d,f to change B mesh type"); y-=h;
   D.text(0, y, "Press LMB to view source meshes"); y-=h;
   D.text(0, y, S+"Operation: \""+OperationText()+'"');
}
/******************************************************************************/
