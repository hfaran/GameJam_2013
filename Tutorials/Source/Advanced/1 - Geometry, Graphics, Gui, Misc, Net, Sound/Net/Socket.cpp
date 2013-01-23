/******************************************************************************/
#include "stdafx.h"
/******************************************************************************

   This tutorial presents the most basic socket usage.
   It needs to be launched few times, so multiple windows of it will appear on the desktop.

   Each of the program instances (windows) will create it's own 'player' and a socket (internet connection).

   At the start of the program, it tries to contact other instances (windows) by sending empty data to specific ports.
   When each of the program instances (windows) receive data from other instances, they store their address in a memory container, as a list of "external players"

   In the update each instance sends its own player position to all other instances, so they'll know the most recent player position.
   This way each instance receives data from other instances about their player positions.

/******************************************************************************/
#define BASE_PORT 10000
/******************************************************************************/
struct Player // player, contains only a position
{
   Vec2 pos; // position

   void draw(Color color) // draw
   {
      Circle(0.1f, pos).draw(color, true);
   }
   Player() // constructor
   {
      pos.zero();
   }
};
struct Peer // peers (external players from other program instances)
{
   SockAddr addr  ; // their socket address
   Player   player; // their player data
};
/******************************************************************************/
Player     player; // this program instance player
Socket     sock  ; // this program instance socket
Memb<Peer> peer  ; // container of external peers
/******************************************************************************/
void InitPre()
{
   App.name("Socket");
   App.flag=APP_WORK_IN_BACKGROUND; // specify work in background flag to work also when not focused
   Paks.add("../data/engine.pak");
   D.mode(400,300);
}
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;
   DefaultTextStyle.scale*=2;

   // create this instance socket (internet connection)
   {
      // try to initialize socket on one of the ports starting from BASE_PORT
      // some ports will not be accessible because other program instances may have already reserved them
      FREP(50)
      {
            sock.createUdp();
         if(sock.bind(SockAddr().setServer(BASE_PORT+i)))break; // 50 attempts, stops on first usable port
      }

      sock.block(false); // set non-blocking mode, this will disable waiting when receiving data through socket
   }

   // make contact with other sockets created by other program instances
   {
      SockAddr addr;        // socket address
      addr.ip("127.0.0.1"); // set    address ip to 'localhost', which is your computer
      FREP(50)              // now check 50 ports where possibly other instances of the tutorial have created a socket
      {
         addr.port(BASE_PORT+i);      // set port of the address
         if(addr.port()!=sock.port()) // if it's on a different port than our socket (which means that it's not us)
            sock.send(addr, NULL, 0); // send empty data just to contact the other socket to let it know that we exist
      }
   }

   return true;
}
/******************************************************************************/
void Shut()
{
   sock.del();
   peer.del();
}
/******************************************************************************/
Bool Update()
{
   if(Kb.bp(KB_ESC))return false;

   // update this instance player position
   if(Ms.b(0)) // when application is active (focused) and left mouse button pushed
   {
      player.pos+=Ms.dc(); // move player position
   }

   // send this instance player position to all external players
   REPA(peer)
   {
      sock.send(peer[i].addr, &player.pos, SIZE(player.pos));
   }

   // receive positions from other players
   for(SockAddr addr;;)
   {
      Vec2 pos;
      Int  rcv=sock.receive(addr, &pos, SIZE(pos)); // check if the socket receives any data
      if(  rcv<0)break;                             // if no data then break

      // here we've received some data from an external program instance
      {
         // first we'll check for its address, and store it on the "external player" list
         Peer *p=NULL;
         REPA(peer)if(peer[i].addr==addr){p=&peer[i]; break;} // check if it's already stored on the list
         if(!p)                                               // if not found in add it
         {
            p=&peer.New();
            p->addr=addr;
         }

         // now we've got the sender stored on the list, so we can check for any data that the external player is sending to us
         if(rcv==SIZE(pos))p->player.pos=pos; // if number of received data bytes is equal to Vec2 size then it's a new position of the external player
      }
   }

   return true;
}
/******************************************************************************/
void Draw()
{
   D.clear(WHITE);

   // draw players
                     player.draw(ColorI(        sock.port())); // draw this instance player  with color depending on its   socket port number
   REPA(peer)peer[i].player.draw(ColorI(peer[i].addr.port())); // draw external      players with color depending on their socket port number

   // draw instructions
   D.text(0, 0.88f, "Launch this tutorial multiple times");
   D.text(0, 0.74f, "Press LMB and move the Mouse");
}
/******************************************************************************/
