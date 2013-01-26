#include "stdafx.h"
#include "main.hpp"
#include <iostream>
#include <string>
#include <sstream>

Image image; // image object

void InitPre()
{
   App.name("Image");
   Paks.add("../data/engine.pak");
}

Bool Init()
{
   image.load("../data/obj/particles/star.gfx"); // load image from file
   return true;
}

void Shut()
{
}

Bool Update()
{
   if(Kb.bp(KB_ESC))return false;
   return true;
}

void Draw()
{
   D.clear(BLACK);

   // draw image
   image.draw(Rect(-0.5f, -0.5f, 0.5f, 0.5f)); // draw at given rectangle
}

