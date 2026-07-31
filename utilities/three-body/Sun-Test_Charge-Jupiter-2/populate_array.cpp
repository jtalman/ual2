#include <iostream>
#include <vector>

// Define your Point structure
struct Point {
    double x;
    double y;
};

int main() {
    // Two source arrays
    double x_coords[] = {1.0, 2.0, 3.0, 4.0};
    double y_coords[] = {5.0, 6.0, 7.0, 8.0};
    
    // Calculate the number of elements
    size_t num_points = sizeof(x_coords) / sizeof(x_coords[0]);

    // Initialize the vector
    std::vector<Point> points;
    
    // Optimizing memory allocations
    points.reserve(num_points);

    // Combine arrays into the vector of Points
    for (size_t i = 0; i < num_points; ++i) {
        // emplace_back constructs the Point directly inside the vector
        points.emplace_back(Point{x_coords[i], y_coords[i]});
    }

    // Verify output
    for (const auto& pt : points) {
        std::cout << "(" << pt.x << ", " << pt.y << ")\n";
    }

    return 0;
}
