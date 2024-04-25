#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

extern GLuint programID;

using namespace std;
using namespace glm;

// Function Prototype
GLuint loadPNG(const char *imagepath, bool useAlphaChannel = false);

class Object {
public:
    Object() {};
    // Create the Object Class
    Object(vector<vec3> &vertex, vector<vec2> &uvs) {
        vertices_size = vertex.size();

        // Create the Vertex Array Object
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        // Setup the vertex buffer
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(vec3), &vertex[0], GL_STATIC_DRAW);

        // Setup the UV buffer
        glGenBuffers(1, &uvBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), &uvs[0], GL_STATIC_DRAW);

        Texture = loadPNG("content/terrain.png");
        TextureID = glGetUniformLocation(programID, "myTextureSampler");
    };  

    ~Object() {
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteBuffers(1, &uvBuffer);
        glDeleteVertexArrays(1, &VertexArrayID);
        glDeleteTextures(1, &Texture);
    };

    void Create(vector<vec3> &vertex, vector<vec2> &uvs) {
        vertices_size = vertex.size();

        // Create the Vertex Array Object
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        // Setup the vertex buffer
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(vec3), &vertex[0], GL_STATIC_DRAW);

        // Setup the UV buffer
        glGenBuffers(1, &uvBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), &uvs[0], GL_STATIC_DRAW);

        Texture = loadPNG("content/terrain.png");
        TextureID = glGetUniformLocation(programID, "myTextureSampler");
    }

    void Draw() {
        // Bind the vertex array
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Bind the UV array
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Bind the texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glUniform1i(TextureID, 0);

        // Draw the object
        glDrawArrays(GL_TRIANGLES, 0, vertices_size);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }

private:
    GLuint Texture;
    GLuint TextureID;
    GLuint VertexArrayID;
    GLuint vertexBuffer;
    GLuint uvBuffer;

    uint vertices_size;
};

GLuint loadPNG(const char *imagepath, bool useAlphaChannel) {
    int width, height, nrChannels;
    unsigned char *data = stbi_load(imagepath, &width, &height, &nrChannels, 0);
    if (!data) {
        printf("%s could not be opened. Are you in the right directory? \n", imagepath);
        getchar();
        return 0;
    }

    // Determine the format
    GLenum format;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = useAlphaChannel ? GL_RGBA : GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    return textureID;
}