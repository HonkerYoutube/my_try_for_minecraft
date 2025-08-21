#include <GL/glew.h>        // Include GLEW first, before GLFW
#include <GLFW/glfw3.h>     // Then include GLFW
#include <filesystem>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "voxel_manager.h"
#include "constants.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"









// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

bool isCursorLocked = true; // Start with cursor locked
bool my_tool_active = false;


//unsigned int shaderProgram;

int close_window_key = GLFW_KEY_ESCAPE;
int open_ImGui_key = GLFW_KEY_TAB;


// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);



bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;


bool isSpeedModeActive = false;
double lastKeyPressTime = 0.0;
const double cooldownDuration = 0.25; // cooldown in seconds
const float speedModeMultiplier = 4.0f;


// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;









static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void setMat4(const std::string& name, const glm::mat4& mat);
std::string loadShaderAsString(const std::string& filename);










int main() {

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "my opengl try", NULL, NULL);


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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    // Initialize GLEW and error checking
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW Initialization failed!" << std::endl;
        return -1;
    }



    #pragma endregion



    #pragma region initialization of shaders



    // new shaders must be in out/build/x64-debug/shaders



    std::string vertexShaderStr = loadShaderAsString("resources/shaders/vertex.glsl");
    const char* vertexShaderSource = vertexShaderStr.c_str();

    std::string fragmentShaderStr = loadShaderAsString("resources/shaders/fragment.glsl");
    const char* fragmentShaderSource = fragmentShaderStr.c_str();








    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);




    #pragma endregion



    #pragma region init ImGui


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();


    #pragma endregion








    /*float vertices[] = {
        // image a cube that you could see all the vertices like you could see through it

        // back
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  // far back side bottom left vertex
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  // far back side bottom right vertex
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  // far back side top right vertex
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  // far back side top left vertex

        // front
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  // front side bottom left vertex
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  // front side bottom right vertex
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  // front side top right vertex
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  // front side top left vertex

        // left
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  // left side bottom left vertex
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  // left side bottom right vertex
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  // left side top right vertex
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  // left side top left vertex

        // right
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  // right side bottom left vertex
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  // right side bottom right vertex
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  // right side top right vertex
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  // right side top left vertex

        // botom
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  // bottom side bottom left vertex
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  // bottom side bottom right vertex
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  // bottom side top right vertex
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  // bottom side top left vertex

        // top
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  // top side bottom left vertex
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  // top side bottom right vertex
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  // top side top right vertex
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  // top side top left vertex

        };*/

    float vertices[6][24] = {
        // image a cube that you could see all the vertices like you could see through it

        // back
       {-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  // far back side bottom left vertex
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  // far back side bottom right vertex
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  // far back side top right vertex
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f},  // far back side top left vertex

        // front
       {-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  // front side bottom left vertex
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  // front side bottom right vertex
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  // front side top right vertex
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f},  // front side top left vertex

        // left
       {-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  // left side bottom left vertex
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  // left side bottom right vertex
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  // left side top right vertex
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f},  // left side top left vertex

        // right
       { 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  // right side bottom left vertex
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  // right side bottom right vertex
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  // right side top right vertex
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f},  // right side top left vertex

         // botom
        {-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  // bottom side bottom left vertex
          0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  // bottom side bottom right vertex
          0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  // bottom side top right vertex
         -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f},  // bottom side top left vertex

         // top
        {-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  // top side bottom left vertex
          0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  // top side bottom right vertex
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  // top side top right vertex
         -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f}  // top side top left vertex

    };

    unsigned int indices[6][6] = {
       {0, 1, 3,
        1, 2, 3},

       {4, 5, 7,
        5, 6, 7},

       {8, 9, 11,
        9, 10, 11},

       {12, 13, 15,
        13, 14, 15},

       {16, 17, 19,
        17, 18, 19},

       {20, 21, 23,
        21, 22, 23}
    };





    std::vector<voxel_manager::voxel> voxels;


    voxel_manager::voxel voxel1(0, 0, 0, voxel_manager::SOLID);
    voxels.push_back(voxel1);

    voxel_manager::voxel voxel2(1, 0, 0, voxel_manager::SOLID);
    voxels.push_back(voxel2);


    voxel_manager::chunk chunk(0, 0, 0);







    // 1. Create and bind a VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // 2. Create VBO and upload vertex data
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    // 3. Setup vertex attribute pointer (position)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    GLintptr vertex_color_offset = 3 * sizeof(float);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)vertex_color_offset);
    glEnableVertexAttribArray(1);


    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_DEPTH_TEST);




    /*std::cout << "Vertex shader source:\n" << vertexShaderStr << std::endl << std::endl << std::endl;
    std::cout << "Fragment shader source:\n" << fragmentShaderStr << std::endl;
    int nAttribs;
    glGetProgramiv(shaderProgram, GL_ACTIVE_ATTRIBUTES, &nAttribs);
    std::cout << "Active shader attributes: " << nAttribs << std::endl;*/





    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 view = glm::mat4(1.0f);
    // note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);







    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        #pragma region ImGui UI


        // Create a window called "My First Tool", with a menu bar.
        ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
                if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
                if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }


        ImGui::Text("player pos: %i, %i, %i", int(cameraPos.x), int(cameraPos.y), int(cameraPos.z));

        // Display contents in a scrolling region
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
        ImGui::BeginChild("Scrolling");
        for (int x = 0; x < chunkSize; x++) {
            for (int y = 0; y < chunkSize; y++) {
                for (int z = 0; z < chunkSize; z++) {

                    voxel_manager::voxel* voxel = &chunk.voxels[x][y][z];
                    ImGui::Text("x:%i y:%i z:%i :  back:%i, front:%i, left:%i, right:%i bottom:%i, top:%i", x, y, z, voxel->faces[0], voxel->faces[1], voxel->faces[2], voxel->faces[3], voxel->faces[4], voxel->faces[5]); // back, front, left, right, bottom, top
                }
            }
        }
        ImGui::EndChild();
        ImGui::End();


        #pragma endregion


        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
            


        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


        int modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));


        int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 




        int i = 0;
        for (int x = 0; x < chunkSize; x++) {
            for (int y = 0; y < chunkSize; y++) {
                for (int z = 0; z < chunkSize; z++) {
                    voxel_manager::voxel voxel = chunk.voxels[x][y][z];
                    voxel.makeAndRenderMesh();
                    i++;
                }
            }
        }
        //printf("voxels: %i, faces: %i \n", i, i*6);


        //printf("player: x: %f,  y: %f,  z: %f \n", cameraPos.x, cameraPos.y, cameraPos.z);




        // ImGui Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());





        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------






        glfwSwapBuffers(window);
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);


    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();


    glfwDestroyWindow(window);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}











/*void setMat4(const std::string& name, const glm::mat4& mat)
{
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}*/


std::string loadShaderAsString(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << filename << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}


// GLFW callback to adjust viewport size when window is resized
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
static void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, close_window_key) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Toggle cursor lock with TAB key
    if (glfwGetKey(window, open_ImGui_key) == GLFW_PRESS) {
        static double lastTabPressTime = 0.0;
        double currentTime = glfwGetTime();
        if (currentTime - lastTabPressTime > 0.2) { // Debounce the key press
            isCursorLocked = !isCursorLocked;
            lastTabPressTime = currentTime;

            if (isCursorLocked) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                my_tool_active = true;
            }
            else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                my_tool_active = false;
            }
        }
    }

    double currentTime = glfwGetTime();
    float cameraSpeed = 2.5f * deltaTime;

    if (!isSpeedModeActive)
        cameraSpeed = static_cast<float>(2.5 * deltaTime);
    else
        cameraSpeed = static_cast<float>(2.5 * deltaTime) * speedModeMultiplier;

    // Only process camera movement if the cursor is locked
    if (isCursorLocked) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            cameraPos += cameraSpeed * glm::vec3(0, 1, 0);
    }


    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && currentTime - lastKeyPressTime >= cooldownDuration)
    {
        isSpeedModeActive = !isSpeedModeActive;
        lastKeyPressTime = currentTime;
        std::cout << "speed: " << isSpeedModeActive << "\n";
    }


}


// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (!isCursorLocked) return; // Don't update camera if cursor is not locked

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

