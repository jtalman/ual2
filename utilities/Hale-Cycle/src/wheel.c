#include<GL/freeglut.h>
#include<stdio.h>

void mouseWheel(int wheel, int direction, int x, int y) {
    printf("Wheel %d scrolled %s at coords (%d, %d)\n", 
           wheel, (direction > 0) ? "UP" : "DOWN", x, y);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutCreateWindow("FreeGLUT Mouse Wheel Example");
    
    glutDisplayFunc(display);
    glutMouseWheelFunc(mouseWheel); // Register mouse wheel callback
    
    glutMainLoop();
    return 0;
}
