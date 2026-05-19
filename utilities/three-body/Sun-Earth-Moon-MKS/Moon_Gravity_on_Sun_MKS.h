void Moon_Gravity_on_Sun_MKS(){

// vector from Sun to Moon
    RS_M.x= M.x-S.x;
    RS_M.y= M.y-S.y;
    RS_M.z= M.z-S.z;
// vector from Sun to Moon

    long double d_SM = sqrt(RS_M.x * RS_M.x + RS_M.y * RS_M.y + RS_M.z * RS_M.z);

    long double fM_S = (G_MKS * M.m * S.m) / (d_SM * d_SM * d_SM);

    S.ax += fM_S * RS_M.x / S.m;
    S.ay += fM_S * RS_M.y / S.m;
    S.az += fM_S * RS_M.z / S.m;

}
