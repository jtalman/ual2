#include <GL/glut.h>
#include <vector>

struct Point {
    float x, y;
};

// Define an array or vector of points
std::vector<Point> points = {
    {0.0f, 0.0f}, {0.5f, 0.5f}, {-0.5f, 0.5f}, 
    {-0.5f, -0.5f}, {0.5f, -0.5f}
};

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Set point attributes
    glColor3f(1.0f, 0.0f, 0.0f); // Red points
    glPointSize(5.0f);            // Size in pixels

    // Render the array of points
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, sizeof(Point), points.data());
    glDrawArrays(GL_POINTS, 0, points.size());
    glDisableClientState(GL_VERTEX_ARRAY);

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Plot Array of Points");
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // White background
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
