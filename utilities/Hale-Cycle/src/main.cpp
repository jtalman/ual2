#include<GL/freeglut.h>

#include<cstdlib>
#include<iostream>
#include<sstream>
#include<iomanip>
#include"OrbitCamera.h"
#include"Matrices.h"
#include"Vectors.h"
#include"Quaternion.h"
#include"Timer.h"

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
HaleCycleState sun;

#include"./glui-2.36/src/include/GL/glui.h"
GLUI *Glui;
int handle;
GLfloat PX   = +5.5;
GLfloat PY   = +0;
GLfloat PZ   = +0;
GLint   HClk = +0;
int     year = +2026;
int     S    = +0;
GLint   SClk = +0;
int WhichGrid=1;
float cx = 0.0f;
float cy = 0.0f;

long double one=1;
long double two=2;
long double three=3;

 GLfloat Pr=sqrt(PX*PX+PY*PY);
 GLfloat Pt = atan(PY/PX);
 GLfloat PR = sqrt(PX*PX+PY*PY+PZ*PZ);

/*
GLfloat PC   = +sqrt(two);

//GLfloat Pr   = +sqrt(two);
//GLfloat Pt   = +atan(PY/PX);

GLfloat PR   = +sqrt(three);
*/

GLUI_Spinner *spinner_PX, *spinner_PY, *spinner_PZ, *spinner_Pr, *spinner_Pt, *spinner_PR;

/*
GLint viewport[4];
GLdouble mvmatrix[16], projmatrix[16];
GLdouble wx, wy, wz;
*/

#include"Get3DObjectCoordinates.cpp"

void GetOGLPos(int x, int y, GLdouble &ppX, GLdouble &ppY, GLdouble &ppZ){
 GLint viewport[4];
 GLdouble modelview[16];
 GLdouble projection[16];
 GLfloat winX, winY, winZ;

 glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
 glGetDoublev(GL_PROJECTION_MATRIX, projection);
 glGetIntegerv(GL_VIEWPORT, viewport);

 winX = (float)x;
 winY = (float)viewport[3]-(float)y;
 glReadPixels(x, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

 gluUnProject(winX, winY, winZ, modelview, projection, viewport, &ppX, &ppY, &ppZ);
}

void drawPointXYZ(GLfloat PX, GLfloat PY, GLfloat PZ, float size){
    glDepthFunc(GL_ALWAYS);     // to avoid visual artifacts with grid lines
    glDisable(GL_LIGHTING);

    glPointSize(size);
    glColor3f(1, 1, 1);

    glBegin(GL_POINTS);
    glVertex3f(PX, PY, PZ);
    glEnd();

    glPointSize(1);

    // restore default settings
    glEnable(GL_LIGHTING);
    glDepthFunc(GL_LEQUAL);
}

float zoom = 0.8f;//1.0f;
void mouseWheel(int wheel, int direction, int x, int y) {
    if (direction > 0) {
        zoom += 0.03f; // Zoom in
    } else {
        zoom -= 0.03f; // Zoom out
    }
    if (zoom < 0.1f) zoom = 0.1f; // Prevent inversion/disappearance
    glutPostRedisplay();
}

void mouseHover(int x, int y) {
    // x and y are the window-relative coordinates
    if(HClk) std::cout << "Mouse Hover Position: X=" << x << ", Y=" << y << std::endl;
}

/*
void mouseWheel(int wheel, int direction, int x, int y) {
    printf("Wheel %d scrolled %s at coords (%d, %d)\n", 
           wheel, (direction > 0) ? "UP" : "DOWN", x, y);
if (direction > 0) {
        zoom += 0.1f; // Zoom in
    } else {
        zoom -= 0.1f; // Zoom out
    }
    if (zoom < 0.1f) zoom = 0.1f; // Prevent inversion/disappearance
}
*/

//GLdouble &objX, &objY, &objZ;
double mposX;
double mposY;
double mposZ;
//void GetOGLPos(int x, int y, GLdouble &ppX, GLdouble &ppY, GLdouble &ppZ){

void GetOGLPS(int x, int y, double posX, double posY, double posZ) {
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;

    // Retrieve current matrices and viewport from OpenGL state machine
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    winX = (float)x;
    // Invert screen Y axis (OpenGL bottom-left vs Window top-left origin)
    winY = (float)viewport[3] - (float)y;
    
    // Read depth value at the selected pixel
    glReadPixels(x, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    // Map window coordinates to 3D object coordinates
    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
}

/*
void mouseClicks(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // (x, y) coordinates are relative to top-left corner of the window
        std::cout << "Left click at pixel: " << x << ", " << y << std::endl;

        GetOGLPS(x, y, &mposX, &mposY, &mposZ);
//      GetOGLPos(x, y, ppX, ppY, ppZ);
//      Get3DObjectCoordinates(x, y, &objX, &objY, &objZ);
        fprintf(stderr, "Object: (%d, %d, %d)\n", posX, posY, posZ);
    }
}
*/

double objx, objy, objz;

void mouseClicks2( int button, int state, int x, int y ){
 if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    double modelview[16], projection[16];
    int viewport[4];
    float z;

  glGetDoublev( GL_PROJECTION_MATRIX, projection );

  glGetDoublev( GL_MODELVIEW_MATRIX, modelview );

  glGetIntegerv( GL_VIEWPORT, viewport );

  glReadPixels( x, viewport[3]-y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z );

  gluUnProject( x, viewport[3]-y, z, modelview, projection, viewport, &objx, &objy, &objz );
  fprintf(stderr, "Object: (%lf, %lf, %lf)\n", objx, objy, objz);
 }
}

// GLUT CALLBACK functions
void displayCB();
void reshapeCB(int w, int h);
void timerCB(int millisec);
void idleCB();
void keyboardCB(unsigned char key, int x, int y);
void keyboardUpCB(unsigned char key, int x, int y);
void mouseCB(int button, int stat, int x, int y);
void mouseMotionCB(int x, int y);
void specialCB(int key, int x, int y);
void specialUpCB(int key, int x, int y);

// CALLBACK function when exit() called ///////////////////////////////////////
void exitCB();

void initGL();
int  initGLUT(int argc, char **argv);
bool initSharedMem();
void clearSharedMem();
void initLights();
void drawString(const char *str, int x, int y, float color[4], void *font);
void drawString3D(const char *str, float pos[3], float color[4], void *font);
void showHale2();
void showInfo();
void showFPS();
void toOrtho();
void toPerspective();
void drawAxis(float size=2.5f);
void drawGridXZ(float size=10.0f, float step=1.0f);
void drawGridXY(float size=10.0f, float step=1.0f);
void drawPoint(float size);
void drawModel();
void resetCamera();

// constants
const int   SCREEN_WIDTH    = 1100;//1700;//600;
const int   SCREEN_HEIGHT   = 1100;//600;
const float CAMERA_DISTANCE = 10.0f;
const int   TEXT_WIDTH      = 8;
const int   TEXT_HEIGHT     = 13;
const float DEG2RAD         = 3.141593f / 180;
const float MOVE_SPEED      = 3.0f;             // units per sec
const float MOVE_ACCEL      = 10.0f;            // units per squared sec

float CPX=5;//0;
float CPY=4;//0;
float CPZ=8;//CAMERA_DISTANCE;
float CTX=0;
float CTY=0;
float CTZ=0;
float CUX=0;
float CUY=1;
float CUZ=0;

// global variables
void *font = GLUT_BITMAP_8_BY_13;
int screenWidth;
int screenHeight;
bool mouseLeftDown;
bool mouseRightDown;
bool plusKeyDown;
bool minusKeyDown;
bool leftKeyDown;
bool rightKeyDown;
bool upKeyDown;
bool downKeyDown;
float mouseX, mouseY;
float mouseDownX, mouseDownY;
int drawMode;

OrbitCamera camera;
Vector3 cameraAngle;
Quaternion cameraQuat;
void resetCameraPTU(float, float, float, float, float, float, float, float, float);
#include"Reset.cpp"
#include"Grid.cpp"
#include"Buttons.cpp"

const int STEPS = 4500;
const long double dt = 86400;

 long double SX[STEPS]={0};
 long double SY[STEPS]={0};
 long double SZ[STEPS]={0};

 long double JX[STEPS]={0};
 long double JY[STEPS]={0};
 long double JZ[STEPS]={0};

 long double PPX[STEPS]={0};
 long double PPY[STEPS]={0};
 long double PPZ[STEPS]={0};

Timer timer;
float playTime;     // sec

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv){

    // init global vars
    initSharedMem();

    // register exit callback
    atexit(exitCB);

    // init GLUT and GL
    initGLUT(argc, argv);
    initGL();

int i=0;
long double t; 
FILE*fptr = fopen("Gravity__Sun-Test_Mass-Jupiter-Lorentz_prelude.out", "r");
//long double SX[i++]=0, SY[i++]=0, SZ[i++]=0, JX[i++]=0, JY[i++]=0, JZ[i++]=0, PX[i++]=0, PY[i++]=0, PZ[i++]=0;
//while (fscanf(fptr, "%Le %Le %Le %Le %Le %Le %Le %Le %Le\n", SX, SY, SZ, JX, JY, JZ, PX, PY, PZ) == 9) {
//while (fscanf(fptr, "%Le %Le %Le %Le %Le %Le %Le %Le %Le\n", &SX, &SY, &SZ, &JX, &JY, &JZ, &PX, &PY, &PZ) == 9) {

//while (fscanf(fptr, "%Le %Le %Le %Le %Le %Le %Le %Le %Le\n", SX+i, SY+i, SZ+i, JX+i, JY+i, JZ+i, PX+i, PY+i, PZ+i) == 9) {
  while (fscanf(fptr, "%Le %Le %Le %Le %Le %Le %Le %Le %Le %Le\n", &t, &SX[i], &SY[i], &SZ[i], &JX[i], &JY[i], &JZ[i], &PPX[i], &PPY[i], &PPZ[i]) == 10) {

//while (fscanf(fptr, "%+Le %+Le %+Le %+Le %+Le %+Le %+Le %+Le %+Le\n", &SX, &SY, &SZ, &JX, &JY, &JZ, &PX, &PY, &PZ) == 9) {
//while (fscanf(fptr, "%+.4Le %+.4Le %+.4Le %+.4Le %+.4Le %+.4Le %+.4Le %+.4Le %+.4Le\n", &SX, &SY, &SZ, &JX, &JY, &JZ, &PX, &PY, &PZ) == 9) {
//while (fscanf(fptr, "% Le % Le % Le % Le % Le % Le % Le % Le % Le\n", &SX, &SY, &SZ, &JX, &JY, &JZ, &PX, &PY, &PZ) == 9) {
    fprintf(stderr, "%+Le %+Le %+Le %+Le %+Le %+Le %+Le %+Le %+Le %Le\n", t, SX[i], SY[i], SZ[i], JX[i], JY[i], JZ[i], PPX[i], PPY[i], PPZ[i]);
//  printf("%Le %Le %Le %Le %Le %Le %Le %Le %Le\n", SY, SZ, JX, JY, JZ, PX, PY, PZ);
i++;
}

/*
    int start_year = 1755;
    int num_cycles = 24; // Up to recent cycles
    
    printf("Cycle | Years       | Type / Hale Group\n");
    printf("-----------------------------------------\n");
    
    for (int i = 1; i <= num_cycles; i++) {
        int cyc_start = start_year + (i - 1) * 11;
        int cyc_end = cyc_start + 11;
        int hale_group = (i - 1) / 2 + 1;
        
        printf("%5d | %d - %d | Hale Group %d (%s)\n", 
               i, 
               cyc_start, 
               cyc_end, 
               hale_group,
               (i % 2 != 0) ? "Odd/Asc" : "Even/Desc");
    }
*/

//         GLUI_Master.create_glui(char *name, int flags = 0, int x = -1, int y = -1)
int flags = 0;
int x = -100;
int y = 0;
    Glui = GLUI_Master.create_glui("Camera Controls", flags, x, y);

    GLUI_Panel *panel1, *panel2;
    panel1 = Glui->add_panel("Coords");

                  spinner_PX = Glui->add_spinner_to_panel(panel1, "Point X:", GLUI_SPINNER_FLOAT, &PX, 0, (GLUI_Update_CB) Buttons );
    spinner_PX->set_float_limits( -100.f, 100.f );
    spinner_PX->enable();

                  spinner_PY = Glui->add_spinner_to_panel(panel1, "Point Y:", GLUI_SPINNER_FLOAT, &PY, 1, (GLUI_Update_CB) Buttons );
    spinner_PY->set_float_limits( -100.f, 100.f );
    spinner_PY->enable();

                  spinner_PZ = Glui->add_spinner_to_panel(panel1, "Point Z:", GLUI_SPINNER_FLOAT, &PZ, 2, (GLUI_Update_CB) Buttons );
    spinner_PZ->set_float_limits( -100.f, 100.f );
    spinner_PZ->enable();

                  spinner_Pr = Glui->add_spinner_to_panel(panel1, "Point r:", GLUI_SPINNER_FLOAT, &Pr, 3, (GLUI_Update_CB) Buttons );
    spinner_Pr->set_float_limits(0, +100);
    spinner_Pr->disable();

                  spinner_Pt = Glui->add_spinner_to_panel(panel1, "Point theta:", GLUI_SPINNER_FLOAT, &Pt, 4, (GLUI_Update_CB) Buttons );
    spinner_Pt->set_float_limits(-10, +10);
    spinner_Pt->disable();

                  spinner_PR = Glui->add_spinner_to_panel(panel1, "Point R:", GLUI_SPINNER_FLOAT, &PR, 101, (GLUI_Update_CB) Buttons );
    spinner_PR->set_float_limits(0, 200);
    spinner_PR->disable();

    GLUI_RadioGroup *group;
    panel2 = Glui->add_panel("Grid");
        group = Glui->add_radiogroup_to_panel( panel2, &WhichGrid, 5, (GLUI_Update_CB) Buttons);
            Glui->add_radiobutton_to_group( group, "None" );
            Glui->add_radiobutton_to_group( group, "Rectangular" );
            Glui->add_radiobutton_to_group( group, "Cylindrical" );

    GLUI_Spinner *spinner_step;
    spinner_step  = new GLUI_Spinner( Glui, "Year:", &year, 6, (GLUI_Update_CB) Buttons );
    spinner_step->set_int_limits( 1750, 2030 );

  GLUI_Checkbox*CB  =new GLUI_Checkbox(Glui, "SHOW HOVER", &HClk, 7, (GLUI_Update_CB) Buttons );

	                 Glui->add_checkbox( "Hale 1755 - present", &SClk, 8, (GLUI_Update_CB) Buttons );

    GLUI_Button*button_R=new GLUI_Button(Glui, "RESET", 9, (GLUI_Update_CB) Buttons );

    Glui->set_main_gfx_window(glutGetWindow());

    glutMainLoop(); /* Start GLUT event-processing loop */

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// draw a grid on XZ-plane
///////////////////////////////////////////////////////////////////////////////
void drawGridXZ(float size, float step)
{
    // disable lighting
    glDisable(GL_LIGHTING);

    glBegin(GL_LINES);

    glColor4f(0.5f, 0.5f, 0.5f, 0.5f);
    for(float i=step; i <= size; i+= step)
    {
        glVertex3f(-size, 0,  i);   // lines parallel to X-axis
        glVertex3f( size, 0,  i);
        glVertex3f(-size, 0, -i);   // lines parallel to X-axis
        glVertex3f( size, 0, -i);

        glVertex3f( i, 0, -size);   // lines parallel to Z-axis
        glVertex3f( i, 0,  size);
        glVertex3f(-i, 0, -size);   // lines parallel to Z-axis
        glVertex3f(-i, 0,  size);
    }

    // x-axis
    glColor3f(1, 0, 0);
    glVertex3f(-size, 0, 0);
    glVertex3f( size, 0, 0);

    // z-axis
    glColor3f(0,0,1);
    glVertex3f(0, 0, -size);
    glVertex3f(0, 0,  size);

    glEnd();

    // enable lighting back
    glEnable(GL_LIGHTING);
}

void drawGrid_r_theta(float cx, float cy, float r, int num_segments){
    glDisable(GL_LIGHTING);

//  glColor4f(0.5f, 0.5f, 0.5f, 0.5f);
    glColor4f(0.85f, 0.85f, 0.85f, 1);

    float radii[] = {1, 2, 3, 4, 5, 6};
    for (float r : radii) {
//  glBegin(GL_LINE_LOOP);
    glBegin(GL_LINE_STRIP);
//   float radii[] = {0.2f, 0.4f, 0.6f, 0.8f};
//   float radii[] = {1.2f, 2.4f, 3.6f, 4.8f};
//   float radii[] = {2, 4, 6, 8};
      for (int i = 0; i <= num_segments; i++) {
       float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
       float x = r * cosf(theta);
       float y = r * sinf(theta);
       glVertex2f(x + cx, y + cy);
      }
     glEnd();
    }

    glEnable(GL_LIGHTING);
}

///////////////////////////////////////////////////////////////////////////////
// draw a grid on XY-plane
///////////////////////////////////////////////////////////////////////////////
void drawGridXY(float size, float step)
{
    // disable lighting
    glDisable(GL_LIGHTING);

    // 20x20 grid
    glBegin(GL_LINES);

    glColor4f(0.5f, 0.5f, 0.5f, 0.5f);
    for(float i=step; i <= size; i+= step)
    {
        glVertex3f(-size,  i, 0);   // lines parallel to X-axis
        glVertex3f( size,  i, 0);
        glVertex3f(-size, -i, 0);   // lines parallel to X-axis
        glVertex3f( size, -i, 0);

        glVertex3f( i, -size, 0);   // lines parallel to Y-axis
        glVertex3f( i,  size, 0);
        glVertex3f(-i, -size, 0);   // lines parallel to Y-axis
        glVertex3f(-i,  size, 0);
    }

    // x-axis
    glColor3f(1, 0, 0);
    glVertex3f(-size, 0, 0);
    glVertex3f( size, 0, 0);

    // y-axis
    glColor3f(0, 1, 0);
    glVertex3f(0, -size, 0);
    glVertex3f(0,  size, 0);

    glEnd();

    // enable lighting back
    glEnable(GL_LIGHTING);
}

///////////////////////////////////////////////////////////////////////////////
// draw the local axis of an object
///////////////////////////////////////////////////////////////////////////////
void drawAxis(float size)
{
    glDepthFunc(GL_ALWAYS);     // to avoid visual artifacts with grid lines
    glDisable(GL_LIGHTING);

    // draw axis
    glLineWidth(5);
    glBegin(GL_LINES);
        glColor3f(1, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(size, 0, 0);
        glColor3f(0, 1, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, size, 0);
        glColor3f(0, 0, 1);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, size);
    glEnd();
    glLineWidth(1);

    // draw arrows(actually big square dots)
    glPointSize(10);
    glBegin(GL_POINTS);
        glColor3f(1, 0, 0);
        glVertex3f(size, 0, 0);
        glColor3f(0, 1, 0);
        glVertex3f(0, size, 0);
        glColor3f(0, 0, 1);
        glVertex3f(0, 0, size);
    glEnd();
    glPointSize(1);

    // restore default settings
    glEnable(GL_LIGHTING);
    glDepthFunc(GL_LEQUAL);
}



///////////////////////////////////////////////////////////////////////////////
// draw a point with a given size
///////////////////////////////////////////////////////////////////////////////
void drawPoint(float size)
{
    glDepthFunc(GL_ALWAYS);     // to avoid visual artifacts with grid lines
    glDisable(GL_LIGHTING);

    glPointSize(size);
    glColor3f(1, 1, 0);

    Vector3 cameraTarget = camera.getTarget();
    glBegin(GL_POINTS);
    glVertex3f(cameraTarget.x, cameraTarget.y, cameraTarget.z);
    glEnd();

    glPointSize(1);

    // restore default settings
    glEnable(GL_LIGHTING);
    glDepthFunc(GL_LEQUAL);
}



///////////////////////////////////////////////////////////////////////////////
// draw a model (tetrahedron)
///////////////////////////////////////////////////////////////////////////////
void drawModel()
{
    glColor3f(1, 1, 1);
    glBegin(GL_TRIANGLES);
    glNormal3f(0.6667f, 0.6667f, 0.3334f);
    glVertex3f(1, 0, 0);
    glVertex3f(0, 1, 0);
    glVertex3f(0, 0, 2);

    glNormal3f(-0.6667f, 0.6667f, 0.3334f);
    glVertex3f(-1, 0, 0);
    glVertex3f(0, 0, 2);
    glVertex3f(0, 1, 0);

    glNormal3f(0, 0, -1);
    glVertex3f(1, 0, 0);
    glVertex3f(0, 0, 2);
    glVertex3f(-1, 0, 0);

    glNormal3f(0, -1, 0);
    glVertex3f(1, 0, 0);
    glVertex3f(-1, 0, 0);
    glVertex3f(0, 1, 0);
    glEnd();
}



///////////////////////////////////////////////////////////////////////////////
// initialize GLUT for windowing
///////////////////////////////////////////////////////////////////////////////
int initGLUT(int argc, char **argv)
{
    // GLUT stuff for windowing
    // initialization openGL window.
    // it is called before any other GLUT routine
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);   // display mode

    glutInitWindowSize(screenWidth, screenHeight);  // window size

    glutInitWindowPosition(400, 100);           // window location
//  glutInitWindowPosition(100, 100);           // window location

    // finally, create a window with openGL context
    // Window will not displayed until glutMainLoop() is called
    // it returns a unique ID
//  int handle = glutCreateWindow(argv[0]);     // param is the title of window
        handle = glutCreateWindow(argv[0]);     // param is the title of window

    // register GLUT callback functions
    glutDisplayFunc(displayCB);
    glutTimerFunc(33, timerCB, 33);             // redraw only every given millisec
    //glutIdleFunc(idleCB);                       // redraw whenever system is idle
    glutReshapeFunc(reshapeCB);
    glutKeyboardFunc(keyboardCB);
    glutKeyboardUpFunc(keyboardUpCB);

    glutMouseFunc(mouseCB);
//  glutMouseFunc(mouseClicks);
//  glutMouseFunc(mouseClicks2);

    glutMotionFunc(mouseMotionCB);
    glutSpecialFunc(specialCB);
    glutSpecialUpFunc(specialUpCB);
    glutMouseWheelFunc(mouseWheel);
    glutPassiveMotionFunc(mouseHover); //

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
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);

    // enable /disable features
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);

     // track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

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

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopAttrib();
}



///////////////////////////////////////////////////////////////////////////////
// initialize global variables
///////////////////////////////////////////////////////////////////////////////
bool initSharedMem()
{
    // init timer
    timer.start();
    playTime = 0;

    screenWidth = SCREEN_WIDTH;
    screenHeight = SCREEN_HEIGHT;

    mouseLeftDown = mouseRightDown = false;
    mouseDownX = mouseDownY = 0;
    mouseX = mouseY = 0;

    // for +/- keys
    plusKeyDown = minusKeyDown = false;

    // for arrow keys
    leftKeyDown = rightKeyDown = upKeyDown = downKeyDown = false;

    // init camera
    resetCamera();

    drawMode = 0; // 0:fill, 1: wireframe, 2:points

    return true;
}



///////////////////////////////////////////////////////////////////////////////
// clean up shared memory
///////////////////////////////////////////////////////////////////////////////
void clearSharedMem()
{
}

///////////////////////////////////////////////////////////////////////////////
// reset camera to default
///////////////////////////////////////////////////////////////////////////////
void resetCamera()
{
//  Vector3 cameraPosition(0, 0, CAMERA_DISTANCE);
    Vector3 cameraPosition(CPX, CPY, CPZ);

//  Vector3 cameraTarget(0, 0, 0);
    Vector3 cameraTarget(CTX, CTY, CTZ);

//  Vector3 cameraUp(0, 1, 0);
    Vector3 cameraUp(CUX, CUY, CUZ);

    camera.lookAt(cameraPosition, cameraTarget);
    //camera.lookAt(cameraPosition, cameraTarget, cameraUp);
    std::cout << camera.getMatrix() << std::endl;
    std::cout << "distance: " << camera.getDistance() << std::endl;
}

void resetCameraPTU(float CPX, float CPY, float CPZ, float CTX, float CTY, float CTZ, float CUX, float CUY, float CUZ){
 fprintf(stderr, "enter void resetCameraPTU(float CPX, float CPY, float CPZ, float CTX, float CTY, float CTZ, float CUX, float CUY, float CUZ)\n");
 fprintf(stderr, "CPX, CPY, CPZ: %f %f %f\n", CPX, CPY, CPZ);
 Vector3 cameraPosition(CPX, CPY, CPZ);
 Vector3 cameraTarget(CTX, CTY, CTZ);
    Vector3 cameraUp(CUX, CUY, CUZ);

 camera.lookAt(cameraPosition, cameraTarget);
 //camera.lookAt(cameraPosition, cameraTarget, cameraUp);
 
 std::cout << camera.getMatrix() << std::endl;
 std::cout << "distance: " << camera.getDistance() << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
// initialize lights
///////////////////////////////////////////////////////////////////////////////
void initLights()
{
    // set up light colors (ambient, diffuse, specular)
    GLfloat lightKa[] = {.2f, .2f, .2f, 1.0f};  // ambient light
    GLfloat lightKd[] = {.7f, .7f, .7f, 1.0f};  // diffuse light
    GLfloat lightKs[] = {1, 1, 1, 1};           // specular light
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

    // position the light
    float lightPos[4] = {0, 1, 1, 0}; // directional light
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_LIGHT0);                        // MUST enable each light source after configuration
}

void showHale1(){

    int start_year = 1755;
    int num_cycles = 24; // Up to recent cycles
    
    printf("Cycle | Years       | Type / Hale Group\n");
    printf("-----------------------------------------\n");
    
    for (int i = 1; i <= num_cycles; i++) {
        int cyc_start = start_year + (i - 1) * 11;
        int cyc_end = cyc_start + 11;
        int hale_group = (i - 1) / 2 + 1;
        
        printf("%5d | %d - %d | Hale Group %d (%s)\n", 
               i, 
               cyc_start, 
               cyc_end, 
               hale_group,
               (i % 2 != 0) ? "Odd/Asc" : "Even/Desc");
    }
}

void showHale2(){
// fprintf(stderr, "enter void showHale2(): SClk:: %d\n", SClk);
    glPushMatrix();                     // save current modelview matrix
    glLoadIdentity();                   // reset modelview matrix

    // set to 2D orthogonal projection
    glMatrixMode(GL_PROJECTION);        // switch to projection matrix
    glPushMatrix();                     // save current projection matrix
    glLoadIdentity();                   // reset projection matrix
    gluOrtho2D(0, screenWidth, 0, screenHeight); // set to orthogonal projection

    float color[4] = {1, 1, 1, 1};

    std::stringstream ss;
    ss << std::fixed << std::setprecision(1);

    int start_year = 1755;
    int num_cycles = 24; // Up to recent cycles
    
/*
    printf("Cycle | Years       | Type / Hale Group\n");
    printf("-----------------------------------------\n");
*/
    
    for (int i = 1; i <= num_cycles; i++) {
        int cyc_start = start_year + (i - 1) * 11;
        int cyc_end = cyc_start + 11;
        int hale_group = (i - 1) / 2 + 1;
        
    ss << "     Cycle: " << i << " " << cyc_start << " " << cyc_end << " " << hale_group << " " << (i % 2 != 0) ? "Odd/Asc" : "Even/Desc";
    drawString(ss.str().c_str(), 1, screenHeight - i*TEXT_HEIGHT, color, font);
    ss.str("");
    }

    update_hale_state(&sun, year);
    ss.str("");
    ss << "     Year: " << sun.year << " " << "Polarity: " << sun.magnetic_polarity << " " << "Strength: " << sun.field_strength;
    drawString(ss.str().c_str(), 1, screenHeight - 26*TEXT_HEIGHT, color, font);
//  fprintf(stderr, "Year: %.1f | Polarity: %d | Field Strength: %.2f\n", sun.year, sun.magnetic_polarity, sun.field_strength);

    ss << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);

    glPopMatrix();                   // restore to previous projection matrix

    // restore modelview matrix
    glMatrixMode(GL_MODELVIEW);      // switch to modelview matrix
    glPopMatrix();                   // restore to previous modelview matrix
// fprintf(stderr, "leave void showHale2()\n");
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
    gluOrtho2D(0, screenWidth, 0, screenHeight); // set to orthogonal projection

    float color[4] = {1, 1, 1, 1};

    std::stringstream ss;
    ss << std::fixed << std::setprecision(1);

    // print camera info
    ss << "     Angle: " << camera.getAngle();
    drawString(ss.str().c_str(), 1, screenHeight - TEXT_HEIGHT, color, font);

    ss.str("");
    ss << "Quaternion: " << camera.getQuaternion();
    drawString(ss.str().c_str(), 1, screenHeight - 2*TEXT_HEIGHT, color, font);

    ss.str("");
    ss << "  Position: " << camera.getPosition();
    drawString(ss.str().c_str(), 1, screenHeight - 3*TEXT_HEIGHT, color, font);

    ss.str("");
    ss << "    Target: " << camera.getTarget();
    drawString(ss.str().c_str(), 1, screenHeight - 4*TEXT_HEIGHT, color, font);

    ss.str("");
    ss << "   Left Axis: " << camera.getLeftAxis();
    drawString(ss.str().c_str(), 1, screenHeight - 5*TEXT_HEIGHT, color, font);

    ss.str("");
    ss << "     Up Axis: " << camera.getUpAxis();
    drawString(ss.str().c_str(), 1, screenHeight - 6*TEXT_HEIGHT, color, font);

    ss.str("");
    ss << "Forward Axis: " << camera.getForwardAxis();
    drawString(ss.str().c_str(), 1, screenHeight - 7*TEXT_HEIGHT, color, font);

    ss.str("");
    ss << "Distance: " << camera.getDistance();
    drawString(ss.str().c_str(), 1, screenHeight - 8*TEXT_HEIGHT, color, font);

    ss.str("");
    ss << "zoom: " << zoom;
    drawString(ss.str().c_str(), 1, screenHeight - 9*TEXT_HEIGHT, color, font);

    ss.str("");
    ss << "Probe Position: " << "(" << PX << ", " << PY << ", " << PZ << ")";
    drawString(ss.str().c_str(), 1, screenHeight - 10*TEXT_HEIGHT, color, font);

    ss.str("");
    ss << "Sun Position: " << "(" << SX[S] << ", " << SY[S] << ", " << SZ[S] << ")";
    drawString(ss.str().c_str(), 1, screenHeight - 11*TEXT_HEIGHT, color, font);

    ss.str("");
    ss << "Use arrow keys to shift camera.";
    drawString(ss.str().c_str(), 1, TEXT_HEIGHT + 1, color, font);

    ss.str("");
    ss << "Use +/- keys to move forward/backward.";
    drawString(ss.str().c_str(), 1, 1, color, font);

    // unset floating format
    ss << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);

    // draw focal guide lines ===================
    /*
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glLineWidth(0.5f);
    glColor3f(1,1,0);
    glBegin(GL_LINES);
    glVertex2f(0,           screenHeight*0.5f);
    glVertex2f(screenWidth, screenHeight*0.5f);
    glVertex2f(screenWidth*0.5f, 0);
    glVertex2f(screenWidth*0.5f, screenHeight);
    glEnd();
    glLineWidth(1);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
*/

    // restore projection matrix
    glPopMatrix();                   // restore to previous projection matrix

    // restore modelview matrix
    glMatrixMode(GL_MODELVIEW);      // switch to modelview matrix
    glPopMatrix();                   // restore to previous modelview matrix
}



///////////////////////////////////////////////////////////////////////////////
// display frame rates
///////////////////////////////////////////////////////////////////////////////
void showFPS()
{
    static Timer timer;
    static int count = 0;
    static std::string fps = "0.0 FPS";
    double elapsedTime = 0.0;;

    ++count;

    // backup current model-view matrix
    glPushMatrix();                     // save current modelview matrix
    glLoadIdentity();                   // reset modelview matrix

    // set to 2D orthogonal projection
    glMatrixMode(GL_PROJECTION);        // switch to projection matrix
    glPushMatrix();                     // save current projection matrix
    glLoadIdentity();                   // reset projection matrix
    gluOrtho2D(0, screenWidth, 0, screenHeight); // set to orthogonal projection

    float color[4] = {1, 1, 0, 1};

    // update fps every second
    elapsedTime = timer.getElapsedTime();
    if(elapsedTime >= 1.0)
    {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1);
        ss << (count / elapsedTime) << " FPS" << std::ends; // update fps string
        ss << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
        fps = ss.str();
        count = 0;                      // reset counter
        timer.start();                  // restart timer
    }
    int textWidth = (int)fps.size() * TEXT_WIDTH;
    drawString(fps.c_str(), screenWidth-textWidth, screenHeight-TEXT_HEIGHT, color, font);

    // restore projection matrix
    glPopMatrix();                      // restore to previous projection matrix

    // restore modelview matrix
    glMatrixMode(GL_MODELVIEW);         // switch to modelview matrix
    glPopMatrix();                      // restore to previous modelview matrix
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
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1);

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
    gluPerspective(60.0f, (float)(screenWidth)/screenHeight, 1.0f, 1000.0f); // FOV, AspectRatio, NearClip, FarClip

    // switch to modelview matrix in order to set scene
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}















//=============================================================================
// CALLBACKS
//=============================================================================

void displayCB()
{
    // update times
    float elapsedTime = timer.getElapsedTime();
    float frameTime =  elapsedTime - playTime;
    playTime = elapsedTime;

    // update camera
    camera.update(frameTime);

    // clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // save the initial ModelView matrix before modifying ModelView matrix
    glPushMatrix();

    // transform camera (view matrix)
    glLoadMatrixf(camera.getMatrix().get());

    // draw the camera rotation pivot (target point)
//  drawPoint(10);

    // draw models

    glScalef(zoom, zoom, 1.0f);
    drawPointXYZ(PX, PY, PZ, 10);

    drawAxis(4.5);                         // for origin (0,0,0)
//  drawGridXZ(20);                     // draw XZ-grid at origin (world space)

    if(WhichGrid){
     if(WhichGrid==1){
      drawGridXY(5, 0.5);                     // draw XY-grid at origin (world space)
//    drawGridXY(5);                     // draw XY-grid at origin (world space)
     }
     if(WhichGrid==2){
//    drawGrid_r_theta(float cx, float cy, float r, int num_segments);
//    drawGrid_r_theta(cx, cy, 0, 100);
      drawGrid_r_theta(cx, cy, 0, 40);
     }
    }

//  drawModel();

    // draw info messages
    if(SClk==0)showInfo();
    if(SClk==1)showHale2();
//  showHale2();

//  showFPS();

    glPopMatrix();

    glutSwapBuffers();
}



void reshapeCB(int w, int h)
{
    screenWidth = w;
    screenHeight = h;
    toPerspective();
}


void timerCB(int millisec)
{
    glutTimerFunc(millisec, timerCB, millisec);
    glutPostRedisplay();
}


void idleCB()
{
    glutPostRedisplay();
}


void keyboardCB(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 27: // ESCAPE
        exit(0);
        break;

    case ' ':
        break;

    case 'r':
    case 'R':
        resetCamera();
        break;

    case 'd': // switch rendering modes (fill -> wire -> point)
    case 'D':
        drawMode = ++drawMode % 3;
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

    case '+':
    case '=':
        //camera.moveForward( 1.0f);
        //camera.moveForward( 1.0f, 0.5f, Gil::EASE_OUT);
        if(!plusKeyDown)
        {
            plusKeyDown = true;
            camera.startForward(MOVE_SPEED, MOVE_ACCEL);
        }
        break;

    case '-':
    case '_':
        //camera.moveForward(-1.0f);
        //camera.moveForward(-1.0f, 0.5f, Gil::EASE_OUT);
        if(!minusKeyDown)
        {
            minusKeyDown = true;
            camera.startForward(-MOVE_SPEED, MOVE_ACCEL);
        }
        break;

    default:
        ;
    }
}


void keyboardUpCB(unsigned char key, int x, int y)
{
    switch(key)
    {
    case '+':
    case '=':
        plusKeyDown = false;
        camera.stopForward();
        break;
    case '-':
    case '_':
        minusKeyDown = false;
        camera.stopForward();
        break;

    default:
        ;
    }
}

void gl_select(int x, int y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);


    printf("%f %f %f\n", posX, posZ, posY);

    glutPostRedisplay();
}

void mouseCB(int button, int state, int x, int y)
{
/*
 if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    double modelview[16], projection[16];
    int viewport[4];
    float z;

  glGetDoublev( GL_PROJECTION_MATRIX, projection );

  glGetDoublev( GL_MODELVIEW_MATRIX, modelview );

  glGetIntegerv( GL_VIEWPORT, viewport );

  glReadPixels( x, viewport[3]-y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z );

  gluUnProject( x, viewport[3]-y, z, modelview, projection, viewport, &objx, &objy, &objz );
  fprintf(stderr, "Object: (%lf, %lf, %lf)\n", objx, objy, objz);

  return;
 }
*/
    mouseX = x;
    mouseY = y;

    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            cameraAngle = camera.getAngle(); // get current camera angle
            cameraQuat = camera.getQuaternion();
            mouseDownX = x;
            mouseDownY = y;
            mouseLeftDown = true;

/*
    double modelview[16], projection[16];
    int viewport[4];
    float z;

  glGetDoublev( GL_PROJECTION_MATRIX, projection );

  glGetDoublev( GL_MODELVIEW_MATRIX, modelview );

  glGetIntegerv( GL_VIEWPORT, viewport );

  glReadPixels( x, viewport[3]-y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z );

  gluUnProject( x, viewport[3]-y, z, modelview, projection, viewport, &objx, &objy, &objz );
  fprintf(stderr, "Object: (%lf, %lf, %lf)\n", objx, objy, objz);
*/

/*
// 1. Get state matrices and viewport
glEnable(GL_DEPTH_TEST);
GLdouble modelview[16];
GLdouble projection[16];
GLint viewport[4];
glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
glGetDoublev(GL_PROJECTION_MATRIX, projection);
glGetIntegerv(GL_VIEWPORT, viewport);

// 2. Adjust mouse Y from top-left (GUI) to bottom-left (OpenGL)
int winX = mouseX;
int winY = viewport[3] - mouseY - 1; 

// 3. Ensure rendering is complete
glFinish();

// 4. Read depth value Z
GLfloat winZ = 0.0f;
glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

// 5. Unproject to world coordinates
GLdouble posX = 0.0, posY = 0.0, posZ = 0.0;
gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
fprintf(stderr, "Position: (%lf, %lf, %lf)\n", posX, posY, posZ);
glDisable(GL_DEPTH_TEST);
*/

//gl_select(x,y);

        }
        else if(state == GLUT_UP)
        {
            mouseLeftDown = false;
        }
    }

    else if(button == GLUT_RIGHT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseDownX = x;
            mouseDownY = y;
            mouseRightDown = true;
        }
        else if(state == GLUT_UP)
        {
            mouseRightDown = false;
        }
    }
}


void mouseMotionCB(int x, int y)
{
    const float SCALE_ANGLE = 0.2f;
    const float SCALE_SHIFT = 0.2f;
    float prevX = mouseX;
    float prevY = mouseY;
    mouseX = x;
    mouseY = y;

    if(mouseLeftDown)
    {
        Vector3 delta;
        delta.y = (mouseX - mouseDownX) * SCALE_ANGLE;
        delta.x = (mouseY - mouseDownY) * SCALE_ANGLE;

        // re-compute camera matrix
        /*
        //@@ using Euler angles
        Vector3 angle;
        angle.x = cameraAngle.x + delta.x;
        angle.y = cameraAngle.y - delta.y;  // must negate for camera
        //camera.rotateTo(angle);
        camera.rotateTo(angle, 0.5f, Gil::EASE_OUT);
        */

        //@@ using quaternion
        Quaternion qx = Quaternion(Vector3(1,0,0), delta.x * DEG2RAD * 0.5f);   // rotate along X-axis
        Quaternion qy = Quaternion(Vector3(0,1,0), delta.y * DEG2RAD * 0.5f);   // rotate along Y-axis
        Quaternion q = qx * qy * cameraQuat;
        //Quaternion q = Quaternion::getQuaternion(delta * DEG2RAD * 0.5f); // quat from angles
        //q *= cameraQuat;
        //camera.rotateTo(q);
        camera.rotateTo(q, 0.5f, Gil::EASE_OUT);

        /*
        //@@ using delta angle
        //camera.rotate(delta);
        //camera.rotate(delta, 0.5f, Gil::EASE_OUT);
        */
    }
    if(mouseRightDown)
    {
        Vector2 delta;
        delta.x = (mouseX - prevX) * SCALE_SHIFT;
        delta.y = (mouseY - prevY) * SCALE_SHIFT;
        camera.shift(delta, 0.5f, Gil::EASE_OUT);
    }

}


void specialCB(int key, int x, int y)
{
    Vector2 shiftVector;
    bool shifting = false;
    switch(key)
    {
    case GLUT_KEY_LEFT:
        if(!leftKeyDown)
        {
            shiftVector.x = 1;          // shift camera to left
            leftKeyDown = true;
            shifting = true;
        }
        break;

    case GLUT_KEY_RIGHT:
        if(!rightKeyDown)
        {
            shiftVector.x = -1;         // shift camera to right
            rightKeyDown = true;
            shifting = true;
        }
        break;

    case GLUT_KEY_UP:
        if(!upKeyDown)
        {
            shiftVector.y = 1;          // shift camera upward
            upKeyDown = true;
            shifting = true;
        }
        break;

    case GLUT_KEY_DOWN:
        if(!downKeyDown)
        {
            shiftVector.y = -1;         // shiftcamera downward
            downKeyDown = true;
            shifting = true;
        }
        break;

    default:
        ;
    }

    // pan camera to adjust camera's target point
    if(shifting)
    {
        shiftVector.normalize();
        shiftVector *= MOVE_SPEED;
        camera.startShift(shiftVector, MOVE_ACCEL);
        //camera.shift(shiftVector, 0.5f, Gil::EASE_OUT);
    }
}


void specialUpCB(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_LEFT:
        leftKeyDown = false;
        break;

    case GLUT_KEY_RIGHT:
        rightKeyDown = false;
        break;

    case GLUT_KEY_UP:
        upKeyDown = false;
        break;

    case GLUT_KEY_DOWN:
        downKeyDown = false;
        break;

    default:
        ;
    }
    // stop shifting
    camera.stopShift();
}


void exitCB()
{
    clearSharedMem();
}
