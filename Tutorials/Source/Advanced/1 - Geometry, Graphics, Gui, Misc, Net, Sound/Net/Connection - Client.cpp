/******************************************************************************/
#include "stdafx.h"
/******************************************************************************

   This tutorial presents a sample Client Connection.

/******************************************************************************/
Connection connection; // connection for client/server connection
Str        data      ; // received data
/******************************************************************************/
void InitPre()
{
   App.name("Connection Client");
   App.flag=APP_WORK_IN_BACKGROUND; // specify work in background flag to work also when not focused
   Paks.add("../data/engine.pak");
   D.mode(400,300);
}
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;
   DefaultTextStyle.scale*=2;

   for(Int i=0xFFFF; i>0xFFFF-5; i--) // try 5 port attempts
   {
      SockAddr server; server.setLocal(i); // set target server address
      if(connection.clientConnectToServer(server))break; // if connected successfully then break
   }

   return true;
}
/******************************************************************************/
void Shut()
{
   connection.del();
}
/******************************************************************************/
Bool Update()
{
   if(Kb.bp(KB_ESC))return false;

   // move window on mouse button
   if(Ms.b(0))WindowMove(App.hwnd(), Ms.pixelDelta().x, Ms.pixelDelta().y);

   // check if received any data
   if(connection.receive(0))
   {
      connection.data.getStr(data); // read data as string
   }

   // send random text when space pressed
   if(Kb.bp(KB_SPACE))
   {
      File f; f.writeMem();
      f.putStr(S+"Random Text "+Random(1024));
      f.pos(0);
      connection.dataFull(f, f.size());
   }

   return true;
}
/******************************************************************************/
void Draw()
{
   D.clear(WHITE);
   if(connection.state()!=CONNECT_GREETED)
   {
      D.text(0, 0, S+"Invalid connection");
   }else
   {
      D.text(0, 0.8f, S+"Server Address: "+connection.address().asText());
      D.text(0, 0.6f, S+"Press Space to send random text");
      D.text(0, 0.0f, S+"Received Data: "+data);
   }
}
/******************************************************************************/
