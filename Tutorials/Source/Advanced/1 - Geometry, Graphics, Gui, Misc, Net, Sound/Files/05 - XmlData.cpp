/******************************************************************************/
#include "stdafx.h"
/******************************************************************************

   'XmlData' is meant for automatic loading and saving data in xml format.

/******************************************************************************/
void Draw(XmlNode &node, Flt x, Flt &y) // draw obtained Xml Node
{
   TextStyle ts; ts.align.set(1,-1); ts.scale=0.076f;

   // node name and params
   Str s=S+"<"+node.name; FREPA(node.params)s+=S+" "+node.params[i].name+"=\""+node.params[i].value+'"'; s+=">"; D.text(ts, x, y, s); y-=0.1f;

   // node data and child nodes
   {
      x+=0.1f;

      if(node.data.elms())
      {
         s.clear(); FREPA(node.data)s+=node.data[i]+' '; D.text(ts, x, y, s); y-=0.1f;
      }
      FREPA(node.nodes)Draw(node.nodes[i], x, y);

      x-=0.1f;
   }

   // close node name
   D.text(ts, x, y, S+"</"+node.name+">"); y-=0.1f;
}
void Draw(XmlData &data) // draw obtained Xml Data
{
   Flt x=-D.w()+0.1f, y=D.h()-0.1f;
   FREPA(data.nodes)Draw(data.nodes[i], x, y);
}
/******************************************************************************/
// MAIN
/******************************************************************************/
XmlData data;
/******************************************************************************/
void InitPre()
{
   App.name("XmlData");
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   data.load("LocalData/xml.txt"); // automatically load Xml Data

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
   Draw(data);
}
/******************************************************************************/
