/******************************************************************************/
#include "stdafx.h"
/******************************************************************************

   Properties are helper functions for manipulating class member values using Gui controls.

   In this tutorial we're going to present how to use Properties on a Sample Class and its Object.

/******************************************************************************/
enum SAMPLE_ENUM // Sample Enum
{
   SE_ENUM_0,
   SE_ENUM_1,
   SE_ENUM_2,
};
CChar8 *enum_names[]=
{
   "SE_ENUM_0",
   "SE_ENUM_1",
   "SE_ENUM_2",
};
struct SampleClass
{
   Bool        bool_member;
   Int          int_member;
   Flt          flt_member;
   SAMPLE_ENUM enum_member;
   Color      color_member;

   Int  privateMember(     )C {return private_member  ;}
   void privateMember(Int x)  {       private_member=x;}

   SampleClass()
   {
         bool_member=false;
          int_member=0;
          flt_member=0;
         enum_member=SE_ENUM_0;
        color_member=WHITE;
      private_member=0;
   }
   
private:
   Int private_member;
};
/******************************************************************************/
static void PrivateMember(  SampleClass &object, C Str &text) {         object.privateMember(TextInt(text));}
static Str  PrivateMember(C SampleClass &object             ) {return S+object.privateMember(             );}
/******************************************************************************/
Memx<Property> props ; // container of properties (each property will be assigned to exactly one class member)
Window         window; // window containing the properties
SampleClass    object; // object of the sample class
/******************************************************************************/
void InitPre()
{
   App.name("Properties - Class Member Editing");
   App.x=0;
   App.y=0;
   DataPath("../Data");
   Paks.add("engine.pak");
   D.mode(1024,768);
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   Gui+=window.create(Rect_C(0, 0, 0.8f, 0.55f), "Properties"); // create window

   // setup properties
   props.New().create(MEMBER(SampleClass,  bool_member),  "bool_member");                                        // set property for '   bool_member'
   props.New().create(MEMBER(SampleClass,   int_member),   "int_member");                                        // set property for '    int_member'
   props.New().create(MEMBER(SampleClass,   flt_member),   "flt_member").range   (0, 1);                         // set property for '    flt_member' and optionally clamp the values range
   props.New().create(MEMBER(SampleClass,  enum_member),  "enum_member").setEnum (enum_names, Elms(enum_names)); // set property for '   enum_member'
   props.New().create(MEMBER(SampleClass, color_member), "color_member").setColor();                             // set property for '  color_member'
   props.New().create(MemberDesc(DATA_INT).setFunc(PrivateMember, PrivateMember), "private_member");             // set property for 'private_member'

   // add properties to the window
   static TextStyle text_ds; text_ds=Gui.ts_text; text_ds.align.set(-1,0); // set text draw settings
   AddProperties(props, window, Vec2(0.04f,-0.04f), 0.06f, 0.3f, &text_ds);

   // initialize GUI values from the Class Object
   REPAO(props).toGui(&object);

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

   // set Class Object values from the GUI
   REPAO(props).fromGui(&object);

   return true;
}
/******************************************************************************/
void Draw()
{
   D.clear(WHITE);
   
   // draw the object member values on the screen to visualize them in each frame
   D.text(0, 0.9f, S+   "bool_member: "+object. bool_member);
   D.text(0, 0.8f, S+    "int_member: "+object.  int_member);
   D.text(0, 0.7f, S+    "flt_member: "+object.  flt_member);
   D.text(0, 0.6f, S+   "enum_member: "+object. enum_member);
   D.text(0, 0.5f, S+  "color_member: "+object.color_member.asVecB4());
   D.text(0, 0.4f, S+"private_member: "+object.privateMember());

   Gui.draw();
}
/******************************************************************************/
