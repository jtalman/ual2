// File               : LEB.hh
// Description        : This file contains the structure of LEB  
// Created            : May 15, 1994
// Authors            : Nikolay Malitsky (malitsky@ivory.ssc.gov)
//                      Alexander Reshetov (reshetov@vernon.ssc.gov)
//
//
// (C) Copyright
// SSC Laboratory
// 2550 Beckleymeade Ave.
// Dallas, TX, 75237

// Global Parameter

   ENERGY = 1.538*GEV;

   Error glRmsShift = 0.0004*(DX + DY);

// Main Dipole Magnet

   Element hb("Main Dipole");

   int    hbNumber = 96;
   double hbAngle  = 2*PI/hbNumber;

   Element hbSysMlt  = -4.41e-04*K[2] +
                        2.32e+00*K[4] -
                        4.09e+03*K[6] +
                        5.90e+06*K[8];

   Error   hbRmsMlt  =  5.82e-05*K[1] + 2.56e-05*KT[1] +
                        5.70e-04*K[2] + 2.69e-04*KT[2] +
                        8.96e-03*K[3] + 5.06e-03*KT[3] +
                        7.04e-02*K[4] + 5.63e-02*KT[4] +
                        3.78e+00*K[5] + 1.34e+00*KT[5] +
                        3.58e+01*K[6] + 2.05e+01*KT[6];

   hb  = 1.940*M + hbAngle*(RAD + E1/2. + E2/2.);        
   hb += hbSysMlt + hbRmsMlt + glRmsShift;

   Element db1("DB1"); db1 = 0.1115*M;
   Element db2("DB2"); db2 = 0.1485*M;

   Line hbL = db2*hb*db1*db1*hb*db2;

// Main Quadrupoles

   double qK =  0.3722;

   Error   qRmsMlt  =   7.28e-05*K[1] + 1.00e-18*KT[1] +
                        3.36e-04*K[2] + 1.82e-04*KT[2] +
                        1.09e-02*K[3] + 7.28e-03*KT[3] +
                        2.80e-02*K[4] + 3.64e-02*KT[4] +
                        7.28e-01*K[5] + 1.09e+00*KT[5]; 

   Element qtmp  =  qRmsMlt + glRmsShift + 3*IR;
   Element qftmp =  qK*K[1] + qtmp;
   Element qdtmp = -qK*K[1] + qtmp;

   Element qf1h("QF1H");   qf1h  = 0.7564/2*M + qftmp;
   Element qf2h("QF2H");   qf2h  = 0.5919/2*M + qftmp;
   Element qd1h("QD1H");   qd1h  = 0.5983/2*M + qdtmp;
   Element qd2h("QD2H");   qd2h  = 0.6127/2*M + qdtmp;

   Element qfs1h("QFS1H"); qfs1h = 0.6568/2*M + qftmp;
   Element qfs2h("QFS2H"); qfs2h = 0.5552/2*M + qftmp;
   Element qds1h("QDS1H"); qds1h = 0.6980/2*M + qdtmp;
   Element qds2h("QDS2H"); qds2h = 0.6858/2*M + qdtmp;

      
// Norm Trim Quads

   Element qt[7] = {"QTF", "QTD1", "QTD2", "QTFS1", "QTFS2", "QTDS1", "QTDS2"};
   Line    qtL[7]; 

   static double qtLength = 0.294;

   qt[0] =  0.589e-02*K[1];
   qt[1] = -0.116e-01*K[1];
   qt[2] = -0.157e-01*K[1];
   qt[3] =  0.425e-01*K[1];
   qt[4] =  0.573e-01*K[1];
   qt[5] = -0.663e-02*K[1];
   qt[6] = -0.253e-01*K[1];

   Element dqt("DQT"); dqt = 0.048*M;

   for(int iqt=0; iqt < 7; iqt++)
   {
      qt[iqt]  += qtLength*M;
      qtL[iqt]  = dqt*qt[iqt]*dqt;
   }

// Norm Sextupoles

   Element sext[3] = {"SEXTF", "SEXTD1", "SEXTD2"};
   Line    sextL[3];

   static double sextLength = 0.3;

   sext[0] =  0.535*K[2];
   sext[1] = -0.970*K[2];
   sext[2] = -1.040*K[2];

   Element dsext("DSEXT"); dsext = 0.05*M;

   for(int isext=0; isext < 3; isext++)
   {
     sext[isext]  += sextLength*M;
     sextL[isext]  = dsext*sext[isext]*dsext;
   }

// Beam Position Monitors

   Element bpm("BPM");   bpm   = 0.15*M;
   Element dbpm("DBPM"); dbpm  = 0.02*M;
   Line    bpmL;         bpmL  = dbpm*bpm*dbpm;   

// Correctors

   Element ch("CorH");  ch  = 0.15*M;
   Element cv("CorV");  cv  = 0.15*M;
   Element dc("DC");    dc  = 0.06*M;

   Line    chL = dc*ch*dc;               
   Line    cvL = dc*cv*dc; 

// Drifts

   Element dq1("DQ1"); dq1 = 0.1*M;

   Element darc[11] = { 0.4915*M, 0.4974*M, 0.2091*M, 0.3591*M, 0.2134*M,
                        2.6814*M, 0.1900*M, 2.9514*M, 0.2274*M, 0.1900*M,
                        0.4915*M};

   Element dstr[5]  = { 6.4231*M, 2.7976*M, 7.4234*M, 0.0900*M, 7.1534*M};

// Lines

// Arc

   Line fstDBFBD = qds2h*(bpmL*qtL[6]*darc[0])*hbL*(darc[2]*qtL[0]*bpmL)*qf2h*
                   qf2h *(dq1*chL*darc[3])    *hbL*(darc[4]*qtL[1]*bpmL)*qd1h; 
  
   Line DBFBD1   =  qd2h*(bpmL*qtL[2]*darc[1])*hbL*(darc[2]*qtL[0]*bpmL)*qf2h*
                    qf2h*(dq1*chL*darc[3])    *hbL*(darc[4]*qtL[1]*bpmL)*qd1h; 

   Line DOFOD    =  qd1h*(dq1*sextL[1]*cvL*darc[5]*chL*sextL[0]*bpmL)*qf1h*
                    qf1h*(darc[6]*sextL[0]*darc[7]*cvL*sextL[2]*dq1) *qd1h; 

   Line DBFBD2   =  qd1h*(bpmL*qtL[1]*darc[4])*hbL*(darc[3]*chL*dq1)           *qf2h*
                    qf2h*(bpmL*qtL[0]*darc[2])*hbL*(darc[8]*cvL*qtL[2]*darc[9])*qd2h; 

   Line lstDBFBD =  qd1h*(bpmL*qtL[1]*darc[4])*hbL*(darc[3]*chL*dq1)     *qf2h*
                    qf2h*(bpmL*qtL[0]*darc[2])*hbL*(darc[10]*qtL[6]*bpmL)*qds2h;

   Line arcf     = fstDBFBD*DOFOD*DBFBD2;
   Line arcs     = DBFBD1*DOFOD*DBFBD2;
   Line arcl     = DBFBD1*DOFOD*lstDBFBD;

   Line arc      = arcf*arcs*arcs*arcl;

// Straigth Section

   Line DOFOD1      =  qds2h*(dq1*cvL*dstr[0]*qtL[3]*bpmL)*qfs1h*
                       qfs1h*(dq1*chL*dstr[1]*qtL[5]*bpmL)*qds1h; 

   Line DOFOD2      =  qds1h*(dq1*cvL*dstr[2]*qtL[4]*bpmL)           *qfs2h*
                       qfs2h*(dq1*dstr[3]*qtL[4]*chL*dstr[4]*cvL*dq1)*qds1h; 

   Line strSection  = DOFOD1*DOFOD2*reflect(DOFOD1);

// SuperPeriod

   Line superPeriod = arc*strSection;

// LEB

   Line leb("C++ version"); 
   leb = power(superPeriod, 3);

