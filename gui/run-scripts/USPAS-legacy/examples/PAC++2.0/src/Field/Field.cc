#include "Field/Field.hh"

void driftField(double x, double* y, double* bF, double* eF, double* param)
{
  for(int i=0; i <=3; i++) {
    eF[i] = 0.;
    bF[i] = 0.;
  }  
}

void driftField(double x, Series* y, Series* bF, Series* eF, double* param)
{
  for(int i=0; i <= 3; i++) {
    eF[i] = 0.;
    bF[i] = 0.;
  }  
}

void mltField(double x, double* y, double* bF, double* eF, double* param)
{
  int i, order;
  order = (int) ((param[0]-1)/2);
  static double tmp[5];
  for(i=0; i <= 4; i++) tmp[i] = 0;
  tmp[1] = y[1];
  if(BEAM_DIM > 2) tmp[3] = y[3];

  for(i=order; i >= 0 ; i--)
  {
    tmp[0]  = tmp[1]*tmp[2];
    tmp[0] -= tmp[3]*tmp[4];
    tmp[4]  = tmp[1]*tmp[4];
    tmp[4] += tmp[3]*tmp[2];
    if(i){
      tmp[0] += param[i+1];
      tmp[4] += param[i+order+1];
    }
    tmp[2]  = tmp[0];
  }

  for(i=0; i <=3; i++) {
    eF[i] = 0.;
    bF[i] = 0.;
  }

  bF[1] = tmp[4];
  bF[2] = tmp[2];
}

void mltField(double x, Series* y, Series* bF, Series* eF, double* param)
{
  int i, order;
  order = (int) ((param[0]-1)/2);
  static Series tmp[5];
  for(i=0; i <= 4; i++) tmp[i] = 0;
  tmp[1] = y[1];
  if(BEAM_DIM > 2) tmp[3] = y[3];

  for(i=order; i >= 0 ; i--)
  {
    tmp[0]  = tmp[1]*tmp[2];
    tmp[0] -= tmp[3]*tmp[4];
    tmp[4]  = tmp[1]*tmp[4];
    tmp[4] += tmp[3]*tmp[2];
    if(i){
      tmp[0] += param[i+1];
      tmp[4] += param[i+order+1];
    }
    tmp[2]  = tmp[0];
  }

  for(i=0; i <=3; i++) {
    eF[i] = 0.;
    bF[i] = 0.;
  }

  bF[1] = tmp[4];
  bF[2] = tmp[2];
}

void simpleHelixField(double s, double* y, double* bF, double* eF, double* param)
{
  // param[1] - L
  // param[2] - k
  // param[3] - phi
  // param[5] - -2*B0/BR/k

  for(int i=0; i <=3; i++){
    eF[i] = 0.;
    bF[i] = 0.;
  }

  double cs, ss, k2;
  double x2, y2, xy; 

  cs = cos(param[2]*s+param[3]);
  ss = sin(param[2]*s+param[3]);
  k2 = param[2]*param[2];

  x2 = y[1]*y[1];
  xy = y[1]*y[3];
  y2 = y[3]*y[3];
  
  bF[1]  = 3*x2 + y2;
  bF[1] *= k2/8.; 
  bF[1] += 1.;
  bF[1] *= ss;
  bF[1] -= xy*(k2*cs/4.);
  bF[1] *= param[5]/2.*param[2];

  bF[2]  = x2 + 3*y2;
  bF[2] *= k2/8.; 
  bF[2] += 1.;
  bF[2] *= cs;
  bF[2] -= xy*(k2*ss/4.);
  bF[2] *= -param[5]/2.*param[2];

  bF[3]  = x2 + y2;
  bF[3] *= k2/8.;
  bF[3] += 1.;
  bF[3] *= y[1]*cs + y[3]*ss;
  bF[3] *= k2*param[5]/2.;

}

void simpleHelixField(double s, Series* y, Series* bF, Series* eF, double* param)
{
  // param[1] - L
  // param[2] - k
  // param[3] - phi
  // param[5] - -2*B0/BR/k

  for(int i=0; i <=3; i++){
    eF[i] = 0.;
    bF[i] = 0.;
  }

  double cs, ss, k2;
  Series x2, y2, xy; 

  cs = cos(param[2]*s+param[3]);
  ss = sin(param[2]*s+param[3]);
  k2 = param[2]*param[2];

  x2 = y[1]*y[1];
  xy = y[1]*y[3];
  y2 = y[3]*y[3];
  
  bF[1]  = 3*x2 + y2;
  bF[1] *= k2/8.; 
  bF[1] += 1.;
  bF[1] *= ss;
  bF[1] -= xy*(k2*cs/4.);
  bF[1] *= param[5]/2.*param[2];

  bF[2]  = x2 + 3*y2;
  bF[2] *= k2/8.; 
  bF[2] += 1.;
  bF[2] *= cs;
  bF[2] -= xy*(k2*ss/4.);
  bF[2] *= -param[5]/2.*param[2];

  bF[3]  = x2 + y2;
  bF[3] *= k2/8.;
  bF[3] += 1.;
  bF[3] *= y[1]*cs + y[3]*ss;
  bF[3] *= k2*param[5]/2.;

}

int BESSI_PSEUDO_NUMBER = 10;

double bessiPseudo(double r, int n)
{
   int i, fac, now;
   double sum, term, r2;

   fac = 1;
   for(i=2; i <= n+1; i++) fac *= i;

   term  = 1./fac/4.;
   sum   = 1./fac/4.;
   r2    = r/4.; 

//   now = (ZLIB_ORDER-n)/2;
//   if(r2 > ZLIB_TINY) 
   now = BESSI_PSEUDO_NUMBER;
   for(i=2; i <= now; i++)
   {
     term *= r2;
     term /= i*(i+n);
     sum  += term;
   }

   return(sum*1.);
}

void helixField(double s, double* y, double* bF, double* eF, double* param)
{
   int i, m, order;
   double k, coeff;
   double cosTmp, sinTmp, cTmp, fac;

   double rk2, bip, bipD, rcos[2], rsin[2];
   double bFm[4];
   
   for(i=0; i <= 3; i++){
     eF[i] = 0.;
     bF[i] = 0.;
   }
   order = (int) param[0];
   order = (order-4)/2;
   k     =  param[2];
 
   rk2    = y[1]*y[1];
   rk2   += y[3]*y[3];
   rk2   *= k*k;

   rcos[0] = 1.;
   rsin[0] = 0.;

   for(m=1; m <= order; m++){

     bip   = rk2*m*m;
     bipD  = bip;
     bip   = bessiPseudo(bip,  m);
     bipD  = bessiPseudo(bipD, m-1);
     bipD -= bip*(m/2.);

     rcos[1]  =  rcos[0]*y[1];
     rcos[1] -=  rsin[0]*y[3];

     rsin[1]  =  rsin[0]*y[1];
     rsin[1] +=  rcos[0]*y[3];


     coeff =  1;
     fac   =  1;
     cTmp  =  m*k/2.;
     for(i=1; i < m; i++){
        coeff *= cTmp;
        fac   *= i;
     }

     cosTmp  = cos(m*(k*s+param[3]));
     sinTmp  = sin(m*(k*s+param[3]));

     bFm[3]  = param[4+m]      *(rcos[1]*cosTmp + rsin[1]*sinTmp);
     bFm[3] -= param[4+order+m]*(rsin[1]*cosTmp - rcos[1]*sinTmp);

     bFm[0]  = param[4+m]      *(rsin[1]*cosTmp - rcos[1]*sinTmp);
     bFm[0] += param[4+order+m]*(rcos[1]*cosTmp + rsin[1]*sinTmp);
     bFm[0] *= -coeff*(m*m*m*k*k*k);
     bFm[0] *= bipD;

     bFm[3] *= -coeff*cTmp*(m*m*m*k*k);
     bFm[3] *= bip;

     bFm[1]  =  bFm[0]*y[1];
     bFm[1] -=  bFm[3]*y[3]; 

     bFm[2]  =  bFm[0]*y[3];
     bFm[2] +=  bFm[3]*y[1];

     bF[1]  +=  bFm[1];
     bF[2]  +=  bFm[2]; 

     bFm[3] *=  -rk2/k/k;
     bF[3]  +=  bFm[3];

// 

     bFm[3]  = param[4+m]      *(rcos[1]*cosTmp + rsin[1]*sinTmp);
     bFm[3] -= param[4+order+m]*(rsin[1]*cosTmp - rcos[1]*sinTmp);
     bFm[3] *= coeff*cTmp;

     bFm[1]   = param[4+m]      *(-rsin[0]*cosTmp + rcos[0]*sinTmp);
     bFm[1]  -= param[4+order+m]*( rcos[0]*cosTmp + rsin[0]*sinTmp);
     bFm[1]  *= coeff*cTmp/fac;

     bFm[2]   = param[4+m]      *(-rcos[0]*cosTmp - rsin[0]*sinTmp);
     bFm[2]  += param[4+order+m]*( rsin[0]*cosTmp - rcos[0]*sinTmp);
     bFm[2]  *= coeff*cTmp/fac;

     bF[1]  += bFm[1];
     bF[2]  += bFm[2];
     bF[3]  += bFm[3]/fac;

     rcos[0]  =  rcos[1];
     rsin[0]  =  rsin[1];

  }

  bF[3] *= k;
    
}


Series& bessiPseudo(Series& r, int n)
{
   int i, fac, now;
   Series sum, term, r2;

   fac = 1;
   for(i=2; i <= n+1; i++) fac *= i;

   term  = 1./fac/4.;
   sum   = 1./fac/4.;
   r2    = r/4.; 

//   now = (ZLIB_ORDER-n)/2;
//   if(r2[1] > ZLIB_TINY) 
   now = BESSI_PSEUDO_NUMBER;
   for(i=2; i <= now; i++)
   {
     term *= r2;
     term /= i*(i+n);
     sum  += term;
   }

   return(sum*1.);
}

void helixField(double s, Series* y, Series* bF, Series* eF, double* param)
{
   int i, m, order;
   double k, coeff;
   double cosTmp, sinTmp, cTmp, fac;

   Series rk2, bip, bipD, rcos[2], rsin[2];
   Series bFm[4];
   
   for(i=0; i <= 3; i++){
     eF[i] = 0.;
     bF[i] = 0.;
   }
   order = (int) param[0];
   order = (order-4)/2;
   k     =  param[2];
 
   rk2    = y[1]*y[1];
   rk2   += y[3]*y[3];
   rk2   *= k*k;

   rcos[0] = 1.;
   rsin[0] = 0.;

   for(m=1; m <= order; m++){

     bip   = rk2*m*m;
     bipD  = bip;
     bip   = bessiPseudo(bip,  m);
     bipD  = bessiPseudo(bipD, m-1);
     bipD -= bip*(m/2.);

     rcos[1]  =  rcos[0]*y[1];
     rcos[1] -=  rsin[0]*y[3];

     rsin[1]  =  rsin[0]*y[1];
     rsin[1] +=  rcos[0]*y[3];


     coeff =  1;
     fac   =  1;
     cTmp  =  m*k/2.;
     for(i=1; i < m; i++){
        coeff *= cTmp;
        fac   *= i;
     }

     cosTmp  = cos(m*(k*s+param[3]));
     sinTmp  = sin(m*(k*s+param[3]));

     bFm[3]  = param[4+m]      *(rcos[1]*cosTmp + rsin[1]*sinTmp);
     bFm[3] -= param[4+order+m]*(rsin[1]*cosTmp - rcos[1]*sinTmp);

     bFm[0]  = param[4+m]      *(rsin[1]*cosTmp - rcos[1]*sinTmp);
     bFm[0] += param[4+order+m]*(rcos[1]*cosTmp + rsin[1]*sinTmp);
     bFm[0] *= -coeff*(m*m*m*k*k*k);
     bFm[0] *= bipD;

     bFm[3] *= -coeff*cTmp*(m*m*m*k*k);
     bFm[3] *= bip;

     bFm[1]  =  bFm[0]*y[1];
     bFm[1] -=  bFm[3]*y[3]; 

     bFm[2]  =  bFm[0]*y[3];
     bFm[2] +=  bFm[3]*y[1];

     bF[1]  +=  bFm[1];
     bF[2]  +=  bFm[2]; 

     bFm[3] *=  -rk2/k/k;
     bF[3]  +=  bFm[3];

// 

     bFm[3]  = param[4+m]      *(rcos[1]*cosTmp + rsin[1]*sinTmp);
     bFm[3] -= param[4+order+m]*(rsin[1]*cosTmp - rcos[1]*sinTmp);
     bFm[3] *= coeff*cTmp;

     bFm[1]   = param[4+m]      *(-rsin[0]*cosTmp + rcos[0]*sinTmp);
     bFm[1]  -= param[4+order+m]*( rcos[0]*cosTmp + rsin[0]*sinTmp);
     bFm[1]  *= coeff*cTmp/fac;

     bFm[2]   = param[4+m]      *(-rcos[0]*cosTmp - rsin[0]*sinTmp);
     bFm[2]  += param[4+order+m]*( rsin[0]*cosTmp - rcos[0]*sinTmp);
     bFm[2]  *= coeff*cTmp/fac;

     bF[1]  += bFm[1];
     bF[2]  += bFm[2];
     bF[3]  += bFm[3]/fac;

     rcos[0]  =  rcos[1];
     rsin[0]  =  rsin[1];

  }

  bF[3] *= k;
    
}

// Wiggler


int SH_NUMBER = 5;

Series& sh(Series& x)
{
   Series sum, term;

   term  = 1.;
   sum   = 0.;
 
   int now = SH_NUMBER;
   for(int i=1; i <= now; i++)
   {
     term *= x;
     term /= i;
     if((i/2)*2 != i) sum  += term;
   }

   return(sum*1.);
}

Series& ch(Series& x)
{
   Series sum, term;

   term  = 1.;
   sum   = 1.;
 
   int now = SH_NUMBER;
   for(int i=1; i <= now; i++)
   {
     term *= x;
     term /= i;
     if((i/2)*2 == i) sum  += term;
   }

   return(sum*1.);
}

int SN_NUMBER = 5;

Series& sn(Series& x)
{
   Series sum, term;

   term  = 1.;
   sum   = 0.;
 
   int now = SN_NUMBER;
   for(int i=1; i <= now; i++)
   {
     term *=  x;
     term /= -i;
     if((i/2)*2 != i){
       term *= -1;
       sum  += term;
     }
   }

   return(sum*1.);
}

Series& cn(Series& x)
{
   Series sum, term;

   term  = 1.;
   sum   = 1.;
 
   int now = SN_NUMBER;
   for(int i=1; i <= now; i++)
   {
     term *=  x;
     term /= -i;
     if((i/2)*2 == i) {
       term *= -1;
       sum  += term;
     }
   }

   return(sum*1.);
}

void wigglerField(double s, Series* y, Series* bF, Series* eF, double* param)
{

   // param[1] - length
   // param[2] - k_x
   // param[3] - k_y
   // param[4] - k_z
   // param[5] - phase 
   // param[6] - B0/BR


   Series xk, yk;

   if(!param[3]){
     cerr << "wigglerField: param[3]/ k_y / == 0. \n";
     exit(1);
   }
   
   for(int i=0; i <= 3; i++){
     eF[i] = 0.;
     bF[i] = 0.;
   }

   xk = y[1]*param[2];
   yk = y[3]*param[3];

   bF[1]  = sh(yk);
   bF[2]  = cn(xk);
   bF[3]  = bF[1]*bF[2];

   bF[1] *=  sn(xk);
   bF[1] *= -cos(param[4]*s + param[5])*param[6]*param[2]/param[3];

   bF[2] *=  ch(yk);
   bF[2] *=  cos(param[4]*s + param[5])*param[6];

   bF[3] *= -sin(param[4]*s + param[5])*param[6]*param[4]/param[3];

}

void wigglerFieldOld(double s, Series* y, Series* bF, Series* eF, double* param)
{

   // param[1] - length
   // param[2] - k_x
   // param[3] - k_y
   // param[4] - k_z
   // param[5] - phase 
   // param[6] - B0/BR


   Series xk, yk;

   if(!param[3]){
     cerr << "wigglerField: param[3]/ k_y / == 0. \n";
     exit(1);
   }
   
   for(int i=0; i <= 3; i++){
     eF[i] = 0.;
     bF[i] = 0.;
   }

   xk = y[1]*param[2];
   yk = y[3]*param[3];

   bF[1]  = sh(yk);
   bF[2]  = ch(xk);
   bF[3]  = bF[1]*bF[2];

   bF[1] *=  sh(xk);
   bF[1] *=  cos(param[4]*s + param[5])*param[6]*param[2]/param[3];

   bF[2] *=  ch(yk);
   bF[2] *=  cos(param[4]*s + param[5])*param[6];

   bF[3] *= -sin(param[4]*s + param[5])*param[6]*param[4]/param[3];

}
