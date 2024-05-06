#pragma once
#ifndef WORLD_H
#define WORLD_H

#include <mutex>
#include <thread>
#include <unordered_map>

#include "Chunk2.hpp"
#include "Block.hpp"
#include "GLMinclude.hpp"
#include "stb_image.h"

extern GLuint programID;

using namespace std;
using namespace glm;

const char* WORLD_TEXTURE = "content/terrain.png";

GLuint* LoadWorldTexture() {
    int width, height;
    unsigned char *data = stbi_load(WORLD_TEXTURE, &width, &height, nullptr, 0);
    if (!data) {
        printf("%s could not be opened. Are you in the right directory? \n", WORLD_TEXTURE);
        getchar();
        return 0;
    }

    GLuint* textureID;
    glGenTextures(1, textureID);
    glBindTexture(GL_TEXTURE_2D, *textureID);

    // Base level
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // Set up texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);

    stbi_image_free(data);

    return textureID;
};

class World {
private:
    unsigned long seed;
    unsigned short renderDistance;

    mutex chunkgenMutex;
    thread chunkgenThread;
    bool generateChunks;
    unordered_map<vec2, Chunk> chunks;

    GLuint* Texture;

    Block* block;

public:
    World();
    World(unsigned long s);
    World(unsigned long s, unsigned short rendist);
    World(unsigned long s, unsigned short rendist, GLuint* tex);

    void StartGenThread();
    void StopGenThread();
    void Generate();

    //temp
    void DrawBlock();
    void DrawChunk();

    // TODO: Move to Biome Generator
    bool GenerateChunk();
    bool GetChunk();
};

World::World() {
    World(9999);
}

World::World(unsigned long s) {
    World(s, 32);
}

World::World(unsigned long s, unsigned short rendist) {
    World(s, rendist, LoadWorldTexture());
}

World::World(unsigned long s, unsigned short rendist, GLuint* tex) : seed(s), renderDistance(rendist), Texture(tex) {}

void World::StartGenThread() {
    this->generateChunks = 1;
    this->chunkgenThread = thread(Generate);
}

void World::StopGenThread() {
    this->generateChunks = 0;
    this->chunkgenThread.join();
}

void World::Generate() {
    #ifdef MCDEBUG
    cout << "Generation Thread Running\n";
    #endif

    vector<thread> chunkThreads; 

    //Chunk Generation Loop
    while(this->generateChunks) {
        this->chunkgenMutex.lock();
        for(auto& pair : chunks) {
            if (pair.second.IsGenerated()) continue;
            //generate chunk with more threads :D
        }

        for(int i = chunkThreads.size()-1; i >= 0; i--) {
            chunkThreads.at(i).join();
            chunkThreads.pop_back();
        }

        this->chunkgenMutex.unlock();
    }

    #ifdef MCDEBUG
    cout << "Generation loop broke\n"
    #endif
}

void World::DrawBlock() {
    this->block = new Block();
    this->block->Draw(63);
}

#endif