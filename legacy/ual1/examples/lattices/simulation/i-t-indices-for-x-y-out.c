#include<stdio.h>
#include<stdlib.h>
#include<math.h>

const double pi = 3.1415926536;
const double clight = 2.99792458e+8;

double b,v;
int N;
double dN;
double R,C;
double A;
double w,f,w_0;
double Q;
int i;                                          // index  - integer:         N bounded
double t;                                       // index  - double - "time": N bounded
double a;                                       // intmdt - double - angle:  2*M_PI bounded
double r;                                       // result - double - radius:

int main(int argc,char*argv[]){
// printf("\nargc %d\n\n",argc);
 for(i=0;i<argc;i++){
//  printf("%d %s\n",i,argv[i]);
 }
// printf("\n");

 if(argc!=7){
  printf("usage: ./a.out beta Num-Points Radius Amplitude omega_0 omega-factor\n");
  printf("dsign:                                                                             \n");
  printf("       ./a.out 0.75   100        2       0.0      5         1.0 >&! design         \n");
  printf("simul:                                                                             \n");
  printf("       ./a.out 0.75   100        2       0.5      5         1.0 >&! simulation     \n");
  printf("       ./a.out 0.75   100        2       0.5      5         1.0 >&! simulation2    \n");
  printf("                                                                                   \n");
  printf("all:                                                                               \n");
  printf("       ./a.out 0.75   100       100      0.0      5         1.0 >&! design         \n");
  printf("       ./a.out 0.75   100       100      10       5         1.0 >&! simulation     \n");
  printf("       ./a.out 0.75   100       100      10       5         1.0 >&! simulation2    \n");
  exit(1);
 }
//               ./a.out 0.75   100        2       0.5      5         1.1 >&! simulation
//               ./a.out 0.75   100        2       0.5      5         0.9 >&! simulation2


 b=  atof(argv[1]);
 N=  atoi(argv[2]);
 R=  atof(argv[3]);
 A=  atof(argv[4]);
 w_0=atof(argv[5]);
 f=  atof(argv[6]);

 v=b*clight;
 C=2.*pi*R;
 dN=N;
 w=w_0*f;

/*
 Q=dN/(4.*w_0);
// printf("%f %f\n",Q,Q);

 printf("beta   %f\n",b);
 printf("vlcty  %f\n",v);
 printf("N      %d\n",N);
 printf("Radius %f\n",R);
 printf("Circum %f\n",C);
 printf("Betatron amplitude %f\n",A);
 printf("Betatron frequency %f\n\n",w);
*/

 for(i=0;i<N;i++){
  t=i;
  a=2.*M_PI*t/dN;
  r=R+A*cos(w*a);
  printf("%f %f\n",r*cos(a),r*sin(a));
 }

 return 0;
}
