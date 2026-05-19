void Sun_Gravity_on_Earth_MKS(){

// vector from Earth to Sun
    RE_S.x=S.x-E.x;
    RE_S.y=S.y-E.y;
    RE_S.z=S.z-E.z;
// vector from Earth to Sun

    long double d_ES = sqrt(RE_S.x * RE_S.x + RE_S.y * RE_S.y + RE_S.z * RE_S.z);

    long double fS_E = (G_MKS * S.m * E.m) / (d_ES * d_ES * d_ES);

    E.ax += fS_E * RE_S.x / E.m;
    E.ay += fS_E * RE_S.y / E.m;
    E.az += fS_E * RE_S.z / E.m;

}
