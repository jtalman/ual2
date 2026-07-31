#include <stdio.h>
#include <math.h>

// Calculate the Force due to a Changing Magnetic Field (Faraday's Law)
// F = q * E_induced
double calculate_faraday_force(double q, double delta_B, double area, double delta_t) {
    // E_induced = - (d/dt)(B * A) / circumference_or_path_length
    // This example uses a general simplified scalar approximation (Area in m^2)
    double dPhi_dt = (delta_B * area) / delta_t; 
    double E_induced = fabs(dPhi_dt); // For magnitude
    
    double force = q * E_induced;
    return force;
}

int main() {
    double charge = 1.602e-19; // e.g., an electron or proton in Coulombs
    double delta_B = 0.5;      // Magnetic field change in Teslas
    double area = 0.01;        // Loop area in meters squared
    double delta_t = 0.001;    // Time step in seconds
    
    double f = calculate_faraday_force(charge, delta_B, area, delta_t);
    printf("Force on charged particle: %e N\n", f);
    
    return 0;
}
