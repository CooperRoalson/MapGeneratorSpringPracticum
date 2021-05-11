//
//  TileMap.cpp
//  Map Generator
//

#include "TileMap.hpp"

#include "PerlinNoise.hpp"

#include <iostream>

#include <random>

unsigned int TileMap::randomInitialSeed() {
    std::random_device rand;
    return rand();
}

TileMap::TileMap(GenerationSettings settings) : TileMap(settings, randomInitialSeed()) {}

TileMap::TileMap(GenerationSettings _settings, unsigned int seed) {
    settings = _settings;
    tileMap = new Tile*[settings.width*settings.height];
    
    for (int i = 0; i < settings.width*settings.height; i++) {
        tileMap[i] = new Tile();
    }
    
    std::mt19937 seedGen(seed);
    std::uniform_real_distribution<unsigned int> dist(0, UINT_MAX);
    
    
    
    // Elevation
    PerlinNoiseGenerator** elevationGenerators = new PerlinNoiseGenerator*[settings.perlinOctaves];
    
    double size = settings.largestOctave;
    for (int i = 0; i < settings.perlinOctaves; i++) {
        elevationGenerators[i] = new PerlinNoiseGenerator(dist(seedGen), (int) (settings.width/size), (int)(settings.height/size));
        size /= settings.octaveScale;
    }
    
    std::cout << elevationGenerators[0]->noise(3.2,1.7) << std::endl;
    
    // Assign to tiles
    for (int x = 0; x < settings.width; x++) {
        for (int y = 0; y < settings.height; y++) {
            
            int octaveScale = 1;
            for (int i = 0; i < settings.perlinOctaves; i++) {
                
                Tile* t = getTile(x, y);
                t->setAttribute("elevation",
                    t->getAttribute("elevation") +
                        elevationGenerators[i]->noise(
                            ((double) x)/(settings.largestOctave*octaveScale),
                            ((double) y)/(settings.largestOctave*octaveScale)
                        )/octaveScale
                );
                
                octaveScale *= settings.octaveScale;
            }
            
        }
    }

    delete[] elevationGenerators;
    
}

TileMap::~TileMap() {
    for (int i = 0; i < settings.width*settings.height; i++) {
        delete tileMap[i];
    }
    delete[] tileMap;
}

Tile* TileMap::getTile(int x, int y) {
    return tileMap[y*settings.width + x];
}

int TileMap::getWidth() {
    return settings.width;
}

int TileMap::getHeight() {
    return settings.height;
}
