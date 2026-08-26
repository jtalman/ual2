#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

// Structure to represent a point in 3D space
struct Point3D {
    glm::vec3 position;
    float radius; // Bounding click tolerance radius
};

// Converts 2D screen coordinates into a 3D ray direction vector
glm::vec3 CalculatePickRay(double mouseX, double mouseY, int screenWidth, int screenHeight, 
                            const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) 
{
    // Step 1: Convert screen coordinates to Normalized Device Coordinates (NDC)
    float x = (2.0f * mouseX) / screenWidth - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / screenHeight; // Invert Y-axis
    float z = 1.0f; 
    glm::vec3 rayNDC = glm::vec3(x, y, z);

    // Step 2: Convert NDC to Homogeneous Clip Coordinates
    glm::vec4 rayClip = glm::vec4(rayNDC.x, rayNDC.y, -1.0f, 1.0f);

    // Step 3: Convert Clip Coordinates to Eye/Camera Coordinates
    glm::vec4 rayEye = glm::inverse(projectionMatrix) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f); // Unproject z and set w to vector

    // Step 4: Convert Eye Coordinates to World Coordinates
    glm::vec3 rayWorld = glm::vec3(glm::inverse(viewMatrix) * rayEye);
    
    // Normalize the final directional ray vector
    return glm::normalize(rayWorld);
}

// Tests intersection between the ray and a 3D point bounding sphere
bool RayIntersectsPoint(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, 
                         const Point3D& point, float& t) 
{
    // Vector from ray origin to the center of the point sphere
    glm::vec3 oc = point.position - rayOrigin;
    
    // Project oc onto the ray direction vector
    float tClosest = glm::dot(oc, rayDirection);
    
    // Find the shortest distance squared from point center to the ray
    float rayDistanceSq = glm::dot(oc, oc) - (tClosest * tClosest);
    
    // Check if the ray passes within the point's clickable radius
    if (rayDistanceSq > (point.radius * point.radius)) {
        return false; 
    }

    t = tClosest; // Distance along the ray to the intersection point
    return t > 0.0f; 
}

// Scans all scene points and returns the index of the closest clicked point
int PickClosestPoint(double mouseX, double mouseY, int screenWidth, int screenHeight,
                     const glm::vec3& cameraPos, const glm::mat4& view, const glm::mat4& proj,
                     const std::vector<Point3D>& pointCloud) 
{
    // Calculate the ray direction vector
    glm::vec3 rayDir = CalculatePickRay(mouseX, mouseY, screenWidth, screenHeight, view, proj);
    
    int intersectedIdx = -1;
    float closestT = std::numeric_limits<float>::max();

    for (size_t i = 0; i < pointCloud.size(); ++i) {
        float t = 0.0f;
        if (RayIntersectsPoint(cameraPos, rayDir, pointCloud[i], t)) {
            // Check if this point is closer than previously hit points
            if (t < closestT) {
                closestT = t;
                intersectedIdx = static_cast<int>(i);
            }
        }
    }
    return intersectedIdx; // Returns -1 if no point was clicked
}

int main() {
    // Mock Setup: Camera matrices and window configuration
    int width = 800, height = 600;
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
    glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width/height, 0.1f, 100.0f);

    // Populate a sample array of clickable 3D points
    std::vector<Point3D> points = {
        { glm::vec3(0.0f, 0.0f, 0.0f),  0.2f }, // Point 0 (Origin)
        { glm::vec3(1.0f, 1.0f, -2.0f), 0.2f }, // Point 1
        { glm::vec3(-1.5f, 0.5f, 1.0f), 0.2f }  // Point 2
    };

    // Simulate clicking in the center of the screen (should hit the origin point)
    double mockMouseX = 400.0; 
    double mockMouseY = 300.0;

    int clickedPointIndex = PickClosestPoint(mockMouseX, mockMouseY, width, height, cameraPos, view, proj, points);

    if (clickedPointIndex != -1) {
        std::cout << "Successfully picked point index: " << clickedPointIndex << std::endl;
    } else {
        std::cout << "No point selected." << std::endl;
    }

    return 0;
}
