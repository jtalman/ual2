#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"Constants-MKS.h"

typedef struct{
 long double x, y, z;
 long double vx, vy, vz;
 long double Mx, My, Mz;
 long double m;
 long double q;
}Body;

Body S ={
 .x =  0, .y =  0, .z =  0,
 .vx = 0, .vy = 0, .vz = 0,
 .Mx = 0, .My = 0, .Mz = 0,
 .m =  M_SUN_MKS,
 .q =  Q_SUN_MKS
};

Body J ={
 .x =  D_JUPITER_X_MKS, .y =  D_JUPITER_Y_MKS, .z =  D_JUPITER_Z_MKS,
 .vx = V_JUPITER_X_MKS, .vy = V_JUPITER_Y_MKS, .vz = V_JUPITER_Z_MKS,
 .Mx = 0,               .My = 0,               .Mz = 0,
 .m =  M_JUPITER_MKS,
 .q =  Q_JUPITER_MKS
};

// Calculate gravitational acceleration on a body due to another
void Body1_Force_from_Body2(Body *b1, Body *b2, long double *ax, long double *ay, long double *az) {
 long double dx = b2->x - b1->x;
 long double dy = b2->y - b1->y;
 long double dz = b2->z - b1->z;
 long double r = sqrt(dx*dx + dy*dy + dz*dz);
    
 long double force_magnitude = (G * b1->m * b2->m) / (r * r * r);
 *ax += force_magnitude * dx / b1->m;
 *ay += force_magnitude * dy / b1->m;
 *az += force_magnitude * dz / b1->m;
}

int main(int argc, char*argv[]){

 if(argc!=14){
  printf("usage ./a.out  secs   steps   TMx    TMy  TMz  TMvx  TMvy  TMvz  TMMx  TMMy  TMMz  TMm  TMq\n");
  printf("e.g.  ./a.out  86400  4500  1.496e11  0    0    0   29780   0     0     0     0     1    1 > Gravity__Sun-Test_Mass-Jupiter-Lorentz_prelude.out\n");
  exit(0);
 }

         int        i = 0;
 long double      ldi = i;
 long double       dt = atof(argv[1]);
 long double   dsteps = atof(argv[1]);
         int    steps = (int)dsteps;

 long double      TMx = atof(argv[3]);
 long double      TMy = atof(argv[4]);
 long double      TMz = atof(argv[5]);

 long double     TMvx = atof(argv[6]);
 long double     TMvy = atof(argv[7]);
 long double     TMvz = atof(argv[8]);

 long double     TMMx = atof(argv[9]);
 long double     TMMy = atof(argv[10]);
 long double     TMMz = atof(argv[11]);

 long double      TMm = atof(argv[12]);
 long double      TMq = atof(argv[13]);

 Body TM ={
  .x =  TMx,  .y =  TMy,  .z =  TMz,
  .vx = TMvx, .vy = TMvy, .vz = TMvz,
  .Mx = TMMx, .My = TMMy, .Mz = TMMz,
  .m =  TMm,
  .q =  TMq
 };

 long double total_mass = S.m + J.m;
 S.vx = -(J.vx * J.m) / S.m;
 S.vy = -(J.vy * J.m) / S.m;
 S.vz = -(J.vz * J.m) / S.m;

 long double S_ax=0,     S_ay=0,     S_az=0,     J_ax=0,     J_ay=0,     J_az=0,     TM_ax=0,     TM_ay=0,     TM_az=0;
 long double S_ax_new=0, S_ay_new=0, S_az_new=0, J_ax_new=0, J_ay_new=0, J_az_new=0, TM_ax_new=0, TM_ay_new=0, TM_az_new=0;

// Initial accelerations
 Body1_Force_from_Body2(&S,  &J, &S_ax,  &S_ay,  &S_az);
 Body1_Force_from_Body2(&J,  &S, &J_ax,  &J_ay,  &J_az);
 Body1_Force_from_Body2(&TM, &J, &TM_ax, &TM_ay, &TM_az);
 Body1_Force_from_Body2(&TM, &S, &TM_ax, &TM_ay, &TM_az);

 for (int i = 0; i < steps; i++) {
  ldi = i;
  long double t = ldi * dt;
  printf("%Le %Le %Le %Le %Le %Le %Le %Le %Le %Le\n", t, S.x, S.y, S.z, J.x, J.y, J.z, TM.x, TM.y, TM.z);

// 1. Position Verlet: Update positions to half-step / full-step
  S.x += S.vx * dt + 0.5 * S_ax * dt * dt;
  S.y += S.vy * dt + 0.5 * S_ay * dt * dt;
  S.z += S.vz * dt + 0.5 * S_az * dt * dt;

  J.x += J.vx * dt + 0.5 * J_ax * dt * dt;
  J.y += J.vy * dt + 0.5 * J_ay * dt * dt;
  J.z += J.vz * dt + 0.5 * J_az * dt * dt;

  TM.x += TM.vx * dt + 0.5 * TM_ax * dt * dt;
  TM.y += TM.vy * dt + 0.5 * TM_ay * dt * dt;
  TM.z += TM.vz * dt + 0.5 * TM_az * dt * dt;

  S_ax_new=0, S_ay_new=0, S_az_new=0, J_ax_new=0, J_ay_new=0, J_az_new=0, TM_ax_new=0, TM_ay_new=0, TM_az_new=0;

// 2. Calculate new accelerations at the new positions
  Body1_Force_from_Body2(&S,  &J, &S_ax_new,  &S_ay_new,  &S_az_new);
  Body1_Force_from_Body2(&J,  &S, &J_ax_new,  &J_ay_new,  &J_az_new);
  Body1_Force_from_Body2(&TM, &J, &TM_ax_new, &TM_ay_new, &TM_az_new);
  Body1_Force_from_Body2(&TM, &S, &TM_ax_new, &TM_ay_new, &TM_az_new);

// 3. Velocity Verlet: Update velocities
  S.vx += 0.5 * (S_ax + S_ax_new) * dt;
  S.vy += 0.5 * (S_ay + S_ay_new) * dt;
  S.vz += 0.5 * (S_az + S_az_new) * dt;

  J.vx += 0.5 * (J_ax + J_ax_new) * dt;
  J.vy += 0.5 * (J_ay + J_ay_new) * dt;
  J.vz += 0.5 * (J_az + J_az_new) * dt;

  TM.vx += 0.5 * (TM_ax + TM_ax_new) * dt;
  TM.vy += 0.5 * (TM_ay + TM_ay_new) * dt;
  TM.vz += 0.5 * (TM_az + TM_az_new) * dt;

// Save new accelerations for next iteration
  S_ax = S_ax_new;
  S_ay = S_ay_new;
  S_az = S_az_new;

  J_ax = J_ax_new;
  J_ay = J_ay_new;
  J_az = J_az_new;

  TM_ax = TM_ax_new;
  TM_ay = TM_ay_new;
  TM_az = TM_az_new;
 }

 return 0;
}
