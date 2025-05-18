#include <fstream.h>
#include <stdlib.h>
#include <math.h>

#include "NumLib/NumLib.hh"
#include "Constant/Constant.hh"

int sign(double a)
{
  return (a<0. ? -1 : 1);
}

int min(int a, int b)
{
  return (a<b ? a : b);
}

int max(int a, int b)
{
  return (a>b ? a : b);
}

//---------------------------------------------------

void ludcmp(double** a, int n, int* indx, double* d)
{
  int imax, k, i, j;
  double big, dum, sum, temp;
  double* vv;
 
  vv = new double[n+1];
  if(!vv) 
  { 
    cerr << "ludcmp: allocation failure in vector()\n";
    exit(1);
  }
  *d = 1.0;
  for(i=1; i <= n ; i++)
  {
     big = 0.0;
     for(j=1; j <= n; j++)
       if ((temp = fabs(a[i][j])) > big) big = temp;
     if (big == 0.0) cerr << "ludcmp : Singular matrix in routine \n";
     vv[i] = 1./big;
  }
  for (j = 1; j <= n; j++)
  {
     for (i=1; i < j; i++)
     {
         sum = a[i][j];
         for (k=1; k < i ; k++) sum -= a[i][k]*a[k][j];
         a[i][j] = sum;
     }
     big = 0.0;
     for(i=j; i <= n; i++)
     {
         sum = a[i][j];
         for (k=1; k < j; k++)
             sum -= a[i][k]*a[k][j];
         a[i][j] = sum;
         if ((dum = vv[i]*fabs(sum)) >= big)
         {
             big = dum;
             imax = i;
         }
     }
     if ( j != imax)
     {
         for (k=1; k <= n; k++)
         {
            dum = a[imax][k];
            a[imax][k] = a[j][k];
            a[j][k] = dum;
         }
         *d = -(*d);
         vv[imax] = vv[j];
     }
   indx[j] = imax;
   if (a[j][j] == 0.0) a[j][j] = TINY;
   if (j != n) 
   {
      dum =1.0/(a[j][j]);
      for ( i=j+1; i <= n; i++) a[i][j] *= dum;
   }
 }
 delete [] vv;
}

void lubksb(double** a, int n, int* indx, double* b)
{
   int ii = 0;
   int ip, j, i;
   double sum;

   for(i=1; i <= n; i++)
   {
      ip = indx[i];
      sum = b[ip];
      b[ip] = b[i];
      if(ii)
          for(j=ii; j <= i-1; j++) sum -= a[i][j]*b[j];
      else if (sum) ii = i;
      b[i] = sum;
    }
    for (i=n; i >=1; i--)
    {
       sum = b[i];
       for (j=i+1; j <= n; j++) sum -= a[i][j]*b[j];
       b[i] = sum/a[i][i];
    }
}

void invMatrix(double**a, int n)
{
   double** y;
   double* col;
   double  d;
   int* indx;
   int i, j;

   indx = new int[n+1];   
   col  = new double[n+1]; 
   y = new double*[n+1];
   for(i=1; i <= n; i++)
           y[i] = new double[n+1];
   if(!y[n]) 
   {
     cerr << "invMatrix: allocation failure \n";
     exit(1);
   } 
   ludcmp(a, n, indx, &d);
   for(i = 1; i <= n; i++) d *= a[i][i];

   if( fabs(d) < TINY) 
   {
      cerr << "Error : invMatrix ";
      cerr << "fabs(Determinant) = " << fabs(d) << " < " << TINY << " \n";
      exit(1);
   }

   for(j=1; j <= n; j++)
   {
      for(i=1; i <= n; i++) col[i]= 0.0;
      col[j] = 1.0;
      lubksb(a, n, indx, col);
      for(i=1; i <= n; i++) y[i][j]=col[i];
   }
   for(i=1; i <= n; i++)
     for(j=1; j <= n; j++)
          a[i][j]=y[i][j];

   for(i=1; i <= n; i++)
            delete [] y[i];
   delete [] y;
   delete [] col;
   delete [] indx;
   return;
}

  
//---------------------------------------------------------

void solveLinearEquations(double**a, double* b, int n)
{
 
   double  d;
   int* indx;

   indx = new int[n+1];   
   if(!indx) 
   {
     cerr << "solveLinearEquations: allocation failure \n";
     exit(1);
   } 

   ludcmp(a, n, indx, &d);
   lubksb(a, n, indx, b);

   delete [] indx;
   return;
}


    
