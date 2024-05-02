/*
 *  --PLAYER CLASS--
 *
 *  SHOULD BE ABLE TO INITIALIZE OPENGL STUFF
 *  & CAMERA STUFF, ALSO RUNS DRAW FUNCTIONS
 * 
 *  HANDLES MAIN WINDOW LOOP
 */

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "World.hpp"
#include "shader.hpp"
#include "controls.hpp"

const bool DEBUG = true;

extern GLuint programID;

using namespace std;
using namespace glm;

class Player {
private:
    GLFWwindow* window;
    GLuint programID;

    World* world;
    vec3 position;
    int renderDistance;

public:
    Player(World* world);
    Player(World* world, vec3 pos);
    Player(World* world, vec3 pos, int rendist);
    ~Player();

    bool Init();
    int setupWindow(bool vsync, bool fullscreen);

    void renderWorld();
};

Player::~Player() {

};

Player::Player(World* w) {
    Player(w, { 0, 4, 0 });
}

Player::Player(World* w, vec3 pos) {
    Player(w, pos, 32);
}

Player::Player(World* w, vec3 pos, int rendist) : world(w), position(pos), renderDistance(rendist) {}

bool Player::Init() {

    this->setupWindow(true, false);

    // Create & Load the shader
    programID = LoadShaders( "src/shaders/shader.vert", "src/shaders/shader.frag" );
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    // Setup the input modes
	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwPollEvents();
	glfwSetCursorPos(window, width/2, height/2);

    // Setup OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glFrontFace(GL_CW);
    if(DEBUG) glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    // Set light data
    vec3 lightDirection = vec3(0.f, 0.f, 1.f);

    // Get the uniform locations
    GLint lightDirUniformLocation = glGetUniformLocation(programID, "lightDirection");

    // Main Program loop
    while (!glfwWindowShouldClose(window)) {

        // Update the world
        this->world->UpdateChunks(position);

        // Clears Buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);

        // Update Camera
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniform3fv(lightDirUniformLocation, 1, value_ptr(lightDirection));

        // Render the world
        unique_lock<mutex> lock(chunkMutex);
        chunkCondition.wait(lock, [] { return !chunks.empty(); });
        for (auto& chunk : chunks) {
            chunk.second.Draw();
        }
        lock.unlock();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

        // Update the window
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Close the program
    glDeleteProgram(programID);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


int Player::setupWindow(bool vsync, bool fullscreen) {
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

void Player::renderWorld() {
    //TODO: Make chunks only check to render chunk sides if there is one next to it, else dont. FOR THE WORLD NOT PLAYER OR CHUNK
    //vector<Chunk*> chunks = this->world->GetChunks(this->position, this->renderDistance);

    for(int i = 0; i < chunks.size(); i++) {
        
    }
}
