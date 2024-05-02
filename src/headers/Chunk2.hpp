#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <cstdlib>

#include "object.hpp"
#include "block_data.hpp"
#include "fast_noise.hpp"
#include "chunk_helper.hpp"

extern GLuint programID;

using namespace std;
using namespace glm;

class Chunk {
private:
    vec2 position;

public:
    Chunk(vec2 pos);


};