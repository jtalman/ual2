#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"../clr"

  long double PI = 3.14159265359; // 'nuff said!                              // ca 3.14
  long double c  = 2.9979e+08;    // speed of light                           // ca 3e8

// NON LEAP YEAR !!!
  long double T = 365;           // days in a calendar year     [days]       // NON LEAP YEAR !!!
// NON LEAP YEAR !!!
//
  long double S = 86400;         // seconds in a day            [s]          // 60 * 60 * 24
  long double A = 1.4960e+11;    // Astronomical Unit in meters [m]          // definition based on observation
//long double R = A;             // representative value for Sun-Eart distance
//#define D  1.4960e+11;    // Sun to Earth distance       [m]          // varies
//#define V  2.9785e+04;    // speed of earth              [m/s]        // derived !

  long double G  = 6.6743e-11;    // Gravitational constant      [Nt m^2 / kg^2]
  long double M  = 1.9885e+30;    // Mass of the Sun             [kg]
  long double m  = 5.9722e+24;    // Mass of the Earth           [kg]

// k = 7.9262e+44     // "Gravitational strength

int main(int argc, char*argv[]){

 long double R = A;             // representative value for Sun-Eart distance

 long double gammaX   =atof(argv[1]);
 long double lambdaX  =atof(argv[2]);
 long double epsilonX =atof(argv[3]);
 long double kappaX   =atof(argv[4]);

 if(argc!=6){
  printf("Usage: e.g. ./precess-ellipse gamma lambda epsilon kappa Ngrid\n");
  exit(1);
 }

  printf("\n");

//long double T = C/V;
 printf("                    T            %3.0f days                       typical Earth revolution period\n",(float)T); 
 printf("                                                                    (in\n"); 
 printf("                    T[s]      %+25.15Le            seconds)\n\n",T*S); 

 printf("                    R         %+25.15Le         typical Earth radius to Sun in meters\n",R); 

 long double C = 2.L*PI*R;
 printf("                    C         %+25.15Le         typical Earth orbit length in meters\n",C); 

 long double V = C/T/S;
 printf("                    V         %+25.15Le         typical Earth velocity [m/s]\n",V); 

 long double betaV = V/c;
 printf("                    betaV     %+25.15Le         typical Earth beta []\n",betaV); 

 long double gammaV = 1.L/sqrt(1.L-betaV*betaV);
 printf("                    gammaV    %+25.15Le         typical Earth gamma []\n\n",gammaV); 

 long double L = m*R*V;
 printf("                    L         %+25.15Le         typical Earth angular momentum [kg m/s]\n",L); 

 long double thetaDot = V/R;
 printf("                    thetaDot  %+25.15Le         typical Earth angular velocity [1/s]\n",thetaDot); 

 long double LDC = gammaV*m*R*R*thetaDot;
 printf("                    LDC       %+25.15Le         typical Earth angular momentum (double checked via Munoz eqn \"1.5\")\n\n",LDC); 

 long double k = G*M*m;
 printf("                    k         %+25.15Le         fixed measure of Sun Earth gravitational strength\n",k); 

 long double E = gammaV*m*c*c - k/A;
 printf("                    E         %+25.15Le         typical Earth energy\n",E); 

 long double EovermcSqu = E/m/c/c;
 printf("                    EovrmcSqu %+25.15Le         typical Earth energy in units of mc^2\n",EovermcSqu); 

 long double Lcoverk = L*c/k;
 printf("                    Lcoverk   %+25.15Le         typical Earth Lc/k                   \n",Lcoverk); 

 long double gammaDC = (E+k/R)/m/c/c;
 printf("                    gammaDC   %+25.15Le         typical Earth gamma (double checked via Munoz eqn 8)\n\n",gammaDC); 

 long double kappaSqu = 1.L-(k/L/c)*(k/L/c);
 printf("                    kappaSqu  %+25.15Le         typical kappa squared\n",kappaSqu); 

 long double kappa = sqrt(kappaSqu);
 printf("                    kappa     %+25.15Le         typical kappa\n\n",kappa); 

 long double lambda = (kappa*L*c)*(kappa*L*c)/k/E;
 printf("                    lambda    %+25.15Le         typical lambda\n",lambda); 

 long double h0 = c*sqrt( EovermcSqu - kappaSqu );
 printf("                    h0        %+25.15Le         typical h0\n\n",h0); 

 long double epsilon = Lcoverk*sqrt( 1.L-(kappa/EovermcSqu)*(kappa/EovermcSqu) );
 printf("                    epsilon   %+25.15Le         typical epsilon\n",epsilon); 

 printf(RESET);

 long double Ngrid=atof(argv[5]);
 long double delThta=2.*PI/Ngrid;
 long double delThtH=delThta/2.L;
 long double tau=2.*M_PI;
 long double Thta=0.; 
 long double ThtM=0.; 
 long double r=0.; 
   long double rA=0.; 
 long double x=0.; 
 long double y=0.; 
 long double dt=0.;
//long double T=0.;

 FILE *fptr;
 fptr = fopen("orbit", "w");
 for(Thta=0.;Thta<tau;Thta+=delThta){
  ThtM=Thta+delThtH;
//r=lambda/( 1.L+L*h0*cos(kappa*ThtM/E/k) );
  r=lambda/( 1.L+epsilon*(kappa*ThtM/E/k) );
  x=r*cos(Thta);
  y=r*sin(Thta);
//dt=r/k*(L-mp*r*ht)*delThta;
//dt=r/L/c/c*(k+r*EM)*delThta;
//T=T+dt;
//fp << x << " " << y << "\n";
  fprintf(fptr, "%+25.15Le, %+25.15Le\n",x,y);
 }
 fclose(fptr);

//printf("T (simple sum)      %+25.15Le [s]\n",T); 

  printf("\n");
}
