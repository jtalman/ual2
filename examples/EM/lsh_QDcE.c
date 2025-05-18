#include <stdio.h>

double dx=1e-6,dxp=0,dy=0,dyp=0,dct=0,de=0;

double AT=0;

typedef struct marker{
 char tag[11];
 void(*marker_track)(struct marker *);
} marker;

void marker_track (marker *mp){
 printf("%s:AT:: %+e %+e %+e %+e %+e %+e %+e\n", mp->tag,AT,dx,dxp,dy,dyp,dct,de);
}

typedef struct drift{
 char tag[11];
 double length;
 void(*drift_track)(struct drift *);
} drift;

void drift_track (drift *dp){
 double l = dp->length;
 dx = dx + l * dxp;
 dy = dy + l * dyp;

 AT = AT + l;
 printf("%s:AT:: %+e %+e %+e %+e %+e %+e %+e\n", dp->tag,AT,dx,dxp,dy,dyp,dct,de);
}

typedef struct quad{
 char tag[11];
 double length;
 double strength;
 void(*quad_track)(struct quad *);
} quad;

void quad_track (quad *qp){
 double l = qp->length;
 double q = qp->strength;
 dxp = q * dx + dxp;
 dyp = q * dy + dyp;

 AT = AT + l;
 printf("%s:AT:: %+e %+e %+e %+e %+e %+e %+e\n", qp->tag,AT,dx,dxp,dy,dyp,dct,de);
}

void main(){
/*
PTR_m_nomEQ0p0.32349_sl4.adxf
 46         <sector name="lsh_QDcE">
 47             <frame ref="Dllshsl"/>
 48             <frame ref="mNullSlice"/>
 49             <frame ref="Dllshsl"/>
 50             <frame ref="mNullSlice"/>
 51             <frame ref="Dllshsl"/>
 52             <frame ref="mNullSlice"/>
 53             <frame ref="Dllshsl"/>
 54             <frame ref="mQcend"/>
 55             <frame ref="QDcMh"/>
 56             <frame ref="mQDcM"/>
 57             <frame ref="QDcMh"/>
 58             <frame ref="DlQch"/>
 59             <frame ref="QDcE"/>
 60             <frame ref="mQDcE"/>
 61         </sector>
*/
  drift Dllshsl   = {.tag="Dllshsl   ",.length=+0.46776750000000006,                  .drift_track=&drift_track};
  quad mNullSlice = {.tag="mNullSlice",.length=+0.0000000001,.strength=+0.0000000001, .quad_track=&quad_track};
//drift Dllshsl   = {.tag="Dllshsl   ",.length=+0.46776750000000006,                  .drift_track=&drift_track};
//quad mNullSlice = {.tag="mNullSlice",.length=+0.0000000001,.strength=+0.0000000001, .quad_track=&quad_track};
//drift Dllshsl   = {.tag="Dllshsl   ",.length=+0.46776750000000006,                  .drift_track=&drift_track};
//quad mNullSlice = {.tag="mNullSlice",.length=+0.0000000001,.strength=+0.0000000001, .quad_track=&quad_track};
//drift Dllshsl   = {.tag="Dllshsl   ",.length=+0.46776750000000006,                  .drift_track=&drift_track};
  marker mQcend   = {.tag="mQcend    ",                                               .marker_track=&marker_track};
  quad QDcMh      = {.tag="QDcMh     ",.length=+0.0000000001,.strength=-0.00000045455,.quad_track=&quad_track};
  marker mQDcM    = {.tag="mQDcM     ",                                               .marker_track=&marker_track};
//quad QDcMh      = {.tag="QDcMh     ",.length=+0.0000000001,.strength=-0.00000045455,.quad_track=&quad_track};
  drift DlQch     = {.tag="DlQch     ",.length=+0.2,                                  .drift_track=&drift_track};
  quad QDcE       = {.tag="QDcE      ",.length=+0.0000000001,.strength=+0.0090900909, .quad_track=&quad_track};
  marker mQDcE    = {.tag="mQDcE     ",                                               .marker_track=&marker_track};

  printf("%s:AT:: %+e %+e %+e %+e %+e %+e %+e\n", "INJECT    ",AT,dx,dxp,dy,dyp,dct,de);

  Dllshsl.drift_track(&Dllshsl);
  mNullSlice.quad_track(&mNullSlice);
  Dllshsl.drift_track(&Dllshsl);
  mNullSlice.quad_track(&mNullSlice);
  Dllshsl.drift_track(&Dllshsl);
  mNullSlice.quad_track(&mNullSlice);
  Dllshsl.drift_track(&Dllshsl);
  mQcend.marker_track(&mQcend);
  QDcMh.quad_track(&QDcMh);
  mQcend.marker_track(&mQcend);
  QDcMh.quad_track(&QDcMh);
  DlQch.drift_track(&DlQch);
  QDcE.quad_track(&QDcE);
  mQDcE.marker_track(&mQDcE);
}
