#include<GL/glut.h>
#include<vector>
#include<cstdlib>

struct Point
{
    float x, y;
    unsigned char r, g, b, a;
};
std::vector< Point > points;

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50, 50, -50, 50, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // draw
    glColor3ub( 255, 255, 255 );
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY );
    glVertexPointer( 2, GL_FLOAT, sizeof(Point), &points[0].x );
    glColorPointer( 4, GL_UNSIGNED_BYTE, sizeof(Point), &points[0].r );
    glPointSize( 3.0 );
    glDrawArrays( GL_POINTS, 0, points.size() );
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_COLOR_ARRAY );

    glFlush();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_STENCIL);

    glutInitWindowSize(640,480);
    glutCreateWindow("Random Points");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

     // populate points
    for( size_t i = 0; i < 1000; ++i )
    {
        Point pt;
        pt.x = -50 + (rand() % 100);
        pt.y = -50 + (rand() % 100);
        pt.r = rand() % 255;
        pt.g = rand() % 255;
        pt.b = rand() % 255;
        pt.a = 255;
        points.push_back(pt);
    }    

    glutMainLoop();
    return 0;
}
