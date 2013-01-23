/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
struct ChatMessage
{
   Str   text;
   Color background;
   Flt   height, y_min, y_max;

   // get / set
   void set(Str text, Int color_index) {T.text=text; background=ColorHSB(color_index*0.3f, 0.1f, 1);}

   // draw
   void draw(TextStyle &ts, Vec2 &box_top_left, Flt box_width, Flt margin);

   ChatMessage() {background.zero(); height=y_min=y_max=0;}
};
STRUCT(ChatMessages , GuiCustom)
//{
   Flt               margin;
   TextStyle         ts;
   Memc<ChatMessage> messages;

   // manage
   ChatMessages& create();

   // operate
   void New         (Str text); // add new message
   void rebuildSpace(        );

   // draw
   virtual void draw(C GuiPC &gpc); // draw object
};
/******************************************************************************/
void ChatMessage::draw(TextStyle &ts, Vec2 &box_top_left, Flt box_width, Flt margin)
{
   Rect_LU rect(box_top_left+Vec2(0, -y_min), box_width, height);
   rect.draw(background);
   rect.min.x+=margin;
   D.text(ts, rect, text, AUTO_LINE_SPACE);
}
/******************************************************************************/
ChatMessages& ChatMessages::create()
{
   super::create();

   margin=0.01f;

   ts.reset();
   ts.scale=0.05f;
   ts.align.set(1,-1);

   return T;
}
/******************************************************************************/
void ChatMessages::New(Str text)
{
   messages.New().set(text, messages.elms());

   rebuildSpace();
}
/******************************************************************************/
void ChatMessages::rebuildSpace()
{
   Flt y=0, width=0;

   Region *region=NULL;
   if(parent() && parent()->type()==GO_REGION)region=&parent()->asRegion();

   if(region)width=region->crect.w()-margin;

   FREPA(messages)
   {
      Flt h=Max(1, ts.textLines(messages[i].text, width, AUTO_LINE_SPACE))*0.05f;

      messages[i].height=h  ;
      messages[i].y_min =y  ;
      messages[i].y_max =y+h;

      y+=h;
   }
   
   if(region)region->setSize(&Vec2(0,y));
}
/******************************************************************************/
static Int FindMinY(C ChatMessage &message, C Flt &min_y) {return Compare(message.y_max, min_y);}
static Int FindMaxY(C ChatMessage &message, C Flt &max_y) {return Compare(message.y_min, max_y);}

void ChatMessages::draw(C GuiPC &gpc)
{
   if(visible() && !gpc.hidden && parent() && parent()->type()==GO_REGION)
   {
      // clip drawing rectangle
      D.clip(gpc.clip);

      // calculate offsets
      Region &region       =parent()->asRegion();
      Flt     offset       =region.slidebar[1].offset(); // get regions vertical slidebar offset
      Vec2    box_top_left =gpc.offset;                  // get top left corner in "screen space"
      Flt     box_width    =region.crect.w(),            // get visible space width
              visible_min_y=offset,                      // get visible space in "message space"
              visible_max_y=region.crect.h()+offset;     // get visible space in "message space"

      // find visible message indexes
      Int from, to;
      messages.binarySearch(visible_min_y, from, FindMinY),
      messages.binarySearch(visible_max_y, to  , FindMaxY);

      // draw them
      for(Int i=from; i<to; i++)messages[i].draw(ts, box_top_left, box_width, margin);
   }
}
/******************************************************************************/
Window       window; // gui  window
Region       region; // gui  region
Button       add   ; // add  message button
ChatMessages chat  ; // chat messages
/******************************************************************************/
void InitPre()
{
   App.name("Chatbox");
   Paks.add("../data/engine.pak");
   D.mode(800,600);
}
/******************************************************************************/
Str texts[3]=
{
   "This is a random text",
   "This is a random text but a little bit longer",
   "This is a random text but much much longer, hopefully to take much more more more more more more more more lines",
};
void AddMessage(Ptr)
{
   chat.New("New Custom Message");
   region.scrollToY(9999); // scroll to the end of the region
}
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;

   Gui   +=window.create(Rect(-0.5f , -0.4f, 0.5f ,  0.4f), "Chatbox");
   window+=region.create(Rect( 0.05f, -0.6f, 0.95f, -0.1f));
   window+=add   .create(Rect_C(window.crect.w()/2, -0.67f, 0.3f, 0.06f), "Add Message").func(AddMessage);
   region+=chat  .create();

   FREP(20)chat.New(S+i+": "+texts[Random(Elms(texts))]);

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
   D  .clear(WHITE);
   Gui.draw ();
}
/******************************************************************************/
