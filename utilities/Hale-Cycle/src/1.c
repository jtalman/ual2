#include <stdio.h>
#include <math.h>

#define HALE_CYCLE_YEARS 22.0
#define SCHWABE_CYCLE_YEARS 11.0

typedef struct {
    double year;
    double schwabe_phase; // 0 to 1 for the 11-year sunspot cycle
    int magnetic_polarity; // +1 or -1 for the 22-year Hale cycle
    double field_strength;
} HaleCycleState;

void update_hale_state(HaleCycleState *state, double current_time_years) {
    state->year = current_time_years;
    
    // Schwabe phase (11-year periodicity)
    double schwabe_mod = fmod(current_time_years, SCHWABE_CYCLE_YEARS);
    state->schwabe_phase = schwabe_mod / SCHWABE_CYCLE_YEARS;
    
    // Hale polarity reversal occurs every 22 years (flips every 11 years at max/reversal)
    double hale_mod = fmod(current_time_years, HALE_CYCLE_YEARS);
    state->magnetic_polarity = (hale_mod < SCHWABE_CYCLE_YEARS) ? 1 : -1;
    
    // Approximate combined magnetic field and sunspot proxy wave
    double base_activity = sin(state->schwabe_phase * 2.0 * M_PI);
    state->field_strength = base_activity * state->magnetic_polarity;
}

int main() {
    HaleCycleState sun;
    double test_years[] = {2.5, 12.0, 15.0, 23.5};
    int num_tests = sizeof(test_years) / sizeof(test_years[0]);

    for (int i = 0; i < num_tests; i++) {
        update_hale_state(&sun, test_years[i]);
        printf("Year: %.1f | Polarity: %d | Field Strength: %.2f\n", 
               sun.year, sun.magnetic_polarity, sun.field_strength);
    }
    
    return 0;
}
