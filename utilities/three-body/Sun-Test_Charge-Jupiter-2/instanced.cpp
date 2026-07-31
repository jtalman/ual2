#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include <cmath>


GLuint VBO;
#define VERTEX_LOCATION 0

GLuint MBO;
#define MATRICES_LOCATION 1
#define NUM_INSTANCES 1000

static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, (GLsizei)NUM_INSTANCES);

    glutSwapBuffers();
}


static void CreateVertexBuffer()
{
    float vertices[9] = {-0.3f, -0.3f , 0.0f,
                          0.3f, -0.3f , 0.0f,
                          0.0f,  0.25f, 0.0f};
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(VERTEX_LOCATION, 3, GL_FLOAT, GL_FALSE, (GLsizei)3*sizeof(float), NULL);
    glEnableVertexAttribArray(VERTEX_LOCATION);

    float mat[NUM_INSTANCES * 16];
    for (int matrix_id=0; matrix_id < NUM_INSTANCES; matrix_id++) {
        float pos_x = 0.002f *  matrix_id * cos(40*M_PI*matrix_id / NUM_INSTANCES);
        float pos_y = 0.002f *  matrix_id * sin(40*M_PI*matrix_id / NUM_INSTANCES);
        float scale = 0.0004f * matrix_id;
        int i = 16 * matrix_id;
        mat[i+0]  = scale; mat[i+4] = 0.0f;  mat[i+8]  = 0.0f;  mat[i+12] = pos_x; 
        mat[i+1]  = 0.0f;  mat[i+5] = scale; mat[i+9]  = 0.0f;  mat[i+13] = pos_y; 
        mat[i+2]  = 0.0f;  mat[i+6] = 0.0f;  mat[i+10] = scale; mat[i+14] = 0.0f; 
        mat[i+3] =  0.0f;  mat[i+7] = 0.0f;  mat[i+11] = 0.0f;  mat[i+15] = 1.0f; 
    }
    glGenBuffers(1, &(MBO));
    glBindBuffer(GL_ARRAY_BUFFER, MBO);
    glBufferData(GL_ARRAY_BUFFER, NUM_INSTANCES * 16 * sizeof(float),
                 mat, GL_DYNAMIC_DRAW);
    for (unsigned int i = 0; i < 4; i++) {
      glEnableVertexAttribArray(MATRICES_LOCATION + i);
      glVertexAttribPointer(MATRICES_LOCATION + i, 4, GL_FLOAT, GL_FALSE,
                            16 * sizeof(float),
                            (const GLvoid *)(sizeof(GLfloat) * i * 4));
      glVertexAttribDivisor(MATRICES_LOCATION + i, 1);
    }
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(0);
    }

    const GLchar* p[1];
    p[0] = pShaderText;

    GLint Lengths[1];
    Lengths[0] = (GLint)strlen(pShaderText);

    glShaderSource(ShaderObj, 1, p, Lengths);

    glCompileShader(ShaderObj);

    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }

    glAttachShader(ShaderProgram, ShaderObj);
}

static void CompileShaders()
{
    GLuint ShaderProgram = glCreateProgram();

    if (ShaderProgram == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

    std::string vs, fs;

    vs = "#version 330 core\n"
         "layout (location = 0) in vec3 Vertex;\n"
         "layout (location = 1) in mat4 Matrix;\n"
         "void main()\n"
         "{\n"
         "  gl_Position = Matrix*vec4(Vertex.x, Vertex.y, Vertex.z, 1.0);\n"
         "}\n";
    AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

    fs = "#version 330 core\n"
         "out vec4 FragColor;\n"
         "void main()\n"
         "{\n"
         "  FragColor = vec4(1.0, 1.0, 1.0, 0.0);\n"
         "}\n";
    AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    glLinkProgram(ShaderProgram);

    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    glUseProgram(ShaderProgram);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    int width = 600;
    int height = 600;
    glutInitWindowSize(width, height);

    int x = 200;
    int y = 100;
    glutInitWindowPosition(x, y);
    int win = glutCreateWindow("Instanced Rendering");

    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    GLclampf Red = 0.2f, Green = 0.2f, Blue = 0.2f, Alpha = 0.0f;
    glClearColor(Red, Green, Blue, Alpha);

    CreateVertexBuffer();

    CompileShaders();

    glutDisplayFunc(RenderSceneCB);

    glutMainLoop();

    return 0;
}
