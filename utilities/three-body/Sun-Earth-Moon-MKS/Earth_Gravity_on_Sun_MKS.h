void Earth_Gravity_on_Sun_MKS(){

// vector from Sun to Earth
    RS_E.x = E.x-S.x;
    RS_E.y = E.y-S.y;
    RS_E.z = E.z-S.z;
// vector from Sun to Earth

    long double d_SE = sqrt(RS_E.x * RS_E.x + RS_E.y * RS_E.y + RS_E.z * RS_E.z);

    long double fE_S = (G_MKS * E.m * S.m) / (d_SE * d_SE * d_SE);

    S.ax += fE_S * RS_E.x / S.m;
    S.ay += fE_S * RS_E.y / S.m;
    S.az += fE_S * RS_E.z / S.m;

}
