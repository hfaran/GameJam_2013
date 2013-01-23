/******************************************************************************/
#include "stdafx.h"
/******************************************************************************

   This tutorial presents a sample ConnectionServer,
      which receives data from clients, and then sends the same data to all other clients.

/******************************************************************************/
STRUCT(Client , ConnectionServer::Client)
//{
   virtual Bool update(); // override client update method
};
/******************************************************************************/
ConnectionServer server;
/******************************************************************************/
Bool Client::update()
{
   if(super::update())
   {
      if(connection.receive(0)) // if received any data
      {
         // send it to all other clients
         server.clients.lock(); // lock client map
         REPA(server.clients) // iterate all clients
         {
            if(Client *client=CAST(Client, &server.clients.lockedData(i)))
               if(this!=client) // don't send back where it came from
            {
               connection.data.pos(0); // set received data position to start
               client->connection.dataFull(connection.data, connection.data.size()); // send received data to other client
            }
         }
         server.clients.unlock(); // unlock client map
      }
      return true;
   }
   return false;
}
/******************************************************************************/
void InitPre()
{
   App.name("Connection Server");
   App.flag=APP_WORK_IN_BACKGROUND; // specify work in background flag to work also when not focused
   Paks.add("../data/engine.pak");
   D.mode(400,300);
}
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;
   DefaultTextStyle.scale*=2;

   if(!server.create())Exit("Can't create ConnectionServer"); // create server

   server.clients.replaceClass<Client>(); // use custom client class

   return true;
}
/******************************************************************************/
void Shut()
{
   server.del();
}
/******************************************************************************/
Bool Update()
{
   if(Kb.bp(KB_ESC))return false;

   server.update(); // update server, this accepts new clients and processes all existing

   return true;
}
/******************************************************************************/
void Draw()
{
   D.clear(WHITE);
   D.text(0, 0.8f, S+"Clients: "       +server.clients.elms());
   D.text(0, 0.6f, S+"Local Address: " +server.addressLocal ().asText());
 //D.text(0, 0.4f, S+"Global Address: "+server.addressGlobal().asText());
}
/******************************************************************************/
