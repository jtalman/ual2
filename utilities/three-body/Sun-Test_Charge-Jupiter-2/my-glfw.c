#include <glad/glad.h>
#include <GLFW/glfw3.h>

const float TARGET_FPS = 30.0f;
const float TARGET_FRAME_TIME = 1.0f / TARGET_FPS;

int main()
{
    // Initialize GLFW
    if (!glfwInit()) {
        return -1;
    }

    // Set OpenGL version (4.5 Core Profile)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return -1;
    }

    // Set viewport
    glViewport(0, 0, 800, 600);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Disable V-Sync since we're manually limiting FPS
    glfwSwapInterval(0);

    // FPS limiting
    float lastRenderTime = 0.0f;

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        float currentTime = (float)glfwGetTime();
        
        // Always process input for responsiveness
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        
        // Check if enough time has passed for rendering (30 FPS limit)
        if (currentTime - lastRenderTime < TARGET_FRAME_TIME) {
            glfwPollEvents();
            continue; // Skip rendering this frame
        }
        lastRenderTime = currentTime;

        // Rendering
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glfwTerminate();
    return 0;
}
