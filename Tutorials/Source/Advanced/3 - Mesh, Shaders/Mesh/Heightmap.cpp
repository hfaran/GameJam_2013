/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
Heightmap heightmap;
Mesh      mesh;
PhysPart  phys;
Actor     actor;
/******************************************************************************/
void Build(Heightmap &heightmap, Mesh &mesh, PhysPart &phys, Actor &actor) // build 'mesh', 'phys' and 'actor' from 'heightmap'
{
   // later we will create a new physical body, that's why we must first delete the actor, so the old physical body will no longer be used by any actors
   actor.del();

   // build mesh
   heightmap.build(mesh, 0, 6, true, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

   // adjust mesh scale and position
   Flt scale=32;
   Vec pos  (0,0,0);
   mesh.scaleMove(scale, pos);

   // simplify the mesh
   mesh.setBase  (           ); // set software version needed for simplification and later physical body creation
   mesh.simplify (0, 0, 0.05f); // simplify
   mesh.setRender(           ); // set rendering version from software version

   // create physical body
   MeshBase base; base.createPhys(mesh); base.simplify(0, 0, 0.05f); // create a 1 MeshBase from all MeshParts in mesh and simplify it again
   phys .createMesh(base); // create physical body from that MeshBase
   actor.create    (phys); // create actor         from physical body

   mesh.delBase(); // mesh software version is no longer needed
}
/******************************************************************************/
void InitPre()
{
   App.name("Heightmap");
   App.flag=APP_MS_EXCLUSIVE|APP_FULL_TOGGLE|APP_MAXIMIZABLE|APP_MINIMIZABLE;
   DataPath("../data");
   Paks.add("engine.pak");

   Cam.dist=24;
   Cam.pitch=-1.3f;
   Cam.at.set(16,0,16);

   Sky.atmospheric();
}
/******************************************************************************/
Bool Init()
{
   Physics.create(CSS_NONE, true, "../Installation/PhysX");

   heightmap.create(32, 0, Materials.ptrRequire("mtrl/grass/0.mtrl"), false, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL); // create heightmap

   Build(heightmap, mesh, phys, actor); // build mesh phys actor from heightmap

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

   // adjust the heightmap
   {
      // set material
      heightmap.setMaterial(Random(32), Random(32), Materials.ptrRequire(Random(2) ? "mtrl/grass/0.mtrl" : "mtrl/ground/0.mtrl"), 1.0f, NULL, 0, NULL, 0);

      // set height
      Vec2 pos; SinCos(pos.y, pos.x, Time.time()); pos=pos*8+16;
      REPD(x, heightmap.resolution())
      REPD(y, heightmap.resolution())
      {
         Flt d=Dist(Vec2(x,y), pos);
         heightmap.height(x, y, BlendGaus(d/10)*0.1f);
      }
   }

   // rebuild after making changes
   Build(heightmap, mesh, phys, actor);

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         mesh.draw(MatrixIdentity); // draw mesh

         LightDir(Cam.matrix.z, 1-D.ambientColor()).add();
      }break;
   }
}
/******************************************************************************/
void Draw()
{
   Renderer.wire=Kb.b(KB_TILDE);
   Renderer(Render);

   if(Ms.b(0))
   {
      if(Renderer.rebuildDepthNeededForDebugDrawing())Renderer.rebuildDepth();
      Physics.draw();
   }
   
   D.text(0, 0.9f, "Press LMB to draw Physics");
   D.text(0, 0.8f, "Press Tilde for Wireframe view");
}
/******************************************************************************/
