#include "chunk_coord.hpp"

#include <typeinfo>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <thread>
#include <set>

using namespace std;

mutex chunkMutex;

Chunk blankChunk;

class World {
public:
    World(unsigned long seed, unsigned short renderDistance) {
        this->seed = seed;
        this->renderDistance = renderDistance;
        this->genChunks = false;

        // Set the noise settings
        heightMap.SetSeed(seed);
        heightMap.SetNoiseType(FastNoise::PerlinFractal);
        heightMap.SetFrequency(0.00153f);
        heightMap.SetFractalOctaves(16);

        gravel.SetSeed(seed);
        gravel.SetNoiseType(FastNoise::Cellular);
        gravel.SetFrequency(0.03f);
        gravel.SetFractalOctaves(6);
        gravel.SetFractalLacunarity(1.86f);
        gravel.SetFractalGain(3.0f);
        gravel.SetCellularReturnType(FastNoise::Distance2Add);

        dirt.SetSeed(seed + 1);
        dirt.SetNoiseType(FastNoise::Cellular);
        dirt.SetFrequency(0.03f);
        dirt.SetFractalOctaves(6);
        dirt.SetFractalLacunarity(1.86f);
        dirt.SetFractalGain(3.0f);
        dirt.SetCellularReturnType(FastNoise::Distance2Add);
        

        // Generate the chunks
        //GenerateChunks();
    }

    void GenerateChunks() {
        unique_lock<mutex> lock(chunkMutex);
        int halfRenderDistance = renderDistance / 2;

        // Add to generation vector
        for (int i = 0, k = 0; i < renderDistance; i++) {
            for (int j = 0; j < renderDistance; j++) {
                ChunkCoord chunkPos = {i - halfRenderDistance, j - halfRenderDistance};
                Chunk& newChunk = chunks[chunkPos];
                if(newChunk.generated) continue;
                newChunk.chunkPos = {i - halfRenderDistance, j - halfRenderDistance};
                printf("Add Chunk To Gen: %d, %d\n", chunkPos.x, chunkPos.z);
                chunks[chunkPos] = std::move(newChunk);
                chunksToCreate.push_back(chunkPos);
            }
        }

        lock.unlock();
    }

    void UpdateChunks(vec3 playerPos) {
        unique_lock<mutex> lock(chunkMutex);

        // Calculate player's chunk position
        int playerChunkX = static_cast<int>(playerPos.x);
        int playerChunkZ = static_cast<int>(playerPos.z);
        int halfRenderDistance = renderDistance / 2;

        std::set<ChunkCoord> existingChunks;
        for (auto& pair : chunks) {
            existingChunks.insert(pair.first);
        }

        std::vector<ChunkCoord> chunksToRemove;

        // Determine new chunks and chunks to remove
        for (int i = -halfRenderDistance; i <= halfRenderDistance; i++) {
            for (int j = -halfRenderDistance; j <= halfRenderDistance; j++) {
                ChunkCoord searchCoord = {i + playerChunkX, j + playerChunkZ};

                if (existingChunks.find(searchCoord) == existingChunks.end()) {
                    // If not found, it needs to be created
                    chunksToCreate.push_back(searchCoord);
                }
                existingChunks.erase(searchCoord); // Mark this chunk as active
            }
        }

        // Any remaining coordinates in existingChunks are now too far and should be removed
        chunksToRemove.assign(existingChunks.begin(), existingChunks.end());

        // Remove old chunks
        for (auto& coord : chunksToRemove) {
            chunks.erase(coord);
        }

        lock.unlock();
    }
    
    void startGenThread() {
        genChunks = true;
        chunkGen = thread([this]{Generate();});
    }

    void stopGenThread() {
        genChunks = false;
        chunkGen.join();
    }

    void Generate() {
        while(this->genChunks) {
            if(chunksToCreate.size() <= 0) continue;
            printf("Gen Count: %d\n", chunksToCreate.size());
            
            unique_lock<mutex> lock(chunkMutex);

            vector<thread> threads(chunksToCreate.size());

            // Create new chunks
            int i = 0;
            for (auto& coord : chunksToCreate) {
                if(chunks[coord].generated) continue;
                threads[i] = thread([&]{ chunks[coord].Generate(heightMap, gravel, dirt); });
                printf("Generating: %d, %d\n", coord.x, coord.z);
                i++;
            }

            for (int i = 0; i < threads.size(); i++) {
                threads[i].join();
                cout << "Chunk done " << (int) i << "\n";
            }

            for (auto& chunkPos : chunksToCreate) {
                if(!chunks[chunkPos].generated) continue;
                printf("VBO: %d, %d\n", chunkPos.x, chunkPos.z);

                chunks[chunkPos].MakeVertexObject(
                    (chunks.count({chunkPos.x-1, chunkPos.z})) ? blankChunk : chunks[{chunkPos.x - 1, chunkPos.z}],
                    (chunks.count({chunkPos.x+1, chunkPos.z})) ? blankChunk : chunks[{chunkPos.x + 1, chunkPos.z}],
                    (chunks.count({chunkPos.x, chunkPos.z-1})) ? blankChunk : chunks[{chunkPos.x, chunkPos.z - 1}],
                    (chunks.count({chunkPos.x, chunkPos.z+1})) ? blankChunk : chunks[{chunkPos.x, chunkPos.z + 1}]
                );
                chunks[chunkPos].CreateObject();
                //chunks[chunkPos].Cleanup();
            }

            cout << "Finished Meshing Terrain" << endl;

            chunksToCreate.clear();
            lock.unlock();
        }
    }

    void changeRenderDistance(unsigned short newRenderDistance) {
        renderDistance = newRenderDistance;
        GenerateChunks();
    }

private:
    // World Settings
    uint64_t seed;

    // User Settings
    unsigned short renderDistance;
    std::vector<ChunkCoord> chunksToCreate;

    // Noise
    FastNoise heightMap;
    FastNoise gravel;
    FastNoise dirt;

    // MultiThreading
    thread chunkGen;
    bool genChunks;
};
