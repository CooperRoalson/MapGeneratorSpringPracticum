//
//  PerlinNoise.cpp
//  Map Generator
//

#include "PerlinNoise.hpp"

#include <random>
// below line required for windows
#define _USE_MATH_DEFINES
#include <math.h>

#include <stdexcept>
#include <string>
#include <iostream>

unsigned int PerlinNoiseGenerator::randomInitialSeed() {
    std::random_device rand;
    return rand();
}

PerlinNoiseGenerator::PerlinNoiseGenerator(int width, int height) : PerlinNoiseGenerator(randomInitialSeed(), width, height) {}

PerlinNoiseGenerator::PerlinNoiseGenerator(unsigned int _seed, int width, int height) {
    
    dimensions = Vec2D<int>(width, height);

    seed = _seed;
    
    generateGradients();
    
    
}

PerlinNoiseGenerator::~PerlinNoiseGenerator() {
    delete[] gradients;
}

void PerlinNoiseGenerator::generateGradients() {
    
    // Set up gradient grid (stored in an array of length width*height)
    gradients = new Vec2D<double>[(dimensions.x+1)*(dimensions.y+1)];
    
    std::mt19937 rand(seed);
    std::uniform_real_distribution<double> dist(0, 2*M_PI);
    
    // Initialize grid to vectors of length 1
    for (int i = 0; i < (dimensions.x+1)*(dimensions.y+1); i++) {
        double angle = dist(rand);
        gradients[i].x = cos(angle);
        gradients[i].y = sin(angle);
    }
}

double PerlinNoiseGenerator::gridDotProduct(Vec2D<double> point, Vec2D<int> grid) {
        
    // Get gradient vector at grid point
    Vec2D<double> gradient = gradients[grid.y * (dimensions.x+1) + grid.x];
    
    // Get offset vector from grid point to target point
    Vec2D<double> offset(point.x - grid.x, point.y - grid.y);
    
    // Return dot product of offset vector and gradient
    return offset.x * gradient.x + offset.y * gradient.y;
    
}

double PerlinNoiseGenerator::interpolate(double a0, double a1, double w) {
    // Use the smoothstep interpolation
    return (a1 - a0) * (3. - w * 2.) * w*w + a0;
}

double PerlinNoiseGenerator::noise(double x, double y) {
    
    if ((int) x < 0 || x >= dimensions.x || (int) y < 0 || y >= dimensions.y) {
        throw std::out_of_range("Point (" + std::to_string(x) + "," + std::to_string(y) + ") out of range for noise of size (" + std::to_string(dimensions.x) + "," + std::to_string(dimensions.y) + ")");
    }
    
    Vec2D<double> point(x, y);
    
    // Get grid point coords
    int x0 = (int) x;
    int x1 = x0 + 1;
    int y0 = (int) y;
    int y1 = y0 + 1;

    double xInterp = x - x0;
    double yInterp = y - y0;

    double g0, g1, interp0, interp1, result;

    // Compute dot products
    
    g0 = gridDotProduct(point, {x0, y0});
    g1 = gridDotProduct(point, {x1, y0});
    interp0 = interpolate(g0, g1, xInterp);

    g0 = gridDotProduct(point, {x0, y1});
    g1 = gridDotProduct(point, {x1, y1});
    interp1 = interpolate(g0, g1, xInterp);
        
    // Interpolate final result
    result = interpolate(interp0, interp1, yInterp);
    
    return (result + 1) / 2; // Put in range [0, 1]
    
}
