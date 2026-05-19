void Moon_Gravity_on_Earth_MKS(){

// vector from Earth to Moon
    RE_M.x=M.x-E.x;
    RE_M.y=M.y-E.y;
    RE_M.z=M.z-E.z;
// vector from Earth to Moon

    long double d_EM = sqrt(RE_M.x * RE_M.x + RE_M.y * RE_M.y + RE_M.z * RE_M.z);

    long double fM_E = (G_MKS * M.m * E.m) / (d_EM * d_EM * d_EM);

    E.ax += fM_E * RE_M.x / E.m;
    E.ay += fM_E * RE_M.y / E.m;
    E.az += fM_E * RE_M.z / E.m;

}
