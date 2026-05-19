void Earth_Gravity_on_Moon_MKS(){

// vector from Moon to Earth
    RM_E.x= -(M.x-E.x);
    RM_E.y= -(M.y-E.y);
    RM_E.z= -(M.z-E.z);
// vector from Moon to Earth

    long double d_ME = sqrt(RM_E.x * RM_E.x + RM_E.y * RM_E.y + RM_E.z * RM_E.z);

    long double fE_M = (G_MKS * M.m * E.m) / (d_ME * d_ME * d_ME);

    M.ax += fE_M * RM_E.x / M.m;
    M.ay += fE_M * RM_E.y / M.m;
    M.az += fE_M * RM_E.z / M.m;

}
