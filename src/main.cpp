#include <iostream>

#include "headers/Client.hpp"
#include "headers/controls.hpp"
#include "headers/GLMinclude.hpp"
#include "headers/shader.hpp"
#include "headers/World2.hpp"

#define MCDEBUG

const int width = 1920, height = 1080;
GLFWwindow* window;
GLuint* programID;

int main() {
    // Generate World Seed & Create World
    uint64_t seed = time(NULL);
    World world(seed, 32);

    // Create and Initialize Client
    Client client(&world);
    cout << "Am running\n";
    char a = 'a';

    cin >> a;
    cout << a << "test\n";
    // if(client.Init()) {
    //     cout << "Failed to Initialize Client\n";
    // }

    return 0;
}