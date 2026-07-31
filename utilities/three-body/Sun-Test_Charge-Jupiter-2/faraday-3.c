#include <stdio.h>
#include <math.h>

// Function to calculate average induced EMF
// flux_values: Array of magnetic flux at different time steps
// time_steps: Array of time points
// n: Number of data points
void calculate_induced_emf(double flux_values[], double time_steps[], int n) {
    printf("--- Faraday's Law of Induction: EMF Calculation ---\n");
    
    // We start at the second point to calculate the rate of change
    for (int i = 1; i < n; i++) {
        double d_flux = flux_values[i] - flux_values[i-1];
        double d_time = time_steps[i] - time_steps[i-1];
        
        if (d_time != 0) {
            // Faraday's law: EMF = - d(Flux) / dt
            double emf = - (d_flux / d_time);
            
            printf("Time: %.2f s | Flux: %.2f Wb | Induced EMF: %.2f V\n", 
                   time_steps[i], flux_values[i], emf);
        }
    }
}

int main() {
    // Example data: Magnetic flux changing over 5 seconds
    double time_steps[] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0};
    double flux_values[] = {10.0, 8.0, 5.0, 5.0, 2.0, 0.0};
    int n = sizeof(time_steps) / sizeof(time_steps[0]);
    
    calculate_induced_emf(flux_values, time_steps, n);
    
    return 0;
}
