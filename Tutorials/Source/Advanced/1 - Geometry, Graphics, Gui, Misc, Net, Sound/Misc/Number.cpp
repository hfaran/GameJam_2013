/******************************************************************************/
#include "stdafx.h"
/******************************************************************************

   'Number' is a number able to store very big values with big precision (Int or Real)

/******************************************************************************/
Number n0, n1, n2, n3, n4;
/******************************************************************************/
void InitPre()
{
   App.name("Number");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   // assign 1.0
   n0=1.0;
   // do some big operations
   n0*=100000; n0.sqrt(); n0+=123.456;
   n0*=100000; n0.sqrt(); n0+=123.456;
   n0*=100000; n0.sqrt(); n0+=123.456;
   // revert operations
   n0-=123.456; n0.sqr(); n0/=100000;
   n0-=123.456; n0.sqr(); n0/=100000;
   n0-=123.456; n0.sqr(); n0/=100000;
   // 'n0' should be back to 1.0

   // calculate sqrt(sqrt(sqrt(1000000000000000000000000000000.)))
   n1=Str("1000000000000000000000000000000.");
   n1.sqrt();
   n1.sqrt();
   n1.sqrt();

   // NOTE: Number can be internally an Int or Real (check Number::real)
   // any operation between Int and Int results also in Int (any other in Real)
   // when calculating Number(10)/3 you'll get 3 instead of 3.333..
   // to convert Number to be Int or Real use toInt() and toReal() methods
   {
      n2=Number(10  )/3; // Int /Int = Int  (3)
      n3=Number(10.0)/3; // Real/Int = Real (3.333..)

      n4=10;       // Int
      n4.toReal(); // convert to Real
      n4/=3;       // Real/Int = Real (3.333..)
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
   D.text (0, 0.6f, S+"n0 = "+n0.asDbl());
   D.text (0, 0.4f, S+"n1 = "+n1.asDbl());
   D.text (0, 0.2f, S+"n2 = "+n2.asDbl());
   D.text (0, 0.1f, S+"n3 = "+n3.asDbl());
   D.text (0, 0.0f, S+"n4 = "+n4.asDbl());
}
/******************************************************************************/
