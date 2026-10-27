#include <stdio.h>
#include <math.h>

// MKS System Constants
#define GG 6.67430e-11          // GGravitational constant (m^3 kg^-1 s^-2)
const double G = GG;

#define MASS_MOON 7.342e22     // Mass of the Moon (kg)
const double M_M = MASS_MOON;

#define MASS_EARTH 5.972e24     // Mass of the Earth (kg)
const double E_M = MASS_EARTH;

#define RADIUS_EARTH 6.371e6   // Mean radius of the Earth (m)
const double E_R = RADIUS_EARTH;

#define DISTANCE_MOON 3.844e8  // Average Earth-Moon distance (m)
const double M_D = DISTANCE_MOON;


int main() {
    // 1. Gravitational acceleration exerted by the Moon at the center of the Earth
//  double a_center = (G * MASS_MOON) / (DISTANCE_MOON * DISTANCE_MOON);
    double a_center = (G * M_M) / (M_D * M_D);

    // 2. Acceleration on the side closest to the Moon (Distance = D - R)
    double dist_near = DISTANCE_MOON - RADIUS_EARTH;
    double a_near = (G * MASS_MOON) / (dist_near * dist_near);

    // 3. Acceleration on the side farthest from the Moon (Distance = D + R)
    double dist_far = DISTANCE_MOON + RADIUS_EARTH;
    double a_far = (G * MASS_MOON) / (dist_far * dist_far);

    // 4. Tidal forces (differential acceleration relative to Earth's center)
    // Positive values indicate a net outward stretching force (bulges)
    double tide_near = a_near - a_center;
    double tide_far = a_far - a_center; 

    // 5. Analytical approximation of tidal acceleration: 2 * G * M * R / D^3
    double tide_approx = (2.0 * G * MASS_MOON * RADIUS_EARTH) / pow(DISTANCE_MOON, 3);

    // Display Results
    printf("=== Moon's Gravitational Effects on Earth (MKS Units) ===\n\n");
    printf("Base Gravitational Pull at Earth Center : %.3e m/s^2\n", a_center);
    printf("Pull on Near Side (Facing Moon)         : %.3e m/s^2\n", a_near);
    printf("Pull on Far Side (Away from Moon)       : %.3e m/s^2\n\n", a_far);
    
    printf("=== Resulting Tidal Forces (Relative to Center) ===\n");
    printf("Near Side Tidal Stretching Acceleration : %.3e m/s^2 (Outward)\n", tide_near);
    printf("Far Side Tidal Stretching Acceleration  : %.3e m/s^2 (Outward)\n", -tide_far);
    printf("Analytical Approximation (2 G M R / D^3)   : %.3e m/s^2\n", tide_approx);

    return 0;
}
