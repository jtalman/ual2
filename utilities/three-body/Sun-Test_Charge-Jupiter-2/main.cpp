#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include<iostream>
#include<sstream>
#include<iomanip>
#include<fstream>
#include<vector>
#include<cstdlib>

struct Point {
 float x, y;
};

#include"plot_parameters.h"

#define STEPS 4500

  std::vector<Point> points;
  std::vector<Point> Cpoint;
  int C=0;

  std::vector<Point> points_Sun;
  std::vector<Point> Cpoint_Sun;

  std::vector<Point> points_TM;
  std::vector<Point> Cpoint_TM;

  std::vector<Point> points_Jupiter;
  std::vector<Point> Cpoint_Jupiter;

//points.reserve(STEPS);
//std::vector<Point>points[STEPS]={(0,0)};
//std::vector<Point>points[STEPS]={0};

#include<GL/glut.h>

//#include<glad/glad.h>
//#include<GL/glad.h>
#include<glm/glm.hpp>

/*
#include"display_cb.cpp"
#include"display_2D.cpp"
*/

  #include"./glui-2.36/src/include/GL/glui.h"
//#include"../../../../../../../gluiSandbox/glui-2.36/src/include/GL/glui.h"

#include"Bmp.h"
#include"Sphere.h"

#include"Constants-MKS.h"
#include"Structures-MKS.h"

//#include"get_position.cpp"

int handle=0;

// GLUT CALLBACK functions
void displayCB();
void reshapeCB(int w, int h);
void timerCB(int millisec);
void keyboardCB(unsigned char key, int x, int y);
void mouseCB(int button, int stat, int x, int y);
void mouseMotionCB(int x, int y);

void initGL();
int  initGLUT(int argc, char **argv);
bool initSharedMem();
void clearSharedMem();
void initLights();
void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ);
void drawString(const char *str, int x, int y, float color[4], void *font);
void drawString3D(const char *str, float pos[3], float color[4], void *font);
void toOrtho();
void toPerspective();
GLuint loadTexture(const char* fileName, bool wrap=true);

// constants
const int   SCREEN_WIDTH    = 1130;    //    1200;    //     800;     //    1500;
const int   SCREEN_HEIGHT   = 1130;    //    1200;    //     800;     //     700;     //     500;
const float CAMERA_DISTANCE = 4.0f;
const int   TEXT_WIDTH      = 8;
const int   TEXT_HEIGHT     = 13;

// global variables
void *font = GLUT_BITMAP_8_BY_13;
int screenWidth;
int screenHeight;
bool mouseLeftDown;
bool mouseRightDown;
bool mouseMiddleDown;
float mouseX=0, mouseY=0;
float cameraAngleX=0;
float cameraAngleY=0;
float cameraDistance;
int drawMode;

GLuint texId_Sun;
GLuint texId_Jupiter;       //    texId_Earth;
GLuint texId_TC;            //    texId_Moon;

int imageWidth;
int imageHeight;

// sphere: min sector = 2, min stack = 2
Sphere sphere1(1.0f, 36, 18, false, 2);    // radius, sectors, stacks, non-smooth (flat) shading, Y-up
Sphere sphere2(1.0f, 36, 18, true,  2);    // radius, sectors, stacks, smooth(default), Y-up

#include"new_globals.h"
#include"globals.h"

#include "Axes.cpp"
#include "Reset.cpp"
#include "Buttons.cpp"

//#include"display_2D.cpp"
#include"display_cb.cpp"

#include"InitGlui.cpp"

 long double   SX[STEPS]={0};
 long double   SY[STEPS]={0};
 long double   SZ[STEPS]={0};

 long double   JX[STEPS]={0};
 long double   JY[STEPS]={0};
 long double   JZ[STEPS]={0};

 long double TM_X[STEPS]={0};
 long double TM_Y[STEPS]={0};
 long double TM_Z[STEPS]={0};

int point_num = STEPS;
int step = 0;
#include"display_burkhardt.cpp"

// 1. Define maximum size
const int MAX_POINTS = STEPS;
std::vector<glm::vec2> pointData(MAX_POINTS);

/*
// 2. Generate and bind the VBO
unsigned int vbo, vao;
glGenVertexArrays(1, &vao);
glGenBuffers(1, &vbo);
*/
/*
#include"setupOpenGLObjects"

glBindVertexArray(vao);
glBindBuffer(GL_ARRAY_BUFFER, vbo);

// Allocate space dynamically without data yet
glBufferData(GL_ARRAY_BUFFER, MAX_POINTS * sizeof(glm::vec2), nullptr, GL_DYNAMIC_DRAW);

// Configure attribute pointers (e.g., location=0 for vec2)
glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
glEnableVertexAttribArray(0);

glBindBuffer(GL_ARRAY_BUFFER, 0);
glBindVertexArray(0);
*/

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
// point_num = point_num-1;

 if(argc!=14){
  printf("usage ./a.out  secs   steps   TMx    TMy  TMz  TMvx  TMvy  TMvz  TMMx  TMMy  TMMz  TMm  TMq\n");
  printf("e.g.  ./a.out  86400  4500  1.496e11  0    0    0   29780   0     0     0     0     1    1 > Gravity__Sun-Test_Mass-Jupiter-Lorentz_prelude.out\n");
  exit(0);
 }

//setupOpenGLObjects();

 FILE *fptr;
 fptr = fopen("Gravity__Sun-Test_Mass-Jupiter-Lorentz_prelude.out", "w");

/*
std::ifstream data("Gravity__Sun-Test_Mass-Jupiter-Lorentz_prelude.out");
std::string line;
while (std::getline(data, line))
{
    std::stringstream lineStream(line);

    std::string cell;
    std::vector<float> parsedRow;
*/
/*
    while (std::getline(lineStream, cell, ','))
        parsedRow.push_back(std::stof(cell));

    if (parsedRow.size() == 3)
    {
        glBegin(GL_POINTS);
        glColor3f(0.0f, 1.0f, 1.0f);
        glVertex3fv(parsedRow.data());
        glEnd();
    }
}
*/

         int        i = 0;
 long double      ldi = i;
 long double       dt = atof(argv[1]);
 long double   dsteps = atof(argv[2]);
         int    steps = (int)dsteps;

 long double      TMx = atof(argv[3]);
 long double      TMy = atof(argv[4]);
 long double      TMz = atof(argv[5]);

 long double     TMvx = atof(argv[6]);
 long double     TMvy = atof(argv[7]);
 long double     TMvz = atof(argv[8]);

 long double     TMMx = atof(argv[9]);
 long double     TMMy = atof(argv[10]);
 long double     TMMz = atof(argv[11]);

 long double      TMm = atof(argv[12]);
 long double      TMq = atof(argv[13]);

 Body TM ={
  .x =  TMx,  .y =  TMy,  .z =  TMz,
  .vx = TMvx, .vy = TMvy, .vz = TMvz,
  .Mx = TMMx, .My = TMMy, .Mz = TMMz,
  .m =  TMm,
  .q =  TMq 
 };

/*
 long double  SX[steps]={0};
 long double  SY[steps]={0};
 long double  SZ[steps]={0};

 long double  JX[steps]={0};
 long double  JY[steps]={0};
 long double  JZ[steps]={0};

 long double TM_X[steps]={0};
 long double TM_Y[steps]={0};
 long double TM_Z[steps]={0};
*/

/*
 long double  Sx[steps]={0},  Sy[steps],  Sz[steps];
 long double  Jx[steps],  Jy[steps],  Jz[steps];
 long double TMx[steps], TMy[steps], TMz[steps];
*/

 long double total_mass = S.m + J.m;
 S.vx = -(J.vx * J.m) / S.m;
 S.vy = -(J.vy * J.m) / S.m;
 S.vz = -(J.vz * J.m) / S.m;

 long double S_ax=0,     S_ay=0,     S_az=0,     J_ax=0,     J_ay=0,     J_az=0,     TM_ax=0,     TM_ay=0,     TM_az=0;
 long double S_ax_new=0, S_ay_new=0, S_az_new=0, J_ax_new=0, J_ay_new=0, J_az_new=0, TM_ax_new=0, TM_ay_new=0, TM_az_new=0;

// Initial accelerations
/*
 Body1_Force_from_Body2(&S,  &J, &S_ax,  &S_ay,  &S_az);
 Body1_Force_from_Body2(&J,  &S, &J_ax,  &J_ay,  &J_az);
 Body1_Force_from_Body2(&TM, &J, &TM_ax, &TM_ay, &TM_az);
 Body1_Force_from_Body2(&TM, &S, &TM_ax, &TM_ay, &TM_az);
*/

 Body1_Force_from_Body2(&S,  &J,  &S_ax,  &S_ay,  &S_az);
 Body1_Force_from_Body2(&S,  &TM, &S_ax,  &S_ay,  &S_az);

 Body1_Force_from_Body2(&J,  &S,  &J_ax,  &J_ay,  &J_az);
 Body1_Force_from_Body2(&J,  &TM, &J_ax,  &J_ay,  &J_az);

 Body1_Force_from_Body2(&TM, &J,  &TM_ax, &TM_ay, &TM_az);
 Body1_Force_from_Body2(&TM, &S,  &TM_ax, &TM_ay, &TM_az);

 for (int i = 0; i < steps; i++) {
  ldi = i;
  long double t = ldi * dt; 

  SX[i]=S.x;
  SY[i]=S.y;
  SZ[i]=S.z;

  JX[i]=J.x;
  JY[i]=J.y;
  JZ[i]=J.z;

  TM_X[i]=TM.x;
  TM_Y[i]=TM.y;
  TM_Z[i]=TM.z;

 fprintf(fptr, "%Le %Le %Le %Le %Le %Le %Le %Le %Le %Le\n", t, S.x, S.y, S.z, J.x, J.y, J.z, TM.x, TM.y, TM.z);
//fprintf(stderr, "%Le %Le %Le %Le %Le %Le %Le %Le %Le %Le\n", t, S.x, S.y, S.z, J.x, J.y, J.z, TM.x, TM.y, TM.z);
//printf("%Le %Le %Le %Le %Le %Le %Le %Le %Le %Le\n", t, S.x, S.y, S.z, J.x, J.y, J.z, TM.x, TM.y, TM.z);

// 1. Position Verlet: Update positions to half-step / full-step
  S.x += S.vx * dt + 0.5 * S_ax * dt * dt; 
  S.y += S.vy * dt + 0.5 * S_ay * dt * dt; 
  S.z += S.vz * dt + 0.5 * S_az * dt * dt; 

  J.x += J.vx * dt + 0.5 * J_ax * dt * dt; 
  J.y += J.vy * dt + 0.5 * J_ay * dt * dt; 
  J.z += J.vz * dt + 0.5 * J_az * dt * dt; 

  TM.x += TM.vx * dt + 0.5 * TM_ax * dt * dt; 
  TM.y += TM.vy * dt + 0.5 * TM_ay * dt * dt; 
  TM.z += TM.vz * dt + 0.5 * TM_az * dt * dt; 

  S_ax_new=0, S_ay_new=0, S_az_new=0, J_ax_new=0, J_ay_new=0, J_az_new=0, TM_ax_new=0, TM_ay_new=0, TM_az_new=0;

// 2. Calculate new accelerations at the new positions
/*
  Body1_Force_from_Body2(&S,  &J, &S_ax_new,  &S_ay_new,  &S_az_new);
  Body1_Force_from_Body2(&J,  &S, &J_ax_new,  &J_ay_new,  &J_az_new);
  Body1_Force_from_Body2(&TM, &J, &TM_ax_new, &TM_ay_new, &TM_az_new);
  Body1_Force_from_Body2(&TM, &S, &TM_ax_new, &TM_ay_new, &TM_az_new);
*/

  Body1_Force_from_Body2(&S,  &J,  &S_ax_new, &S_ay_new,  &S_az_new);
  Body1_Force_from_Body2(&S,  &TM, &S_ax_new, &S_ay_new,  &S_az_new);

  Body1_Force_from_Body2(&J,  &S,  &J_ax_new, &J_ay_new,  &J_az_new);
  Body1_Force_from_Body2(&J,  &TM, &J_ax_new, &J_ay_new,  &J_az_new);

  Body1_Force_from_Body2(&TM, &J, &TM_ax_new, &TM_ay_new, &TM_az_new);
  Body1_Force_from_Body2(&TM, &S, &TM_ax_new, &TM_ay_new, &TM_az_new);

// 3. Velocity Verlet: Update velocities
  S.vx += 0.5 * (S_ax + S_ax_new) * dt;
  S.vy += 0.5 * (S_ay + S_ay_new) * dt;
  S.vz += 0.5 * (S_az + S_az_new) * dt;

  J.vx += 0.5 * (J_ax + J_ax_new) * dt;
  J.vy += 0.5 * (J_ay + J_ay_new) * dt;
  J.vz += 0.5 * (J_az + J_az_new) * dt;

  TM.vx += 0.5 * (TM_ax + TM_ax_new) * dt;
  TM.vy += 0.5 * (TM_ay + TM_ay_new) * dt;
  TM.vz += 0.5 * (TM_az + TM_az_new) * dt;

// Save new accelerations for next iteration
  S_ax = S_ax_new;
  S_ay = S_ay_new;
  S_az = S_az_new;

  J_ax = J_ax_new;
  J_ay = J_ay_new;
  J_az = J_az_new;

  TM_ax = TM_ax_new;
  TM_ay = TM_ay_new;
  TM_az = TM_az_new;
 }

    // init global vars
 initSharedMem();

    // init GLUT and GL
 initGLUT(argc, argv);
 initGL();

 InitGlui();

    // load BMP image
 texId_Sun     = loadTexture("2k_sun.bmp", true);
 texId_Jupiter = loadTexture("2k_jupiter.bmp", true);       //    texId_Earth;
//  texId_Moon    = loadTexture("moon1024.bmp", true);
 texId_TC      = loadTexture("checkerboard.bmp", true);
//  texId_Moon    = loadTexture("2k_moon.bmp", true);

 Reset();

 fclose(fptr);

/*
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
*/

points.reserve(STEPS);
Cpoint.reserve(STEPS);

points_Sun.reserve(STEPS);
Cpoint_Sun.reserve(STEPS);

points_TM.reserve(STEPS);
Cpoint_TM.reserve(STEPS);

points_Jupiter.reserve(STEPS);
Cpoint_Jupiter.reserve(STEPS);

for(int i=0;i<STEPS;i++){
//(points[i]).x = (float)SX[i];
//points[i].y = (float)SY[i];
  points.emplace_back(Point{(float)(SX[i]/1e8), (float) (SY[i]/1e8)});
  Cpoint.emplace_back(Point{(float)0, (float)0});

  points_Sun.emplace_back(Point{(float)(SX[i]/1e8), (float) (SY[i]/1e8)});
  Cpoint_Sun.emplace_back(Point{(float)0, (float)0});

  points_TM.emplace_back(Point{(float)(TM_X[i]/1e11), (float) (TM_Y[i]/1e11)});
  Cpoint_TM.emplace_back(Point{(float)0, (float)0});

  points_Jupiter.emplace_back(Point{(float)(JX[i]/1e11), (float) (JY[i]/1e11)});
  Cpoint_Jupiter.emplace_back(Point{(float)0, (float)0});
}

    // the last GLUT call (LOOP)
    // window will be shown and display callback is triggered by events
    // NOTE: this call never return main().
 
GLint viewport[4];
glGetIntegerv(GL_VIEWPORT, viewport);

int width = viewport[2];  // Width in pixels
int height = viewport[3];
fprintf(stderr, "width  %d\n", width);
fprintf(stderr, "height %d\n", height);

 glutMainLoop(); /* Start GLUT event-processing loop */

 return 0;
}

//#include"InitGlut.cpp"

int initGLUT(int argc, char **argv)
{
    // GLUT stuff for windowing
    // initialization openGL window.
    // it is called before any other GLUT routine
    glutInit(&argc, argv);

//  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);   // display mode
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowSize(screenWidth, screenHeight);  // window size

    glutInitWindowPosition(300, 0);               // window location

    // finally, create a window with openGL context
    // Window will not displayed until glutMainLoop() is called
    // it returns a unique ID
//  int handle = glutCreateWindow(argv[0]);     // param is the title of window
        handle = glutCreateWindow(argv[0]);     // param is the title of window
if( Debug ){
 fprintf(stderr, "handle %d\n", handle);
}

    // register GLUT callback functions
    glutDisplayFunc(displayCB);
    glutTimerFunc(33, timerCB, 33);             // redraw only every given millisec
    glutReshapeFunc(reshapeCB);
    glutKeyboardFunc(keyboardCB);
    glutMouseFunc(mouseCB);
    glutMotionFunc(mouseMotionCB);

    return handle;
}


///////////////////////////////////////////////////////////////////////////////
// initialize OpenGL
// disable unused features
///////////////////////////////////////////////////////////////////////////////
void initGL()
{
    glShadeModel(GL_SMOOTH);                    // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment

    // enable /disable features
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);

    // track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
    //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    //glEnable(GL_COLOR_MATERIAL);

    glClearColor(0, 0, 0, 0);                   // background color
    glClearStencil(0);                          // clear stencil buffer
    glClearDepth(1.0f);                         // 0 is near, 1 is far
    glDepthFunc(GL_LEQUAL);

    initLights();
}



///////////////////////////////////////////////////////////////////////////////
// write 2d text using GLUT
// The projection matrix must be set to orthogonal before call this function.
///////////////////////////////////////////////////////////////////////////////
void drawString(const char *str, int x, int y, float color[4], void *font)
{
    glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); // lighting and color mask
    glDisable(GL_LIGHTING);     // need to disable lighting for proper text color
    glDisable(GL_TEXTURE_2D);

    glColor4fv(color);          // set text color
    glRasterPos2i(x, y);        // place text position

    // loop all characters in the string
    while(*str)
    {
        glutBitmapCharacter(font, *str);
        ++str;
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopAttrib();
}



///////////////////////////////////////////////////////////////////////////////
// draw a string in 3D space
///////////////////////////////////////////////////////////////////////////////
void drawString3D(const char *str, float pos[3], float color[4], void *font)
{
    glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); // lighting and color mask
    glDisable(GL_LIGHTING);     // need to disable lighting for proper text color
    glDisable(GL_TEXTURE_2D);

    glColor4fv(color);          // set text color
    glRasterPos3fv(pos);        // place text position

    // loop all characters in the string
    while(*str)
    {
        glutBitmapCharacter(font, *str);
        ++str;
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopAttrib();
}



///////////////////////////////////////////////////////////////////////////////
// initialize global variables
///////////////////////////////////////////////////////////////////////////////
bool initSharedMem()
{
    screenWidth = SCREEN_WIDTH;
    screenHeight = SCREEN_HEIGHT;

    mouseLeftDown = mouseRightDown = mouseMiddleDown = false;
    mouseX = mouseY = 0;

    cameraAngleX = cameraAngleY = 0.0f;
    cameraDistance = CAMERA_DISTANCE;

    drawMode = 0; // 0:fill, 1: wireframe, 2:points

    // change up axis to +Y
    //sphere1.setUpAxis(2);
    //sphere2.setUpAxis(2);

    // debug
    sphere2.printSelf();

    return true;
}



///////////////////////////////////////////////////////////////////////////////
// clean up global vars
///////////////////////////////////////////////////////////////////////////////
void clearSharedMem()
{
}



///////////////////////////////////////////////////////////////////////////////
// initialize lights
///////////////////////////////////////////////////////////////////////////////
void initLights()
{
    // set up light colors (ambient, diffuse, specular)
    GLfloat lightKa[] = {.3f, .3f, .3f, 1.0f};  // ambient light
    GLfloat lightKd[] = {.7f, .7f, .7f, 1.0f};  // diffuse light
    GLfloat lightKs[] = {1, 1, 1, 1};           // specular light
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

    // position the light
    float lightPos[4] = {0, 0, 1, 0}; // directional light
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_LIGHT0);                        // MUST enable each light source after configuration
}

///////////////////////////////////////////////////////////////////////////////
// set camera position and lookat direction
///////////////////////////////////////////////////////////////////////////////
void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(posX, posY, posZ, targetX, targetY, targetZ, 0, 1, 0); // eye(x,y,z), focal(x,y,z), up(x,y,z)
}

///////////////////////////////////////////////////////////////////////////////
// load raw image as a texture
///////////////////////////////////////////////////////////////////////////////
GLuint loadTexture(const char* fileName, bool wrap)
{
    Image::Bmp bmp;
    if(!bmp.read(fileName))
        return 0;     // exit if failed load image

    // get bmp info
    int width = bmp.getWidth();
    int height = bmp.getHeight();
    const unsigned char* data = bmp.getDataRGB();
    GLenum type = GL_UNSIGNED_BYTE;    // only allow BMP with 8-bit per channel

    // We assume the image is 8-bit, 24-bit or 32-bit BMP
    GLenum format;
    int bpp = bmp.getBitCount();
    if(bpp == 8)
        format = GL_LUMINANCE;
    else if(bpp == 24)
        format = GL_RGB;
    else if(bpp == 32)
        format = GL_RGBA;
    else
        return 0;               // NOT supported, exit

    // gen texture ID
    GLuint texture;
    glGenTextures(1, &texture);

    // set active texture and configure it
    glBindTexture(GL_TEXTURE_2D, texture);

    // select modulate to mix texture with color for shading
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    // if wrap is true, the texture wraps over at the edges (repeat)
    //       ... false, the texture ends at the edges (clamp)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // copy texture data
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, data);
    //glGenerateMipmap(GL_TEXTURE_2D);

    // build our texture mipmaps
    switch(bpp)
    {
    case 8:
        gluBuild2DMipmaps(GL_TEXTURE_2D, 1, width, height, GL_LUMINANCE, type, data);
        break;
    case 24:
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, type, data);
        break;
    case 32:
        gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, type, data);
        break;
    }

    bmp.printSelf();
    return texture;
}

///////////////////////////////////////////////////////////////////////////////
// display info messages
///////////////////////////////////////////////////////////////////////////////
void showInfo()
{
    // backup current model-view matrix
    glPushMatrix();                     // save current modelview matrix
    glLoadIdentity();                   // reset modelview matrix

    // set to 2D orthogonal projection
    glMatrixMode(GL_PROJECTION);        // switch to projection matrix
    glPushMatrix();                     // save current projection matrix
    glLoadIdentity();                   // reset projection matrix
    //gluOrtho2D(0, screenWidth, 0, screenHeight); // set to orthogonal projection
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1); // set to orthogonal projection

    float color[4] = {1, 1, 1, 1};

    std::stringstream ss;
    ss << std::fixed << std::setprecision(3);

    ss << "Sphere Radius: " << sphere2.getRadius() << std::ends;
    drawString(ss.str().c_str(), 1, screenHeight-TEXT_HEIGHT, color, font);
    ss.str("");

    ss << "Sector Count: " << sphere2.getSectorCount() << std::ends;
    drawString(ss.str().c_str(), 1, screenHeight-(2*TEXT_HEIGHT), color, font);
    ss.str("");

    ss << "Stack Count: " << sphere2.getStackCount() << std::ends;
    drawString(ss.str().c_str(), 1, screenHeight-(3*TEXT_HEIGHT), color, font);
    ss.str("");

    ss << "Vertex Count: " << sphere2.getVertexCount() << std::ends;
    drawString(ss.str().c_str(), 1, screenHeight-(4*TEXT_HEIGHT), color, font);
    ss.str("");

    ss << "Index Count: " << sphere2.getIndexCount() << std::ends;
    drawString(ss.str().c_str(), 1, screenHeight-(5*TEXT_HEIGHT), color, font);
    ss.str("");

    // unset floating format
    ss << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);

    // restore projection matrix
    glPopMatrix();                   // restore to previous projection matrix

    // restore modelview matrix
    glMatrixMode(GL_MODELVIEW);      // switch to modelview matrix
    glPopMatrix();                   // restore to previous modelview matrix
}

///////////////////////////////////////////////////////////////////////////////
// set projection matrix as orthogonal
///////////////////////////////////////////////////////////////////////////////
void toOrtho()
{
    // set viewport to be the entire window
    glViewport(0, 0, (GLsizei)screenWidth, (GLsizei)screenHeight);

    // set orthographic viewing frustum
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//  glOrtho(0, screenWidth, 0, screenHeight, -1, 1);
    glOrtho(left-offset, right+offset, bottom-offset, top+offset,  zNear, zFar);

    // switch to modelview matrix in order to set scene
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

///////////////////////////////////////////////////////////////////////////////
// set the projection matrix as perspective
///////////////////////////////////////////////////////////////////////////////
void toPerspective()
{
    // set viewport to be the entire window
    glViewport(0, 0, (GLsizei)screenWidth, (GLsizei)screenHeight);

    // set perspective viewing frustum
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0f, (float)(screenWidth)/screenHeight, 1.0f, 1000.0f); // FOV, AspectRatio, NearClip, FarClip
//  gluPerspective(fovy, aspect, zNear, zFar); // FOV, AspectRatio, NearClip, FarClip

    // switch to modelview matrix in order to set scene
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//=============================================================================
// CALLBACKS
//=============================================================================

#include"displayCB.cpp"
/*
void displayCB()
{
    // clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // save the initial ModelView matrix before modifying ModelView matrix
    glPushMatrix();

    // tramsform modelview matrix
    glTranslatef(0, 0, -cameraDistance);

    // set material
    float ambient[]  = {0.5f, 0.5f, 0.5f, 1};
    float diffuse[]  = {0.7f, 0.7f, 0.7f, 1};
    float specular[] = {1.0f, 1.0f, 1.0f, 1};
    float shininess  = 128;
    glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    // line color
    float lineColor[] = {0.2f, 0.2f, 0.2f, 1};

    // draw left flat sphere with lines
    glPushMatrix();
    glTranslatef(-2.5f, 0, 0);
    glRotatef(cameraAngleX, 1, 0, 0);   // pitch
    glRotatef(cameraAngleY, 0, 1, 0);   // heading
    glBindTexture(GL_TEXTURE_2D, texId_Sun);
    sphere1.drawWithLines(lineColor);
    //sphere1.drawLines(lineColor);
    glPopMatrix();

    // draw centre smooth sphere with line
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse); // reset diffuse
    glPushMatrix();
    glRotatef(cameraAngleX, 1, 0, 0);
    glRotatef(cameraAngleY, 0, 1, 0);
    glBindTexture(GL_TEXTURE_2D, texId_Moon);
    sphere2.drawWithLines(lineColor);
    glPopMatrix();

    // draw right sphere with texture
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse); // reset diffuse
    glPushMatrix();
    glTranslatef(2.5f, 0, 0);
    glRotatef(cameraAngleX, 1, 0, 0);
    glRotatef(cameraAngleY, 0, 1, 0);
    glBindTexture(GL_TEXTURE_2D, texId_Earth);
    sphere2.draw();
    glPopMatrix();
*/

    /*
    // using GLU quadric object
    GLUquadricObj* obj = gluNewQuadric();
    gluQuadricDrawStyle(obj, GLU_FILL); // GLU_FILL, GLU_LINE, GLU_SILHOUETTE, GLU_POINT
    gluQuadricNormals(obj, GL_SMOOTH);
    gluQuadricTexture(obj, GL_TRUE);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    gluSphere(obj, 2.0, 50, 50); // radius, slice, stack
    */

/*
    glBindTexture(GL_TEXTURE_2D, 0);

    showInfo();     // print max range of glDrawRangeElements

    glPopMatrix();

    glutSwapBuffers();
}
*/

void reshapeCB(int w, int h)
{
    screenWidth = w;
    screenHeight = h;
    toPerspective();
//  Reset();
    std::cout << "window resized: " << w << " x " << h << std::endl;
}


void timerCB(int millisec)
{
    glutTimerFunc(millisec, timerCB, millisec);
    glutPostRedisplay();
}


void keyboardCB(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 27: // ESCAPE
        clearSharedMem();
        exit(0);
        break;

    case 'd': // switch rendering modes (fill -> wire -> point)
    case 'D':
        ++drawMode;
        drawMode %= 3;
        if(drawMode == 0)        // fill mode
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
        }
        else if(drawMode == 1)  // wireframe mode
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
        }
        else                    // point mode
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
        }
        break;

    case ' ':
        sphere1.reverseNormals();
        sphere2.reverseNormals();
        break;

    default:
        ;
    }
}

void mouseCB(int button, int state, int x, int y)
{
    mouseX = x;
    mouseY = y;

    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseLeftDown = true;
        }
        else if(state == GLUT_UP)
            mouseLeftDown = false;
    }

    else if(button == GLUT_RIGHT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseRightDown = true;
        }
        else if(state == GLUT_UP)
            mouseRightDown = false;
    }

    else if(button == GLUT_MIDDLE_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseMiddleDown = true;
        }
        else if(state == GLUT_UP)
            mouseMiddleDown = false;
    }
}

void mouseMotionCB(int x, int y)
{
    if(mouseLeftDown)
    {
        cameraAngleY += (x - mouseX);
        cameraAngleX += (y - mouseY);
        mouseX = x;
        mouseY = y;
    }
    if(mouseRightDown)
    {
        cameraDistance -= (y - mouseY) * 0.2f;
        mouseY = y;
    }
}
