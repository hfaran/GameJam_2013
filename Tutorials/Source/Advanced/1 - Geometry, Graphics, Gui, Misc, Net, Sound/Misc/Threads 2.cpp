/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Flt time;
/******************************************************************************/
void Calculate(Dbl x)
{
   // perform lot's of numerical calculations
   REP(1000)
   REP(1000)
   {
      x =Cos(Sin(x*x+Sqrt(x)));
      x+=Pow(10+x, 10+x*5);
      x =Cbrt(Tan(x));
   }
}
/******************************************************************************/
void SingleThreadCalculate()
{
   Calculate(1);
   Calculate(2);
}
/******************************************************************************/
Bool ThreadProcA(Thread &thread)
{
   Calculate(1); // perform calculations
   return false; // stop the thread after calculations
}
Bool ThreadProcB(Thread &thread)
{
   Calculate(2); // perform calculations
   return false; // stop the thread after calculations
}
void MultiThreadCalculate()
{
   // create the threads
   Thread thread_a(ThreadProcA),
          thread_b(ThreadProcB);

   // wait for the threads to finish processing
   thread_a.wait();
   thread_b.wait();

   // delete threads
   thread_a.del();
   thread_b.del();
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

   if(Kb.bp(KB_1))
   {
      time=Time.curTime();      // get current time
      SingleThreadCalculate();  // perform calculations on one thread
      time=Time.curTime()-time; // get difference between remembered and current time
   }

   if(Kb.bp(KB_2))
   {
      time=Time.curTime();      // get current time
      MultiThreadCalculate();   // perform calculations on multiple threads
      time=Time.curTime()-time; // get difference between remembered and current time
   }
   return true;
}
/******************************************************************************/
void Draw()
{
   D.clear(WHITE);

   D.text(0, 0.9f, S+"Press 1 for single threaded calculations");
   D.text(0, 0.8f, S+"Press 2 for multi threaded calculations");

   D.text(0, 0, S+"Time spent for calculations : "+time);

   REP(3)
   {
      Flt  a=Time.time()*2+i*PI2/3;
      Tri2 t;
      t.p[0].set(0, 0);
      t.p[1].set(Cos(a      ), Sin(a      ));
      t.p[2].set(Cos(a+PI2/9), Sin(a+PI2/9));
      (t*0.2f-Vec2(0, 0.5f)).draw(GREY);
   }
}
/******************************************************************************/
