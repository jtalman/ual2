const char *WINDOWTITLE = { "OpenGL / GLUT / GLUI Sample -- Joe Graphics" };
const char *GLUITITLE   = { "User Interface Window" };

GLdouble fovy=40;
GLdouble aspect=3;
GLdouble zNear=1;
GLdouble zFar=1000;

GLdouble top = zNear * tan( (fovy / 2.0) * (M_PI / 180.0) );
GLdouble bottom = -top;
GLdouble right = top * aspect;
GLdouble left = -right;

GLdouble offset = 2;

// what the glui package defines as true and false:

//const int GLUITRUE  = { true  };
//const int GLUIFALSE = { false };

// the escape key:

#define ESCAPE		0x1b


// initial window size:

const int INIT_WINDOW_SIZE = { 750 };

// multiplication factors for input interaction:
//  (these are known from previous experience)

const float ANGFACT = { 1. };
const float SCLFACT = { 0.005f };


// able to use the left mouse for either rotation or scaling,
// in case have only a 2-button mouse:

enum LeftButton
{
	ROTATE,
	SCALE
};


// minimum allowable scale factor:

const float MINSCALE = { 0.05f };


// active mouse buttons (or them together):

const int LEFT   = { 4 };
const int MIDDLE = { 2 };
const int RIGHT  = { 1 };


// which projection:

enum Projections
{
	ORTHO,
	PERSP
};


// which button:

enum ButtonVals
{
 AXES,
 PROJ,
 DEPTH,
 C_step,
 SUN_POS,
 MOON_POS,
 EARTH_POS,
 DBUG,
 RESET,
 QUIT
};

/*
enum RadioVals
{
 SPHERE,
 ICONIC
};
*/

// window background color (rgba):

const float BACKCOLOR[] = { 0., 0., 0., 0. };


// color and line width for the axes:

const GLfloat AXES_COLOR[] = { 1., .5, 0. };
const GLfloat AXES_WIDTH   = { 3. };


// the objects:
// this order must match the radio button order
// these const variables are not actually used in the program
// they are just here to remind what the order must be

int RADIOBUTTON_ID=0;

enum Display_Mode
{
	SPHERE,
	ICONIC,
	STEP,
	TORUS,
	DODECAHEDRON,
	OCTAHEADRON,
	TETRAHEDRON,
	ICOSAHEDRON,
	TEAPOT
};


// max # of objects:

#define MAXOBJECTS		9


// the color numbers:
// this order must match the radio button order

enum Colors
{
	RED,
	YELLOW,
	GREEN,
	CYAN,
	BLUE,
	MAGENTA,
	WHITE,
	BLACK
};


// the color definitions:
// this order must match the radio button order

const GLfloat Colors[8][3] = 
{
	{ 1., 0., 0. },		// red
	{ 1., 1., 0. },		// yellow
	{ 0., 1., 0. },		// green
	{ 0., 1., 1. },		// cyan
	{ 0., 0., 1. },		// blue
	{ 1., 0., 1. },		// magenta
	{ 1., 1., 1. },		// white
	{ 0., 0., 0. },		// black
};


// fog parameters:

const GLfloat FOGCOLOR[4] = { .0, .0, .0, 1. };
const GLenum  FOGMODE     = { GL_LINEAR };
const GLfloat FOGDENSITY  = { 0.30f };
const GLfloat FOGSTART    = { 1.5 };
const GLfloat FOGEND      = { 4. };



//
// non-constant global variables:
//

int	ActiveButton;		// current button that is down
GLuint	AxesList;		// list to hold the axes
int	AxesOn=true;		// ON or OFF
int	Debug=true;		// ON means print debug info
int	DepthCueOn=false;	// TRUE means to use intensity depth cueing
GLUI *	Glui;			// instance of glui window
int	GluiWindow;		// the glut id for the glui window
int	LeftButton;		// either ROTATE or SCALE
GLuint	Lists[MAXOBJECTS];	// object display lists

int	MainWindow;		// window id for main graphics window
//int	handle;			// window id for main graphics window

GLfloat	RotMatrix[4][4];	// set by glui rotation widget
float	Scale, Scale2;		// scaling factors
int	WhichColor;		// index into Colors[]
int	WhichObject;		// object index to display
int	WhichProjection=PERSP;	// ORTHO or PERSP
int	Xmouse, Ymouse;		// mouse values
float	Xrot=0, Yrot=0;		// rotation angles in degrees
float	TransXYZ[3];		// set by glui translation widgets


#define WINGS	0.10


// axes:

#define X	1
#define Y	2
#define Z	3


// x, y, z, axes:

static float axx[3] = { 1., 0., 0. };
static float ayy[3] = { 0., 1., 0. };
static float azz[3] = { 0., 0., 1. };


static float xx[] = {
		0.f, 1.f, 0.f, 1.f
	      };

static float xy[] = {
		-.5f, .5f, .5f, -.5f
	      };

static int xorder[] = {
		1, 2, -3, 4
		};


static float yx[] = {
		0.f, 0.f, -.5f, .5f
	      };

static float yy[] = {
		0.f, .6f, 1.f, 1.f
	      };

static int yorder[] = {
		1, 2, 3, -2, 4
		};


static float zx[] = {
		1.f, 0.f, 1.f, 0.f, .25f, .75f
	      };

static float zy[] = {
		.5f, .5f, -.5f, -.5f, 0.f, 0.f
	      };

static int zorder[] = {
		1, 2, 3, 4, -5, 6
		};


// fraction of the length to use as height of the characters:

const float LENFRAC = 0.10f;


// fraction of length to use as start location of the characters:

const float BASEFRAC = 1.10f;
