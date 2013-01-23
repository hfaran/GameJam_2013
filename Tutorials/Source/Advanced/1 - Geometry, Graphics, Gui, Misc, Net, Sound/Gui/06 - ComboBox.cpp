/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
ComboBox combobox_a, // combobox
         combobox_b; // combobox

struct ComboBoxElm // combobox element structure
{
   VecI2 vec;
}veci2[]=
{
   {VecI2(0,  0)},
   {VecI2(0, 10)},
   {VecI2(0, 20)},
   {VecI2(1,  0)},
   {VecI2(1, 10)},
   {VecI2(1, 20)},
};

Str ComboBoxElmFunc(C ComboBoxElm &elm) // function which transforms ComboBoxElm into a string
{
   return S+elm.vec.x+" x "+elm.vec.y;
}
/******************************************************************************/
void InitPre()
{
   App.name("ComboBox");
   App.flag=APP_FULL_TOGGLE;
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   // combobox A (from text-only elements)
   {
      static CChar8 *elm[]= // combobox elements (must be in constant memory address)
      {
         "First" ,
         "Second",
         "Third" ,
      };
      Gui+=combobox_a.create(Rect_C(-0.5f, 0, 0.4f, 0.08f), elm, Elms(elm)); // create combobox with simple elements (text-only)
   }

   // combobox B (from custom structure)
   {
      ListColumn lc[]= // list columns
      {
         ListColumn(ComboBoxElmFunc, 0.3f, L"Function"), // list column which uses custom function for data interpretation
      };
      Gui+=combobox_b.create(Rect_C(0.5f, 0, 0.4f, 0.08f)); // create combobox

      // assign custom elements
      combobox_b.setColumns(lc   , Elms(lc   ), true)  // set list columns
                .setData   (veci2, Elms(veci2)      ); // set list data
   }

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
   D.clear(WHITE);
   D.text (-0.5f, 0.1f, S+"combobox_a element: "+combobox_a()); // draw selected element
   D.text ( 0.5f, 0.1f, S+"combobox_b element: "+combobox_b()); // draw selected element
   Gui.draw();
}
/******************************************************************************/
