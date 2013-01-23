/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
struct Base // base class
{
   Int value;

   virtual Int func()
   {
      // base codes
      return 0;
   }
};
STRUCT(Ext , Base) // extended class
//{
   virtual Int func()
   {
      // updated codes
      return 1;
   }
};
/******************************************************************************/
struct Abstract
{
   virtual int func()=NULL;
};

STRUCT(NonAbstract0 , Abstract)
//{
   virtual int func() {return 0;}
};

STRUCT(NonAbstract1 , Abstract)
//{
   virtual int func() {return 1;}
};
/******************************************************************************/
static Int CompareInt(C Int &a, C Int &b)
{
   return Sign(a-b);
}
/******************************************************************************/
void InitPre()
{
   App.name("Memory Containers Advanced");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   // replacing class
   {
      Memb<Base> memb;
      memb.replaceClass<Ext>(); // replace the class of elements stored in 'memb' (this however removes all previous elements from container)
      Int value=memb.New().func(); // 'value' will be 1
          value=0;
   }
   
   // automatic casting
   {
      Memb<Ext >  ext; ext.New();
      Memb<Base> &base=ext; // automatic casting to container of base elements
      if(base.elms())
      {
         Int value=base.first().func();
             value=0;
      }
   }

   // sorting
   {
      Memb<Int> memb;
      memb.add(5);
      memb.add(3);
      memb.add(7);
      memb.sort(CompareInt);
      Int value=memb[0];
          value=memb[1];
          value=memb[2];
          value=0;
   }

   // constant memory address for elements in all operations on 'Memx' memory container
   {
      Memx<Base> memx;
          REP(10)memx.New().value=0;      // create 10 elements with member 'value'=0
       Base &elm=memx.New(); elm.value=1; // create 1  element  with member 'value'=1 and remember elements memory address
          REP(10)memx.New().value=2;      // create 10 elements with member 'value'=2

      // remove all elements of "value!=1"
      {
         REPA(memx)if(memx[i].value!=1)memx.removeValid(i);
      }
      
      // after adding and removing elements, 'elm' memory address is still valid
      Int value=elm.value;
          value=0;
          
      // methods for checking if element is present in container
      Bool contains=memx.contains  (&elm);
                    memx.removeData(&elm);
           contains=memx.contains  (&elm);
           contains=false;
   }

   // storing elements of abstract classes
   {
      Int value;
      MembAbstract<Abstract> memb;
      memb.replaceClass<NonAbstract0>();
      value=memb.New().func();

      memb.replaceClass<NonAbstract1>();
      value=memb.New().func();
      
      value=0;
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
