#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <vector>
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "headers/world.hpp"
#include "headers/shader.hpp"
#include "headers/controls.hpp"
#include "headers/Player.hpp"

const int width = 1920, height = 1080;
GLFWwindow* window;
GLuint programID;

int main() {
    // Generate World Seed & Create World
    uint64_t seed = time(NULL);
    World world(seed, 32);

    // Create and Initialize Player
    Player player(&world);
    cout << "Am running\n";
    char a = 'a';

    cin >> a;
    cout << a << "test\n";
    // if(player.Init()) {
    //     cout << "Failed to Initialize Player\n";
    // }

    return 0;
}