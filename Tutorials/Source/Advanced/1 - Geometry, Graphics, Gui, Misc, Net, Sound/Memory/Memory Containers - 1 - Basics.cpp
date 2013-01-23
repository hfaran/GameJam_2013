/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
struct Data // Custom Data Structure
{
   Byte bytes[100];
   Int  integer;
   Flt  value;
};
/******************************************************************************/
// Memory Containers
Memc<Data> memc; // Continuous Based
Meml<Data> meml; // List       Based
/******************************************************************************/
void InitPre()
{
   App.name("Memory Containers");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   // adding elements
   {
      // Memc
      {
         Data &data=memc.New();
         data.integer=5;
      }
      // Meml
      {
         Data &data=meml.New();
         data.integer=5;
      }
   }

   // iterating through all elements
   {
      // Memc
      for(Int i=0; i<memc.elms(); i++)
      {
         Data &d=memc[i];
      }
      // Memc through better macro
      FREPA(memc)
      {
         Data &d=memc[i];
      }

      // Meml
      for(MemlNode *i=meml.first(); i; i=i->next())
      {
         Data &d=meml[i];
      }
      // Meml through macro
      MFREP(meml) // please note that here 'i' is not an 'Int' but a 'MemlNode*'
      {
         Data &d=meml[i];
      }
   }

   // removing elements
   {
      // Memc
      {
         memc.remove(0, true); // remove 0th element, second parameter determines keeping order (read more in the header)
      }
      // Meml
      {
         meml.remove(meml.first());
      }
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
   return true;
}
/******************************************************************************/
void Draw()
{
   D.clear(WHITE);
}
/******************************************************************************/
