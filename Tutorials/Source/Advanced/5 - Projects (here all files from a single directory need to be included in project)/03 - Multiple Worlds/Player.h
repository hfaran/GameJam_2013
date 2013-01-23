/******************************************************************************/
STRUCT(Player , Game::Chr)
//{
   Vec ctrl_pos;

   // get / set
   virtual Vec    pos   (                ) {return super::pos   (      );}
   virtual Matrix matrix(                ) {return super::matrix(      );}
   virtual void   pos   (C Vec    &pos   ) {       super::pos   (pos   ); ctrl_pos=ctrl.actor.pos();}
   virtual void   matrix(C Matrix &matrix) {       super::matrix(matrix); ctrl_pos=ctrl.actor.pos();}

   virtual Bool update();
   
   virtual UInt drawPrepare(); // extend drawing to disable head rendering in FPP mode

   virtual void save(File &f);
   virtual Bool load(File &f);
};
/******************************************************************************/
