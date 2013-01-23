/******************************************************************************/
#include "stdafx.h"
#include "main.h"
/******************************************************************************/
void Teleport::create(Game::ObjParams &obj)
{
   super::create(obj);
   
   target_world=obj.getParam("teleport").asText();
}
/******************************************************************************/
void Teleport::save(File &f)
{
   super::save(f);
   f.putStr(target_world);
}
Bool Teleport::load(File &f)
{
   if(super::load(f))
   {
      f.getStr(target_world);
      return true;
   }
   return false;
}
/******************************************************************************/
