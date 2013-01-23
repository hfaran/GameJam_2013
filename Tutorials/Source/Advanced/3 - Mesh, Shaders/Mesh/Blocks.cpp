/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Blocks          blocks;
BlocksOcclusion blocks_occlusion; // helper object used for calculating blocks ambient occlusion
/******************************************************************************/
void InitPre()
{
   App.name("Blocks");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE|APP_MAXIMIZABLE|APP_MINIMIZABLE;
   DataPath("../data");
   Paks.add("engine.pak");

   Cam.dist=10;
   Cam.at.set(4,4,4);
   
   Sky.atmospheric();
}
/******************************************************************************/
Bool Init()
{
   blocks          .create(8); // create blocks with 8x8 XZ resolution, Y dimension will have no limit
   blocks_occlusion.create(2); // create blocks occlusion with 2 blocks range

   // initialize blocks with random settings
   REPD(x, 8)
   REPD(y, 8)
   REPD(z, 8)blocks.set(x, y, z, Materials.ptrRequire(Random(2) ? "" : "mtrl/brick/0.mtrl"));

   // build blocks full mesh
   blocks.buildMesh(1.0f, &blocks_occlusion, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

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
   CamHandle(0.01f, 50, CAMH_ZOOM|(Ms.b(1) ? CAMH_MOVE : CAMH_ROT));
   
   // dynamically adjust the blocks
   Int x=Random(8),
       y=Random(8),
       z=Random(8);
   blocks.set(x, y, z, Materials.ptrRequire(Random(2) ? "" : "mtrl/brick/0.mtrl"));

   // update blocks mesh, this time don't use 'buildMesh' (because it builds full mesh)
   // now we'll use 'updateMesh' which will update the mesh only in specified box area
   blocks.updateMesh(1.0f, &blocks_occlusion, BoxI(VecI(x,y,z)), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         blocks.draw(MatrixIdentity); // draw blocks

         LightDir(Cam.matrix.z, 1-D.ambientColor()).add();
      }break;
   }
}
/******************************************************************************/
void Draw()
{
   Renderer(Render);
}
/******************************************************************************/
