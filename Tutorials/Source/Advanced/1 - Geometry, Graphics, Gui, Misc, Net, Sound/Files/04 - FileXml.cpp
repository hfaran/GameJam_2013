/******************************************************************************/
#include "stdafx.h"
/******************************************************************************

   FileXml is meant for manual parsing xml files.

/******************************************************************************/
struct XmlTag // custom xml node structure stored in a sample xml file
{
   Int          par_int;
   Flt          par_flt;
   Str          par_str,
                data   ;
   Memb<XmlTag> tags   ;

   void load(FileXml &f); // load from xml file

   void draw(Flt x, Flt &y); // draw contents of the node

   XmlTag()
   {
      par_int=0;
      par_flt=0;
   }
};
struct XmlTags
{
   Memb<XmlTag> tags;

   Bool load(Str name); // load data from 'name' file, false on fail

   void draw();
};
/******************************************************************************/
// XML TAG
/******************************************************************************/
void XmlTag::load(FileXml &f)
{
   for(f.begin(); f.level();)switch(f.type())
   {
      case XML_PARAM:
      {
         if(f.cur("par_int"))par_int=f.paramInt ();else
         if(f.cur("par_flt"))par_flt=f.paramReal();else
         if(f.cur("par_str"))par_str=f.param    ();
      }break;

      case XML_DATA:
      {
         if(data.is())data+=' '; // add optional space between the processed words
                      data+=f();
      }break;

      case XML_TAG:
      {
         if(f.cur("node"))tags.New().load(f); // if encountered child is of "node" type then load it
      }break;
   }
}
/******************************************************************************/
void XmlTag::draw(Flt x, Flt &y)
{
   TextStyle ts; ts.align.set(1,-1); ts.scale=0.076f; D.text(ts, x, y, S+"par_int=\""+par_int+"\" par_flt=\""+par_flt+"\" par_str=\""+par_str+"\" data=\""+data+"\"");
   x+=0.1f;
   y-=0.1f;
   FREPAO(tags).draw(x,y);
}
/******************************************************************************/
// XML DATA
/******************************************************************************/
Bool XmlTags::load(Str name)
{
   FileXml f;

   if(f.read(name)) // if file opened successfully
   {
      for(f.begin(); f.level();)switch(f.type()) // process file within its level
      {
         case XML_TAG:
         {
                 if(f.cur("node" ))tags .New().load(f); // if encountered child is of "node"  type then load it
          //else if(f.cur("node2"))tags2.New().load(f); // if encountered child is of "node2" type then load it, this code is commented, its purpose is only to show how to load other types of data
         }break;
      }
      return true;
   }
   return false;
}
/******************************************************************************/
void XmlTags::draw()
{
   Flt x=-D.w()+0.1f, y=D.h()-0.1f;
   FREPAO(tags).draw(x,y);
}
/******************************************************************************/
// MAIN
/******************************************************************************/
XmlTags data;
/******************************************************************************/
void InitPre()
{
   App.name("FileXml");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   data.load("LocalData/xml.txt");

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
   data.draw();
}
/******************************************************************************/
