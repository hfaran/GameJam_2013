
#include "stdafx.h"

void InitPre()
{
   App.name("Cache");
   Paks.add("../data/engine.pak");
}

Bool Init()
{
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

   Images("../data/obj/particles/star.gfx")->draw(Rect(-0.5f, -0.5f, 0.5f, 0.5f)); // get image from cache and draw given rectangle

   // for safer use, you can first check if resource exists using 'get' method
   if(Image *image=Images.get("non existing image.gfx")) // try to get non existing image, NULL will be returned since image doesn't exist
      image->draw(Rect(-0.5f, -0.5f, 0.5f, 0.5f)); // this won't occur

   // see what will happen if you want to use "non existing image.gfx" when space pressed
   if(Kb.b(KB_SPACE))
   {
      Images("non existing image.gfx")->draw(Rect(-0.5f, -0.5f, 0.5f, 0.5f)); // when space pushed an error will be shown and application will exit immediately
   }
}

