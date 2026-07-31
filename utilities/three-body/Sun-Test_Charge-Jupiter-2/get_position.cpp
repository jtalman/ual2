void get_position(Body B, int steps){
 long double total_mass = S.m + J.m;
 S.vx = -(J.vx * J.m) / S.m;
 S.vy = -(J.vy * J.m) / S.m;
 S.vz = -(J.vz * J.m) / S.m;

 long double S_ax=0,     S_ay=0,     S_az=0,     J_ax=0,     J_ay=0,     J_az=0,     TM_ax=0,     TM_ay=0,     TM_az=0;
 long double S_ax_new=0, S_ay_new=0, S_az_new=0, J_ax_new=0, J_ay_new=0, J_az_new=0, TM_ax_new=0, TM_ay_new=0, TM_az_new=0;

// Initial accelerations
/*
 Body1_Force_from_Body2(&S,  &J, &S_ax,  &S_ay,  &S_az);
 Body1_Force_from_Body2(&J,  &S, &J_ax,  &J_ay,  &J_az);
 Body1_Force_from_Body2(&TM, &J, &TM_ax, &TM_ay, &TM_az);
 Body1_Force_from_Body2(&TM, &S, &TM_ax, &TM_ay, &TM_az);
*/

 Body1_Force_from_Body2(&S,  &J,  &S_ax,  &S_ay,  &S_az);
 Body1_Force_from_Body2(&S,  &TM, &S_ax,  &S_ay,  &S_az);

 Body1_Force_from_Body2(&J,  &S,  &J_ax,  &J_ay,  &J_az);
 Body1_Force_from_Body2(&J,  &TM, &J_ax,  &J_ay,  &J_az);

 Body1_Force_from_Body2(&TM, &J,  &TM_ax, &TM_ay, &TM_az);
 Body1_Force_from_Body2(&TM, &S,  &TM_ax, &TM_ay, &TM_az);

 for (int i = 0; i < steps; i++) {
  ldi = i;
  long double t = ldi * dt; 
//fprintf(fptr, "%Le %Le %Le %Le %Le %Le %Le %Le %Le %Le\n", t, S.x, S.y, S.z, J.x, J.y, J.z, TM.x, TM.y, TM.z);
//fprintf(stderr, "%Le %Le %Le %Le %Le %Le %Le %Le %Le %Le\n", t, S.x, S.y, S.z, J.x, J.y, J.z, TM.x, TM.y, TM.z);
// printf("%Le %Le %Le %Le %Le %Le %Le %Le %Le %Le\n", t, S.x, S.y, S.z, J.x, J.y, J.z, TM.x, TM.y, TM.z);

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
/*
  Body1_Force_from_Body2(&S,  &J, &S_ax_new,  &S_ay_new,  &S_az_new);
  Body1_Force_from_Body2(&J,  &S, &J_ax_new,  &J_ay_new,  &J_az_new);
  Body1_Force_from_Body2(&TM, &J, &TM_ax_new, &TM_ay_new, &TM_az_new);
  Body1_Force_from_Body2(&TM, &S, &TM_ax_new, &TM_ay_new, &TM_az_new);
*/

  Body1_Force_from_Body2(&S,  &J,  &S_ax_new, &S_ay_new,  &S_az_new);
  Body1_Force_from_Body2(&S,  &TM, &S_ax_new, &S_ay_new,  &S_az_new);

  Body1_Force_from_Body2(&J,  &S,  &J_ax_new, &J_ay_new,  &J_az_new);
  Body1_Force_from_Body2(&J,  &TM, &J_ax_new, &J_ay_new,  &J_az_new);

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
}
