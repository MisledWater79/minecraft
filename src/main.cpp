#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <vector>
#include <iostream>
#include <windows.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "headers/world.hpp"
#include "headers/shader.hpp"
#include "headers/controls.hpp"
#include "headers/object.hpp"

extern "C"
{
   __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
   __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

GLFWwindow* window;
GLuint programID;
const int width = 1280, height = 720;

using namespace std;
using namespace glm;

vector<vec3> vertices = {
    //back
	{0, 0, 0},
    {1, 0, 0},
    {0, 1, 0},
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},

    //left
    {0, 0, 1},
    {0, 0, 0},
    {0, 1, 1},
    {0, 1, 1},
    {0, 0, 0},
    {0, 1, 0},

    //front
    {1, 0, 1},
    {0, 0, 1},
    {1, 1, 1},
    {1, 1, 1},
    {0, 0, 1},
    {0, 1, 1},
    
    //right
    {1, 0, 0},
    {1, 0, 1},
    {1, 1, 0},
    {1, 1, 0},
    {1, 0, 1},
    {1, 1, 1},

    //top
    {0, 1, 0},
    {1, 1, 0},
    {0, 1, 1},
    {0, 1, 1},
    {1, 1, 0},
    {1, 1, 1},

    //bottom
    {0, 1, 0},
    {1, 1, 0},
    {0, 1, 1},
    {0, 1, 1},
    {1, 1, 0},
    {1, 1, 1},
};

vector<vec2> uvs = {
    // Front face
    {0.125f, 0.0f},
    {0.1875f, 0.0f},
    {0.1875f, 0.0625f},
    {0.125f, 0.0625f},

    // Back face
    {0.125f, 0.0f},
    {0.1875f, 0.0f},
    {0.1875f, 0.0625f},
    {0.125f, 0.0625f},

    // Top face
    {0.125f, 0.0f},
    {0.1875f, 0.0f},
    {0.1875f, 0.0625f},
    {0.125f, 0.0625f},

    // Bottom face
    {0.125f, 0.0f},
    {0.1875f, 0.0f},
    {0.1875f, 0.0625f},
    {0.125f, 0.0625f},

    // Left face
    {0.125f, 0.0f},
    {0.1875f, 0.0f},
    {0.1875f, 0.0625f},
    {0.125f, 0.0625f},

    // Right face
    {0.125f, 0.0f},
    {0.1875f, 0.0f},
    {0.1875f, 0.0625f},
    {0.125f, 0.0625f},
};

int setupWindow(bool vsync, bool fullscreen);

int main() {

    // Create the window
    setupWindow(true, false);

    // Create the shader
    programID = LoadShaders( "src/shaders/shader.vert", "src/shaders/shader.frag" );
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    // Setup the input modes
	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwPollEvents();
	glfwSetCursorPos(window, width/2, height/2);

    // Set the opengl settings
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glFrontFace(GL_CW);
    glPolygonMode(GL_CCW, GL_CLEAR);
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    // Set light data
    vec3 lightDirection = vec3(0.f, 0.f, 1.f);

    // Get the uniform locations
    GLint lightDirUniformLocation = glGetUniformLocation(programID, "lightDirection");

    // Generate the seed
    uint64_t seed = time(NULL);

    cout << "Seed: " << seed << "\n";

    // Create the world
    World world(seed, 1);
    world.startGenThread();

    Object block(vertices, uvs);

    int i = 0;
    // Run the program
    while (!glfwWindowShouldClose(window)) {
        //if(i >= 35) break;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);

		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        // print out the directions
        // printPositions("clear");

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniform3fv(lightDirUniformLocation, 1, value_ptr(lightDirection));

        block.Draw();

        // Update the world
        //world.GenerateChunks();
        //world.UpdateChunks(position);

        // Render the world
        unique_lock<mutex> lock(chunkMutex);
        // Copy the chunks
        for (auto& chunk : chunks) {
            if(chunk.second.generated) {
                //chunk.second.Draw();
                //printf("Rendering: %d, %d : %d\n", chunk.first.x, chunk.first.z, chunk.second.generated);
                //printf("Verts: %d\n", chunk.second.vertices.size());
            }
        }
        //printf("Rendered Chunks\n");
        lock.unlock();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

        // Update the window
        glfwSwapBuffers(window);
        glfwPollEvents();
        i++;
    }

    world.stopGenThread();

    // Close the program
    glDeleteProgram(programID);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


int setupWindow(bool vsync, bool fullscreen) {
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

    return 0;
}