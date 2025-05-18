#include<stdio.h>
#include<stdlib.h>
#include<math.h>
void main(int argc,char* argv[]){
 int i=0;
 double R=atof(argv[1]);
 double S=atof(argv[2]);
 double X,thtIn;
 double fac=180./3.141592653589793;
 double x,y;

 for(i=0;i<argc;i++){
  printf("%d %s\n",i,argv[i]);
 }
 printf("R: %e\n",R);
 printf("S: %e\n",S);

 X=sqrt(R*S+S*S/4.);
 printf("X: %e\n",X);
 thtIn=atan(R/X);
 printf("thtIn (rad): %e\n",thtIn);
 printf("thtIn (deg): %e\n",thtIn*fac);

 x=X*cos(thtIn);
 y=X*sin(thtIn);
 printf("INJECT@ (%e,%e)\n",x,y);
}
