/******************************************************************************/
#include "stdafx.h"
#include "main.h"
/******************************************************************************/
Item::Item()
{
   name[0]=0;
   type=ITEM_NONE;
   power=0;
   icon=NULL;
}
/******************************************************************************/
// MANAGE
/******************************************************************************/
void Item::create(Game::ObjParams &obj)
{
   super::create(obj);

   // set name
   if(Param *p=obj.findParam("name"))Set(name, p->asText());

   // set type
   if(Param *p=obj.findParam("type"))type=p->asEnum();

   // set power
   if(Param *p=obj.findParam("power"))power=p->asFlt();

   // get icon
   for(Game::ObjParamsPtr op=&obj; op; op=op->base()) // iterate through all ObjParams, to find first with a valid icon
      if(CChar *op_file_name=Game::Objs.name(op)) // if current ObjParams is stored in a file (the file name is not NULL)
         if(icon=Images.ptrGet(GetExtNot(op_file_name)+".ico.gfx")) // if there exists an icon with the same name as ObjParams but with different extension
            break; // break
}
/******************************************************************************/
// DRAW
/******************************************************************************/
UInt Item::drawPrepare()
{
   if(Lit==this)SetHighlight(Color(48,48,48,0)); UInt modes=super::drawPrepare();
   if(Lit==this)SetHighlight();
   return modes;
}
/******************************************************************************/
void Item::drawIcon(C Vec2 &pos)
{
   if(icon)
   {
      icon->drawFit(Rect_LU(pos.x, pos.y, icon->x()*PIXEL_SIZE, icon->y()*PIXEL_SIZE));
   }
}
/******************************************************************************/
// IO
/******************************************************************************/
void Item::save(File &f)
{
   super::save(f);
   f<<name<<power<<type;
   f.putStr(icon.name());
}
Bool Item::load(File &f)
{
   if(super::load(f))
   {
      f>>name>>power>>type;
      icon.require(f.getStr());
      return true;
   }
   return false;
}
/******************************************************************************/
