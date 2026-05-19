//
// Physical Constants MKS
//

#define Seconds_in_Day 8.64e4                         // Textual representation
long double sd = Seconds_in_Day;                      // Machine representation

#define         Gravitational_constant_mks  6.674e-11
//                                                          Nt * m^2 / kg^2
//                                                      (kg * m/s^2) * m^2 / kg^2
//                                                           m^3/(s^2 * kg)
long double G_MKS = Gravitational_constant_mks;

#define MSUN 1.989e30
long double S_m = MSUN;

#define MEARTH 5.972e24
long double E_m = MEARTH;

#define MMOON 7.348e22
long double M_m = MMOON;

#define AU 1.496e11
long double au = AU;

#define SE_DIST 1.496e11
long double d_SE = SE_DIST;

#define E_VEL 29780
long double E_v = E_VEL;

#define EM_DIST 3.844e8
long double d_EM = EM_DIST;

#define M_EXTRA_VEL 1022
long double M_x_v = M_EXTRA_VEL;

#define M_VEL 30802
long double M_v = M_VEL;
