#ifndef WORLD_H
#define WORLD_H

#include <typeinfo>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <set>

#include "chunk_coord.hpp"

using namespace std;

mutex chunkMutex;
condition_variable chunkCondition;

Chunk blankChunk;

int totalDeleted = 0;

class World {
private:
    unsigned long seed;
    unsigned short renderDistance;

public:
    World();
    World(unsigned long s);
    World(unsigned long s, unsigned short rendist);

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

World::World(unsigned long s, unsigned short rendist) : seed(s), renderDistance(rendist) {}



#endif