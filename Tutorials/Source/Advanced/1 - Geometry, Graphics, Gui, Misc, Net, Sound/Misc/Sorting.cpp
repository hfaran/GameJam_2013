/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Flt floats[8];

struct CustomData
{
   Byte bytes[100];
   Flt  real;
   Int  integer;
}data[8];

Memc<CustomData> memc;
/******************************************************************************

   Custom comparing functions must be of "Int name(C TYPE &a, C TYPE &b)" format
   they receive references to 2 custom data elements
   they must return:
   -1 if 'a' should be before 'b'
   +1 if 'a' should be after  'b'
    0 if 'a' is the same as   'b'

/******************************************************************************/
Int CompareCustomData(C CustomData &a, C CustomData &b)
{
   if(a.real<b.real)return -1;
   if(a.real>b.real)return +1;
                    return  0;
}
/******************************************************************************/
void InitPre()
{
   App.name("Sorting");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   REPA(floats)floats[i]     =RandomF(10); // fill 'floats'    with random values (0..10)
   REPA(data  )data  [i].real=RandomF(10); // fill 'data.real' with random values (0..10)
   REP (8     )memc  (i).real=RandomF(10); // fill 'memc.real' with random values (0..10)

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

   if(Kb.bp(KB_SPACE)) // sort when space pressed
   {
           Sort(floats, Elms(floats)                   ); // sort 'floats'
           Sort(data  , Elms(data  ), CompareCustomData); // sort custom data by giving pointer to data, number of elements and custom comparing function
      memc.sort(                      CompareCustomData); // sort custom data by giving memory block                        and custom comparing function
   }

   return true;
}
/******************************************************************************/
void Draw()
{
   D.clear(WHITE);
   D.text (0, 0.7f, "Press Space to sort");
   
   Str s="floats:  "; FREPA(floats){s+=floats[i]     ; s+="  ";} D.text(0,  0.2f, s); // draw 'floats'    in one string
       s="data:  "  ; FREPA(data  ){s+=data  [i].real; s+="  ";} D.text(0,  0.0f, s); // draw 'data.real' in one string
       s="memc:  "  ; FREPA(memc  ){s+=memc  [i].real; s+="  ";} D.text(0, -0.2f, s); // draw 'memc.real' in one string
}
/******************************************************************************/
