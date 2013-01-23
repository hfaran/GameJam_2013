/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
void InitPre()
{
   App.name("Config");
   Paks.add("../data/engine.pak");

   // here you can set initial settings
   // they will be used in case "config.txt" file is not found or incomplete
   // ..

   if(ConfigLoad("LocalData/config.txt")) // here settings are loaded from "config.txt"
   {
      // if settings were loaded properly, you can optionally override them here
      // ..
   }
}
/******************************************************************************/
Bool Init()
{
   return true;
}
/******************************************************************************/
void Shut()
{
   ConfigSave("LocalData/config.txt"); // here settings are saved to "config.txt"
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
