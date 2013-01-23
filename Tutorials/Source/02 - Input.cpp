
#include "stdafx.h"

Vec2 point; // point position

void InitPre()
{
   App.name("Input");
   Paks.add("../data/engine.pak");
}

Bool Init()
{
   DefaultTextStyle.color =BLACK; // here change the default text color
   DefaultTextStyle.shadow=0;     // here disable shadows
   return true;
}

void Shut()
{
}

Bool Update()
{
   if(Kb.bp(KB_ESC))return false;

   if(Kb.b(KB_LEFT ))point.x-=Time.d()/20; // move point left  when 'left  arrow' is enabled according to time delta
   if(Kb.b(KB_RIGHT))point.x+=Time.d()/20; // move point right when 'right arrow' is enabled according to time delta
   if(Kb.b(KB_DOWN ))point.y-=Time.d()/20; // move point down  when 'down  arrow' is enabled according to time delta
   if(Kb.b(KB_UP   ))point.y+=Time.d()/20; // move point up    when 'up    arrow' is enabled according to time delta

   if(Kb.bp(KB_Z))point.x-=0.1f; // when 'z' is pushed        , move point left
   if(Kb.br(KB_X))point.x+=0.1f; // when 'x' is released      , move point right
   if(Kb.bd(KB_C))point.y+=0.1f; // when 'c' is double clicked, move point up

   return true;
}

void Draw()
{
   D.clear(WHITE);

   D.dot(RED  , Ms.pos()); // draw red   dot at mouse cursor position
   D.dot(GREEN, point   ); // draw green dot at 'point' position
   
   if(Ms.b(0))D.dot(BLACK, -0.1f, 0.4f, 0.1f); // when 0th mouse button on, draw big black dot
   if(Ms.b(2))D.dot(BLACK,  0.1f, 0.4f, 0.1f); // when 1st mouse button on, draw big black dot


   D.text(0, 0.7f, S+"mouse : "+Ms.pos()); // draw mouse position
   D.text(0, 0.6f, S+"point : "+point   ); // draw point position
}

