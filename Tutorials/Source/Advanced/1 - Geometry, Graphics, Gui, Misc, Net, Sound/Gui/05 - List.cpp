/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
struct Struct // custom structure
{
   Flt  f;
   Int  i;
   Byte b;
}data[10]= // sample data array
{
   {1.0f, 15, 4},
   {2.0f, 25, 3},
   {3.0f, 35, 2},
   {4.0f, 45, 1},
};
ListColumn list_column[]= // gui list column (stores information about structure format)
{
   ListColumn(MEMBER(Struct, f), 0.3f, L"Float"  ), // column describing 'f' member in 'Struct' structure, width of column=0.3, name="Float"
   ListColumn(MEMBER(Struct, i), 0.3f, L"Integer"), // column describing 'i' member in 'Struct' structure, width of column=0.3, name="Integer"
   ListColumn(MEMBER(Struct, b), 0.2f, L"Byte"   ), // column describing 'b' member in 'Struct' structure, width of column=0.2, name="Byte"
};
/******************************************************************************/
Window       window; // gui window
Region       region; // gui region
List<Struct> list  ; // gui list
/******************************************************************************/
void InitPre()
{
   App.name("List");
   Paks.add("../data/engine.pak");
   D.mode(800,600);
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   Gui   +=window.create(Rect(-0.5f , -0.4f, 0.5f ,  0.4f),"Window with list");
   window+=region.create(Rect( 0.05f, -0.6f, 0.95f, -0.1f));                        // create region
   region+=list  .create(list_column, Elms(list_column)).setData(data, Elms(data)); // create list with 'list_column' columns and 'data' data

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
   return true;
}
/******************************************************************************/
void Draw()
{
   D  .clear(WHITE);
   Gui.draw ();

   if(Struct *cur=list())D.text(0, -0.7f, S+"current elements Integer value: "+cur->i);
                         D.text(0, -0.8f, S+"current element (on the list): " +list.cur);
                         D.text(0, -0.9f, S+"current element (in the data): " +list.visToAbs(list.cur));
}
/******************************************************************************/
