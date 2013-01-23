/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Str s;
/******************************************************************************/
void InitPre()
{
   App.name("CPU");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   // check which features are supported
   if(Cpu.flag()&CPU_MMX  )s+="MMX ";
   if(Cpu.flag()&CPU_3DNOW)s+="3dNow ";
   if(Cpu.flag()&CPU_SSE  )s+="SSE ";
   if(Cpu.flag()&CPU_SSE2 )s+="SSE2 ";
   if(Cpu.flag()&CPU_SSE3 )s+="SSE3 ";

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
   D.text (0, 0.3f, S+"Number of cores: "   +Cpu.cores());
   D.text (0, 0.1f, S+"Supported features: "+s);
}
/******************************************************************************/
