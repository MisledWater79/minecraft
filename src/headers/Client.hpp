/*
 *  --PLAYER CLASS--
 *
 *  SHOULD BE ABLE TO INITIALIZE OPENGL STUFF
 *  & CAMERA STUFF, ALSO RUNS DRAW FUNCTIONS
 * 
 *  HANDLES MAIN WINDOW LOOP
 */

#include "controls.hpp"
#include "GLMinclude.hpp"
#include "shader.hpp"
#include "World.hpp"

extern GLuint* programID;

using namespace std;
using namespace glm;

class Client {
private:
    GLFWwindow* window;

    World* world;
    vec3 position;
    int renderDistance;

public:
    Client(World* world);
    Client(World* world, vec3 pos);
    Client(World* world, vec3 pos, int rendist);
    ~Client();

    bool Init();
    int setupWindow(bool vsync, bool fullscreen);

    void renderWorld();
};

Client::~Client() {

};

Client::Client(World* w) {
    Client(w, { 0, 4, 0 });
}

Client::Client(World* w, vec3 pos) {
    Client(w, pos, 32);
}

Client::Client(World* w, vec3 pos, int rendist) : world(w), position(pos), renderDistance(rendist) {}

bool Client::Init() {

    this->setupWindow(true, false);

    // Create & Load the shader
    *programID = LoadShaders( "src/shaders/shader.vert", "src/shaders/shader.frag" );
    GLuint MatrixID = glGetUniformLocation(*programID, "MVP");

    // Setup the input modes
	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwPollEvents();
	glfwSetCursorPos(window, width/2, height/2);

    // Setup OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glFrontFace(GL_CW);
    #ifdef MCDEBUG
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    #endif

    // Set light data
    vec3 lightDirection = vec3(0.f, 0.f, 1.f);

    // Get the uniform locations
    GLint lightDirUniformLocation = glGetUniformLocation(*programID, "lightDirection");

    // Main Program loop
    while (!glfwWindowShouldClose(window)) {

        // Update the world
        //this->world->UpdateChunks(position);

        // Clears Buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(*programID);

        // Update Camera
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniform3fv(lightDirUniformLocation, 1, value_ptr(lightDirection));

        // Update the window
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Close the program
    glDeleteProgram(*programID);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


int Client::setupWindow(bool vsync, bool fullscreen) {
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 0);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (fullscreen) {
        window = glfwCreateWindow(width, height, "Minecraft", glfwGetPrimaryMonitor(), NULL);
    }
    else {
        window = glfwCreateWindow(width, height, "Minecraft", NULL, NULL);
    }
    if (window == NULL) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (vsync) {
	    glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }

    // Check for errors
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return -1;
    }

    return 1;
}

void Client::renderWorld() {
    //TODO: Make chunks only check to render chunk sides if there is one next to it, else dont. FOR THE WORLD NOT PLAYER OR CHUNK
    //vector<Chunk*> chunks = this->world->GetChunks(this->position, this->renderDistance);

}
