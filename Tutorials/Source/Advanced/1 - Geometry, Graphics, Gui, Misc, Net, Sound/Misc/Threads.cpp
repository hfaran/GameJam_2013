/******************************************************************************/
#include "stdafx.h"
/******************************************************************************

   Threads are programs (functions) which run simultaneously

/******************************************************************************/
// global integers
Int x,
    y[2];

// threads
Thread thread_x,
       thread_y0,
       thread_y1;
/******************************************************************************

   Thread functions must be of "Bool name(Thread &thread)" format
   they return true when wan't to continue thread processing, and false when wan't to stop them
   as input parameter thread functions receive reference to the Thread which calls them

/******************************************************************************/
Bool ThreadProc(Thread &thread_caller)
{
   x++;         // simple increase x value
   return true; // continue processing
}

Bool ThreadProcWithUserData(Thread &thread_caller)
{
   y[(UIntPtr)thread_caller.user]++; // make use of thread_caller 'user' user data, and increase wanted 'y' (it will be y[0] or y[1])
   return true;                  // continue processing
}
/******************************************************************************/
void InitPre()
{
   App.name("Threads");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   // create threads
   thread_x .create(ThreadProc            , 0, 0, 100        ); // thread_x  will use 'ThreadProc'             as it's procedure, and it will be called once in 100 miliseconds
   thread_y0.create(ThreadProcWithUserData, 0, 0, 100, Ptr(0)); // thread_y0 will use 'ThreadProcWithUserData' as it's procedure,     it will be called once in 100 miliseconds, and it will use '0' as it's user data
   thread_y1.create(ThreadProcWithUserData, 0, 0, 100, Ptr(1)); // thread_y1 will use 'ThreadProcWithUserData' as it's procedure,     it will be called once in 100 miliseconds, and it will use '1' as it's user data
   return true;
}
/******************************************************************************/
void Shut()
{
   // delete threads
   thread_x .del();
   thread_y0.del();
   thread_y1.del();
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

   // draw values
   D.text(0, 0.2f, S+"x: "   +x   );
   D.text(0, 0.1f, S+"y[0]: "+y[0]);
   D.text(0, 0.0f, S+"y[1]: "+y[1]);
}
/******************************************************************************/
