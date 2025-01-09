#include <GL/glew.h>        // Include GLEW first, before GLFW
#include <GLFW/glfw3.h>     // Then include GLFW
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>




// GLFW callback to adjust viewport size when window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}




int main()
{

    #pragma region initialization

    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "GLFW Initialization failed!" << std::endl;
        return -1;
    }


    //setting up glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 800, "my opengl try", NULL, NULL);


    // error checking
    if (!window)
    {
        std::cerr << "GLFW Window creation failed!" << std::endl;
        glfwTerminate();
        return -1;
    }


    // setting up glfw window
    glfwMakeContextCurrent(window);  // makes main window the current newly made window
    glfwSwapInterval(1);             // Enable vsync
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // Initialize GLEW and error checking
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW Initialization failed!" << std::endl;
        return -1;
    }

    #pragma endregion
   




    // Main rendering loop
    while (!glfwWindowShouldClose(window))
    {
        // check for events
        glfwPollEvents();



        // Rendering
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap buffers
        glfwSwapBuffers(window);
    }


    glfwDestroyWindow(window);
    glfwTerminate();
}
