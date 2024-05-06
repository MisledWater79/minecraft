#pragma once
#ifndef CHUNK_H
#define CHUNK_H

#include "GLMinclude.hpp"

using namespace std;
using namespace glm;

class Chunk {
private:
    vec2 position;
    bool generated;
    
public:
    Chunk(vec2 pos);

    void Draw();

    bool IsGenerated();
};

#endif