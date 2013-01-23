/******************************************************************************/
#include "stdafx.h"
#include "Main.h" // include the main header, which includes all other headers
/******************************************************************************/
void InitPre()
{
   App.name("Simple Multi CPP Project");
   DataPath("../data");
   Paks.add("engine.pak");
}
Bool Init()
{
   StateIntro.set();
   return true;
}
void Shut()
{
}
/******************************************************************************/
Bool Update()
{
   return true;
}
void Draw()
{
}
/******************************************************************************/
