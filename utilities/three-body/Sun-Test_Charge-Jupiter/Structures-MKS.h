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
