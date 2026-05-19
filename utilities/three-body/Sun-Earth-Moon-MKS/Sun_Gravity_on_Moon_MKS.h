void Sun_Gravity_on_Moon_MKS(){

// vector from Moon to Sun
    RM_S.x=S.x-M.x;
    RM_S.y=S.y-M.y;
    RM_S.z=S.z-M.z;
// vector from Moon to Sun

    long double d_MS = sqrt(RM_S.x * RM_S.x + RM_S.y * RM_S.y + RM_S.z * RM_S.z);

    long double fS_M = (G_MKS * S.m * M.m) / (d_MS * d_MS * d_MS);

    M.ax += fS_M * RM_S.x / M.m;
    M.ay += fS_M * RM_S.y / M.m;
    M.az += fS_M * RM_S.z / M.m;

}
