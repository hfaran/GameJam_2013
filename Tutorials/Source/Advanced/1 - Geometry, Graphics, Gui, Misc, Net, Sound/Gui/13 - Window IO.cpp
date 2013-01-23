/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
WindowIO wio_mesh; // window handling input/ouput
Mesh     mesh    ; // mesh which will be used for saving,loadng and drawing
/******************************************************************************/
void InitPre()
{
   App.name("Window IO");
   App.flag=APP_FULL_TOGGLE;
   DataPath("../data");
   Paks.add("engine.pak");
   D.mode(800,600);
}
/******************************************************************************/
void Load(Str name, Ptr user) // this function will be called when WindowIO will load a file
{
   mesh.load(name);
}
void Save(Str name, Ptr user) // this function will be called when WindowIO will save a file
{
   // here you can save your file, however in this tutorial we're not going to do this
   // mesh.save(name);
}
/******************************************************************************/
Bool Init()
{
   wio_mesh.create("mesh", S, DataPath(), Load, Save); // create a WindowIO, giving accepted file extension, paths, and IO functions
   wio_mesh.path_mode=WIN_IO_PM_PART; // set displaying path mode on the window title bar

   return true;
}
/******************************************************************************/
void Shut()
{
}
/******************************************************************************/
Bool Update()
{
   if(Kb.bp(KB_ESC) && !Gui.window())return false;
   Gui.update();

   // activate saving/loading on keypress
   if(Kb.bp(KB_F2))wio_mesh.save();
   if(Kb.bp(KB_F3))wio_mesh.load();

   return true;
}
/******************************************************************************/
void Render()
{
   switch(Renderer())
   {
      case RM_PREPARE:
      {
         LightDir(Cam.matrix.z, 1-D.ambientColor()).add();

         mesh.draw(MatrixIdentity);
      }break;
   }
}
void Draw()
{
   Cam.setSpherical(mesh.box.center(), Time.time(), 0, 0, mesh.box.size().length()*1.2f).updateVelocities().set();
   Renderer(Render);
   D  .text(0, 0.9f, "Press F2/F3 to save/load");
   Gui.draw();
}
/******************************************************************************/
