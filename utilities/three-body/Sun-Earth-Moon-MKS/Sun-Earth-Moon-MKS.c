#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"Constants-MKS.h"

typedef struct{
 long double x, y, z;
 long double vx, vy, vz;
 long double ax, ay, az;
 long double old_ax, old_ay, old_az;
 long double m;
}Body;

Body S;
Body E;
Body M;

typedef struct{
 long double x, y, z;
}vector;

vector RE_S={0};
#include"Sun_Gravity_on_Earth_MKS.h"
vector RE_M={0};
#include"Moon_Gravity_on_Earth_MKS.h"

vector RM_S={0};
#include"Sun_Gravity_on_Moon_MKS.h"
vector RM_E={0};
#include"Earth_Gravity_on_Moon_MKS.h"

vector RS_E={0};
#include"Earth_Gravity_on_Sun_MKS.h"
vector RS_M={0};
#include"Moon_Gravity_on_Sun_MKS.h"

// Velocity Verlet Integration
void verlet_step(Body *b, long double dt) {
// 1. Update position: x(t+dt) = x(t) + v(t)dt + 0.5*a(t)dt^2
 b->x += b->vx * dt + 0.5 * b->ax * dt * dt;
 b->y += b->vy * dt + 0.5 * b->ay * dt * dt;
 b->z += b->vz * dt + 0.5 * b->az * dt * dt;

// Store current acceleration as old_acceleration
 b->old_ax = b->ax;
 b->old_ay = b->ay;
 b->old_az = b->az;

// Note: To be strict, accelerations should be updated here using new positions,
// then update velocity using average of new and old acceleration.
// Simplified Velocity Verlet used here for brevity:
 b->vx += b->ax * dt;
 b->vy += b->ay * dt;
 b->vz += b->az * dt;
}

int main(int argc, char*argv[]){

 if(argc!=7){
  printf("usage ./a.out  secs  steps  Earth_dist  Earth_vel  Earth_Moon_dist  Moon_extra_vel\n");
  printf("e.g.  ./a.out  1e+3  3000    1.496e11    29780        3.844e8          1022\n");
  exit(0);
 }

         int       i = 0;
 long double     ldi = i;
 long double      dt = atof(argv[1]);
         int   steps = atoi(argv[2]);
 long double ld_step = steps;
 long double     day = dt*ld_step/sd;
 long double day_max = -999;
 long double     one = 1;
 long double   sixty = 60;
 long double  ang_XE = 0;

                d_SE = atof(argv[3]);
                 E_v = atof(argv[4]);

                d_EM = atof(argv[5]);;
               M_x_v = atof(argv[6]);

                 M_v = E_v + M_x_v;


 FILE *fptr;
 fptr = fopen("Sun-Earth-Moon-MKS.out", "w");

 long double R_SE_dot_R_EM;
 long double dot_max = -999;

 Body SX =   {0,           0, 0,    0, 0,   0,    0, 0, 0,    0, 0, 0,    S_m};
 Body EX = {d_SE,        0, 0,    0, E_v, 0,    0, 0, 0,    0, 0, 0,    E_m};
 Body MX =  {d_SE + d_EM, 0, 0,    0, M_v, 0,    0, 0, 0,    0, 0, 0,    M_m};

 S =   SX;
 E = EX;
 M =  MX;

 for (; i < steps; i++){
  ldi=i;
  day = dt*ldi/sd;

  E.ax = 0;                    // acceleration vector needs some work!
  E.ay = 0;                    //      see void verlet_step...
  E.az = 0;

  M.ax  = 0;                    // acceleration vector needs some work!
  M.ay  = 0;                    //      see void verlet_step...
  M.az  = 0;

// Calculate current forces
  Sun_Gravity_on_Earth_MKS();
  Moon_Gravity_on_Earth_MKS();

  Sun_Gravity_on_Moon_MKS();
  Earth_Gravity_on_Moon_MKS();

  Earth_Gravity_on_Sun_MKS();
  Moon_Gravity_on_Sun_MKS();

  // Update positions
  verlet_step(&E, dt);
  verlet_step(&M, dt);
  verlet_step(&S, dt);

  ang_XE = acos( one*RS_E.x / au );

  R_SE_dot_R_EM = RS_E.x*RE_M.x + RS_E.y*RE_M.y + RS_E.z*RE_M.z;
  R_SE_dot_R_EM = R_SE_dot_R_EM / au / d_EM;
  if(R_SE_dot_R_EM>dot_max){
   dot_max=R_SE_dot_R_EM;
   day_max=day;
  }

//if (i % 10 == 0) {
   fprintf(fptr, "%Le %Le %Le %Le %Le %Le %Le %Le %Le\n", E.x, E.y, M.x, M.y, E.x-M.x,E.y-M.y, R_SE_dot_R_EM, day, ang_XE*sixty);
//}
 }

 printf("\n\n");
 printf("\033[1m");
 printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
 printf("dot_max: %Le, day_max (simulated month period) : %Le\n",dot_max,day_max);
 printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
 printf("\033[0m");
 fclose(fptr);
 return 0;
}
