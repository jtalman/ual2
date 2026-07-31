#include <stdio.h>
#include <math.h>

typedef struct {
    double x, y, z;
} Vector3;

// Function to calculate the cross product of two vectors: A x B
Vector3 crossProduct(Vector3 A, Vector3 B) {
    Vector3 result;
    result.x = A.y * B.z - A.z * B.y;
    result.y = A.z * B.x - A.x * B.z;
    result.z = A.x * B.y - A.y * B.x;
    return result;
}

int main() {
    // Parameters
    double q = 1.602e-19; // Charge of a proton in Coulombs
    Vector3 E = {0.0, 1000.0, 0.0}; // Electric field in V/m
    Vector3 v = {3.0e6, 0.0, 0.0};  // Velocity in m/s
    Vector3 B = {0.0, 0.0, 2.0};    // Magnetic field in Tesla

    // Calculate the magnetic force component: F_mag = q(v x B)
    Vector3 v_cross_B = crossProduct(v, B);
    Vector3 F_mag = {q * v_cross_B.x, q * v_cross_B.y, q * v_cross_B.z};

    // Calculate the total Lorentz force: F_total = qE + F_mag
    Vector3 F_total = {q * E.x + F_mag.x, q * E.y + F_mag.y, q * E.z + F_mag.z};

    // Output results
    printf("Magnetic Force Component (N): x: %e, y: %e, z: %e\n", F_mag.x, F_mag.y, F_mag.z);
    printf("Total Lorentz Force (N):      x: %e, y: %e, z: %e\n", F_total.x, F_total.y, F_total.z);

    return 0;
}
