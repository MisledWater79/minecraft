#pragma once
#ifndef BLOCK_H
#define BLOCK_H

#include <vector>

#include "GLMinclude.hpp"

using namespace std;

/*
vector<GLfloat> vertices = {
    0.0f, 0.0f, 1.0f, // Bottom-left
    1.0f, 0.0f, 1.0f, // Bottom-right
    1.0f, 1.0f, 1.0f, // Top-right
    1.0f, 1.0f, 1.0f, // Top-right
    0.0f, 1.0f, 1.0f, // Top-left
    0.0f, 0.0f, 1.0f, // Bottom-left

    
    0.0f, 0.0f, 0.0f, // Bottom-right
    1.0f, 0.0f, 0.0f, // Bottom-left
    1.0f, 1.0f, 0.0f, // Top-left
    1.0f, 1.0f, 0.0f, // Top-left
    0.0f, 1.0f, 0.0f, // Top-right
    0.0f, 0.0f, 0.0f, // Bottom-right
    0.0f, 1.0f, 0.0f, // Top-left
    0.0f, 1.0f, 1.0f, // Bottom-left
    1.0f, 1.0f, 1.0f, // Bottom-right
    1.0f, 1.0f, 1.0f, // Bottom-right
    1.0f, 1.0f, 0.0f, // Top-right
    0.0f, 1.0f, 0.0f, // Top-left
    0.0f, 0.0f, 0.0f, // Top-right
    1.0f, 0.0f, 0.0f, // Top-left
    1.0f, 0.0f, 1.0f, // Bottom-left
    1.0f, 0.0f, 1.0f, // Bottom-left
    0.0f, 0.0f, 1.0f, // Bottom-right
    0.0f, 0.0f, 0.0f, // Top-right
    1.0f, 0.0f, 0.0f, // Bottom-left
    1.0f, 1.0f, 0.0f, // Top-left
    1.0f, 1.0f, 1.0f, // Top-right
    1.0f, 1.0f, 1.0f, // Top-right
    1.0f, 0.0f, 1.0f, // Bottom-right
    1.0f, 0.0f, 0.0f, // Bottom-left
    0.0f, 0.0f, 0.0f, // Bottom-right
    0.0f, 1.0f, 0.0f, // Top-right
    0.0f, 1.0f, 1.0f, // Top-left
    0.0f, 1.0f, 1.0f, // Top-left
    0.0f, 0.0f, 1.0f, // Bottom-left
    0.0f, 0.0f, 0.0f, // Bottom-right
    
};


vector<GLfloat> uvs = {
    // Front face
    0.125f, 0.0625f,
    0.1875f, 0.0625f,
    0.1875f, 0.0f,
    0.1875f, 0.0f,
    0.125f, 0.0f,
    0.125f, 0.0625f,

    // Back face
    0.125f, 0.0625f,
    0.1875f, 0.0625f,
    0.1875f, 0.0f,
    0.1875f, 0.0f,
    0.125f, 0.0f,
    0.125f, 0.0625f,

    // Top face
    0.125f, 0.0625f,
    0.1875f, 0.0625f,
    0.1875f, 0.0f,
    0.1875f, 0.0f,
    0.125f, 0.0f,
    0.125f, 0.0625f,

    // Bottom face
    0.125f, 0.0625f,
    0.1875f, 0.0625f,
    0.1875f, 0.0f,
    0.1875f, 0.0f,
    0.125f, 0.0f,
    0.125f, 0.0625f,

    // Right face
    0.125f, 0.0625f,
    0.1875f, 0.0625f,
    0.1875f, 0.0f,
    0.1875f, 0.0f,
    0.125f, 0.0f,
    0.125f, 0.0625f,

    // Left face
    0.125f, 0.0625f,
    0.1875f, 0.0625f,
    0.1875f, 0.0f,
    0.1875f, 0.0f,
    0.125f, 0.0f,
    0.125f, 0.0625f,
};
*/

//"001101111111011001000100110110010000010011111111110010000100101101001000100110111111101100000010011011001000"

const vector<bool> vertices = {
    0, 0, 1,
    1, 0, 1,
    1, 1, 1,
    1, 1, 1,
    0, 1, 1,
    0, 0, 1,

    0, 0, 0,
    1, 0, 0,
    1, 1, 0,
    1, 1, 0,
    0, 1, 0,
    0, 0, 0,
    
    0, 1, 0,
    0, 1, 1,
    1, 1, 1,
    1, 1, 1,
    1, 1, 0,
    0, 1, 0,

    0, 0, 0,
    1, 0, 0,
    1, 0, 1,
    1, 0, 1,
    0, 0, 1,
    0, 0, 0,

    1, 0, 0,
    1, 1, 0,
    1, 1, 1,
    1, 1, 1,
    1, 0, 1,
    1, 0, 0,

    0, 0, 0,
    0, 1, 0,
    0, 1, 1,
    0, 1, 1,
    0, 0, 1,
    0, 0, 0
};

class Block {
private:
    //0000 00
    //textureID: 00 0000 0000 
    //xyz: 0000  0000 0000  0000
    int displayData;
public:
    Block();

    void Draw(short sides);

    void getVertex(short sides);
};

Block::Block() {

}

void Block::Draw(short sides) {

        /*
            [      ----Vertex Arry----       ]
                    --Vertex 1--
            pos(at0), col(at1), tex(at2) | ...
        */

        // Bind the vertex array
        GLuint* verts;
        GLuint* vertArr;
        GLuint* uvBuffer;
        glGenBuffers(1, vertArr);//creates a new glbuffer and gives us the ID for it
        glGenBuffers(1, uvBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, *vertArr);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW); //Sets currents array buffer to the stuff we specify
        
        glGenVertexArrays(1, verts);
        glBindBuffer(GL_ARRAY_BUFFER, *vertArr); //Sets what buffer we are using for buff calls
        glVertexAttribPointer(0, 3, GL_BOOL, GL_FALSE, 3 * sizeof(bool), (void*)0); //Tells gl how big our atributes are and how far appart they are
        glEnableVertexAttribArray(0); //Enables the attribute so its used, i think

        // Bind the UV array
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, *uvBuffer);
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

#endif