#include <stdbool.h>

//#include<gl/glut.h>       // Also included gl.h and glu.h
  #include<GL/glut.h>       // Also included gl.h and glu.h

#define IMAGE_ROWS 64      // Texture image rows and columns
#define IMAGE_COLS 64

// Variable to control the speed of rotation with the mouse
#define DEGREES_PER_PIXEL  0.6f

// Structure to keep track of the mouse state 
typedef struct {
	bool leftButton;
	bool rightButton;
	int x;
	int y;
} MouseState;

MouseState mouseState = {false, false, 0, 0};
   
// Global Variables
char title[] = "Texture - Checkerboard Pattern";  // Title in windowed mode
int windowWidth  = 640;     // Windowed mode's width
int windowHeight = 480;     // Windowed mode's height
int windowPosX   = 50;      // Windowed mode's top-left corner x
int windowPosY   = 50;      // Windowed mode's top-left corner y
bool fullScreenMode = true; // Full-screen or windowed mode?
   
// Texture
GLubyte imageData[IMAGE_ROWS][IMAGE_COLS][3]; // Texture image data
   
// Animation
GLfloat xAngle = 0.0f;  // Rotational angle about the x-axis
GLfloat yAngle = 0.0f;  // Rotational angle about the y-axis
   
// Load the imageData array with checkerboad pattern
void loadTextureImageData() {
   int value;
   for (int row = 0; row < IMAGE_ROWS; row++) {
      for (int col = 0; col < IMAGE_COLS; col++) {
         // Each cell is 8x8, value is 0 or 255 (black or white)
         value = (((row & 0x8) == 0) ^ ((col & 0x8) == 0)) * 255;
         imageData[row][col][0] = (GLubyte)value;
         imageData[row][col][1] = (GLubyte)value;
         imageData[row][col][2] = (GLubyte)value;
      }
   }
}
   
// Initialize OpenGL
void initGL(GLvoid) {
   glShadeModel(GL_SMOOTH);               // Enable smooth shading of color
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // Set background (clear) color to white
   
   // Depth-buffer for hidden surface removal
   glClearDepth(1.0f);       // Set clear depth value to farthest
   glEnable(GL_DEPTH_TEST);  // Enable depth-buffer for hidden surface removal
   glDepthFunc(GL_LEQUAL);   // The type of depth testing to do
   
   // Texture
   loadTextureImageData();   // Load pattern into image data array
   glTexImage2D(GL_TEXTURE_2D, 0, 3, IMAGE_COLS, IMAGE_ROWS, 0, GL_RGB, 
         GL_UNSIGNED_BYTE, imageData);  // Create texture from image data
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   
   glEnable(GL_TEXTURE_2D);  // Enable 2D texture 
   
   // Correct texture distortion in perpective projection
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}
   
// Handler for window paint and re-paint event
void display(void) {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen and depth buffers
   
   // Draw cube
   glLoadIdentity();   // Reset the view
   glTranslatef(0.0f, 0.0f, -5.0f);
   glRotatef(xAngle, 1.0f, 0.0f, 0.0f);
   glRotatef(yAngle, 0.0f, 1.0f, 0.0f);
   
   glBegin(GL_QUADS);
      // Front Face
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
      glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
      // Back Face
      glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
      glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
      // Top Face
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
      glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
      // Bottom Face
      glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
      glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
      // Right face
      glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
      glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
      // Left Face
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
   glEnd();
   glutSwapBuffers(); // Swap front and back buffers (double buffered mode)  
 }
   
// Handler for window's re-size event
void reshape(GLsizei width, GLsizei height) {  // GLsizei: non-negative integer
   if (height == 0) height = 1;  // prevent divide by 0
   
   // Set the viewport (display area) to cover entire application window
   glViewport(0, 0, width, height);
   
   // Select the aspect ratio of the clipping area to match the viewport
   glMatrixMode(GL_PROJECTION);  // Select the Projection matrix
   glLoadIdentity();             // Reset the Projection matrix
   gluPerspective(45.0, (float)width / (float)height, 0.1, 100.0);
   
   // Reset the Model-View matrix
   glMatrixMode(GL_MODELVIEW);  // Select the Model-View matrix
   glLoadIdentity();            // Reset the Model-View matrix
}
   
// Handler for key event
void keyboard(unsigned char key, int x, int y) {
   switch (key) {
      case 27:  // ESC key: exit the program
         exit(0); break;
      default: break;
   }
}
   
// Handler for special-key event
void specialKey(int key, int x, int y) {
   switch (key) {
      case GLUT_KEY_F1:    // F1: Toggle between full-screen and windowed mode
         fullScreenMode = !fullScreenMode;         // Toggle state
         if (fullScreenMode) {                     // Full-screen mode
            windowPosX   = glutGet(GLUT_WINDOW_X); // Save parameters
            windowPosY   = glutGet(GLUT_WINDOW_Y);
            windowWidth  = glutGet(GLUT_WINDOW_WIDTH);
            windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
            glutFullScreen();                      // Switch into full screen
         } else {                                         // Windowed mode
            glutReshapeWindow(windowWidth, windowHeight); // Switch into windowed mode
            glutPositionWindow(windowPosX, windowPosX);   // Postion top-left corner
         }
         break;
      default: break;
   }
}

// Handler for mouse event
void mouse(int button, int state, int x, int y)
{
	// update the button state
	if(button == GLUT_LEFT_BUTTON)
	{
		if(state == GLUT_DOWN)
			mouseState.leftButton = true;
		else
			mouseState.leftButton = false;
	}
	if(button == GLUT_RIGHT_BUTTON)
	{
		if(state == GLUT_DOWN)
			mouseState.rightButton = true;
		else
			mouseState.rightButton = false;
	}
	
	// update the mouse position, so we can track the mouse move
	mouseState.x = x;
	mouseState.y = y;
}

// Handler for mouse move event
void mouseMove(int x, int y)
{
	// Calculate the displacement in movement 
	int xDelta = mouseState.x - x;
	int yDelta = mouseState.y - y;

	// Commit the mouse position 
	mouseState.x = x;
	mouseState.y = y;

	// If left button is down, change the rotation angle for next frame 
	if(mouseState.leftButton) {
		xAngle -= yDelta * DEGREES_PER_PIXEL;
		yAngle -= xDelta * DEGREES_PER_PIXEL;
	}
	glutPostRedisplay();
}


// main function: GLUT runs as a console application
int main(int argc, char** argv) {
   glutInit(&argc, argv);      // Initialize GLUT
   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH); // Set modes
   glutInitWindowSize(windowWidth, windowHeight);  // Initial window width and height
   glutInitWindowPosition(windowPosX, windowPosY); // Initial window top-left corner (x, y)
   glutCreateWindow(title);     // Create window with the given title
   glutDisplayFunc(display);    // Register handler for window re-paint
   glutReshapeFunc(reshape);    // Register handler for window re-size
   glutKeyboardFunc(keyboard);  // Register handler for key event
   glutSpecialFunc(specialKey); // Register handler for special-key event
   glutMouseFunc(mouse);        // Register handler for mouse event
   glutMotionFunc(mouseMove);   // Register handler for mouse move event
   initGL();          // Our own OpenGL initialization
   glutMainLoop();    // Enter the infinitely event-handling loop
   return 0;
}
