/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
struct MenuNew // new-game menu
{
   Window window;
   Text   text;
   Button close;
   
   static void Close(MenuNew &menu_new) {menu_new.window.fadeOut();}

   void create()
   {
      Gui   +=window.create(Rect_C(0, 0, 0.9f, 0.5f)).barVisible(false).hide();
      window+=text  .create(Vec2  (window.crect.w()/2, -0.15f), "New Game");
      window+=close .create(Rect_C(window.crect.w()/2, -window.crect.h()/2-0.1f, 0.4f, 0.08f), "Close").func(Close, T);
   }
}MN;
/******************************************************************************/
struct MenuLoad // load-game menu
{
   Window window;
   Text   text;
   Button close;
   
   static void Close(MenuLoad &menu_load) {menu_load.window.fadeOut();}

   void create()
   {
      Gui   +=window.create(Rect_C(0, 0, 0.9f, 0.5f)).barVisible(false).hide();
      window+=text  .create(Vec2  (window.crect.w()/2, -0.15f), "Load Game");
      window+=close .create(Rect_C(window.crect.w()/2, -window.crect.h()/2-0.1f, 0.4f, 0.08f), "Close").func(Close, T);
   }
}ML;
/******************************************************************************/
struct MenuOptions // options menu
{
   Window window;
   Text   text;
   Button close;
   
   static void Close(MenuOptions &menu_options) {menu_options.window.fadeOut();}

   void create()
   {
      Gui   +=window.create(Rect_C(0, 0, 0.9f, 0.5f)).barVisible(false).hide();
      window+=text  .create(Vec2  (window.crect.w()/2, -0.15f),"Options");
      window+=close .create(Rect_C(window.crect.w()/2, -window.crect.h()/2-0.1f, 0.4f, 0.08f), "Close").func(Close, T);
   }
}MO;
/******************************************************************************/
struct MenuMain // main menu
{
   Window window;
   Button b_new, b_load, b_options, b_exit;

   static void New    (Ptr) {MN.window.fadeIn();}
   static void Load   (Ptr) {ML.window.fadeIn();}
   static void Options(Ptr) {MO.window.fadeIn();}
   static void Exit   (Ptr) {StateExit.set   ();}

   void create()
   {
      Gui   +=window   .create(Rect_C(0, -0.1f, 0.6f, 0.6f)).barVisible(false);
      window+=b_new    .create(Rect_C(window.crect.w()/2, -0.15f, 0.4f, 0.08f), "New"    ).func(New    );
      window+=b_load   .create(Rect_C(window.crect.w()/2, -0.25f, 0.4f, 0.08f), "Load"   ).func(Load   );
      window+=b_options.create(Rect_C(window.crect.w()/2, -0.35f, 0.4f, 0.08f), "Options").func(Options);
      window+=b_exit   .create(Rect_C(window.crect.w()/2, -0.45f, 0.4f, 0.08f), "Exit"   ).func(Exit   );
      window.flag&=~WIN_MOVABLE; // disable moving of this window
      window.level(-1)         ; // set lower level so this window will be always below the other created in this tutorial
   }
}MM;
/******************************************************************************/
void InitPre()
{
   App.name("Game Menu");
   App.flag=APP_FULL_TOGGLE;
   Paks.add("../data/engine.pak");
   D.mode(800,600);
}
/******************************************************************************/
Bool Init()
{
   // in games disable highlighting elements with keyboard focus
   Gui.kb_lit.zero();

   // change default gui text colors
   Gui.ts_text.color=WHITE;
   Gui.ts_text.shadow=255;

   // change Default Gui Window Style
   Gui.style_window.image_back  ="../data/Gui/Style/0/back.gfx";
   Gui.style_window.image_border="../data/Gui/Style/0/border.gfx";
   Gui.style_window.border_color=WHITE;
   Gui.style_window.back_color  =BLUE;
   Gui.style_window.border_width=0.03f;

   MN.create();
   ML.create();
   MO.create();
   MM.create();
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
   Gui.update();
   return true;
}
/******************************************************************************/
void Draw()
{
   D  .clear();
   Gui.draw ();
}
/******************************************************************************/
