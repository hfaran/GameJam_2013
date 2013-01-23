/******************************************************************************/
#include "stdafx.h"
/******************************************************************************/
struct QuakeEffect // Camera Quake Effect
{
   Flt scale; // quake scale, default=0.15

   Vec offset() {return _offset;} // get quake offset
   Vec vel   () {return _vel   ;} // get quake velocity

   void setConst (Flt power  ); // set constant quake (0..1)
   void addLong  (Flt power=1); // add long     quake (0..1)
   void addMedium(Flt power=1); // add medium   quake (0..1)
   void addShort (Flt power=1); // add short    quake (0..1)

   void update();

   QuakeEffect();

private:
   Flt step, power[4];
   Vec from, to, _offset, _vel;
};
/******************************************************************************/
QuakeEffect::QuakeEffect()
{
   scale=0.15f;
}

void QuakeEffect::setConst (Flt power) {T.power[0]=Sat(           power);}
void QuakeEffect::addLong  (Flt power) {T.power[1]=Sat(T.power[1]+power);}
void QuakeEffect::addMedium(Flt power) {T.power[2]=Sat(T.power[2]+power);}
void QuakeEffect::addShort (Flt power) {T.power[3]=Sat(T.power[3]+power);}

void QuakeEffect::update()
{
   Flt p=scale*Min(1.0f, power[0]+power[1]+power[2]+power[3]); // sum of powers

   power[1]=Max(power[1]-Time.d()*0.25f, 0.0f); // decrease power with time, but don't allow being negative
   power[2]=Max(power[2]-Time.d()*1.00f, 0.0f); // decrease power with time, but don't allow being negative
   power[3]=Max(power[3]-Time.d()*4.00f, 0.0f); // decrease power with time, but don't allow being negative

   if(p<=EPSL) // if too small
   {
     _offset.zero();
     _vel   .zero();
   }else
   {
      Flt speed=20; // speed of calculating new position offsets
      if((step-=Time.d()*speed)<=0)
      {
         step=Frac(step);
         from=to;              // new 'from' = old 'to'
         to  =Random(Ball(1)); // new 'to  ' = random position inside Ball
      }
     _offset=p*Lerp(to, from, step); // set quake offset as linear interpolation between 'to from' scaled by 'p'
     _vel   =(speed*p)*(to-from);
   }
}
/******************************************************************************/
Camera      cam;
QuakeEffect qe;
/******************************************************************************/
void InitPre()
{
   App.name("Camera Quake Effect");
   App.flag=APP_MS_EXCLUSIVE;
   Paks.add("../data/engine.pak");
}
/******************************************************************************/
Bool Init()
{
   DefaultTextStyle.color =BLACK;
   DefaultTextStyle.shadow=0;
   cam.dist = 5;
   cam.pitch=-0.2f;
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

   // update quake effect
   if(Kb.bp(KB_SPACE))qe.addMedium(); // add medium quake camera effect when space pressed
                      qe.update   (); // update           camera effects

   // update helper camera
   cam.yaw+=Time.d();

   // set main camera
   Cam.setSpherical(cam.at, cam.yaw, cam.pitch, cam.roll, cam.dist); // set the main 'Cam' camera from helper 'cam' camera
   Cam+=qe.offset();                                                 // apply quake offset
   Cam.updateVelocities();                                           // calculate velocities
   Cam.set             ();                                           // activate

   return true;
}
/******************************************************************************/
void Draw()
{
   D.clear(WHITE);

   // draw 3x3 boxes
   SetMatrix();
   REPD(x, 3)
   REPD(z, 3)Box(0.3f, Vec(x-1, 0, z-1)).draw(BLACK);

   D.text(0, 0.9f, "Press Space for Camera Quake effect");
}
/******************************************************************************/
