#include <iostream>
#include <format>
#include <string>
#include <vector>
#include <cstdint>
#include <random>

float rand_zero_one() {
    return static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
}

int main(int argc, char* argv[]) {
    float f = 1.000;
    for (int i = 0; i < 2000; i++) {
        auto X = rand_zero_one();
        auto Y = rand_zero_one();

        f += X;
        f += Y;
        f -= X;
        f -= Y;

        std::cout << std::format("Loop #{}: F: {}; X: {}; Y: {}\n", i, f, X, Y);
    }
    return 0;
}