#include <stdio.h>
#include <math.h>

// Function to calculate instantaneous EMF using the derivative of flux: d(Phi)/dt
// Example flux function: Phi(t) = B * A * cos(w*t)
double get_instantaneous_flux_derivative(double B, double A, double w, double t) {
    // Derivative of B * A * cos(w*t) with respect to t is -w * B * A * sin(w*t)
    return -w * B * A * sin(w * t);
}

int main() {
    int N; // Number of turns
    double B, A, w, t; // Magnetic Field (Tesla), Area (m^2), Angular Velocity (rad/s), Time (s)

    printf("--- Faraday's Law of Induction Calculator ---\n");

    // 1. Calculate Instantaneous EMF (Using a continuous function)
    printf("\n--- 1. Instantaneous EMF ---\n");
    printf("Enter Magnetic Field (B in Tesla): ");
    scanf("%lf", &B);
    printf("Enter Coil Area (A in m^2): ");
    scanf("%lf", &A);
    printf("Enter Angular Velocity (w in rad/s): ");
    scanf("%lf", &w);
    printf("Enter Time (t in seconds): ");
    scanf("%lf", &t);
    printf("Enter Number of turns (N): ");
    scanf("%d", &N);

    // E = -N * (d(Phi)/dt)
    double d_phi_dt = get_instantaneous_flux_derivative(B, A, w, t);
    double instantaneous_emf = -N * d_phi_dt;
    
    printf("Instantaneous Induced EMF: %.2f Volts\n", instantaneous_emf);

    // 2. Calculate Average EMF (Using discrete intervals)
    printf("\n--- 2. Average EMF (Discrete Change) ---\n");
    double phi1, phi2, dt;
    printf("Enter initial magnetic flux (Phi1 in Webers): ");
    scanf("%lf", &phi1);
    printf("Enter final magnetic flux (Phi2 in Webers): ");
    scanf("%lf", &phi2);
    printf("Enter time interval (dt in seconds): ");
    scanf("%lf", &dt);

    double delta_phi = phi2 - phi1;
    double average_emf = -N * (delta_phi / dt);

    printf("Average Induced EMF: %.2f Volts\n", average_emf);

    return 0;
}
