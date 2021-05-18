//
//  TileMap.hpp
//  Map Generator
//

#ifndef TileMap_hpp
#define TileMap_hpp

#include <iostream>
#include <random>

#include "Tile.hpp"
#include "PerlinNoise.hpp"


class TileMap {
    public:
        struct GenerationSettings {
            int width;
            int height;
            int largestOctave; // How big one perlin cell is in the biggest octave
            int perlinOctaves;
            float octaveScale;
            
            int mountainPerlinScale; // How big one perlin cell is for mountains
            float mountainThreshold; // What's the perlin threshold for generating mountains? [0-1]
            float mountainChance; // What's the chance of a mountain past that threshold
            float mountainMinHeight;
            float mountainMaxHeight;
        };
    
    public:
        TileMap(GenerationSettings settings);
        TileMap(GenerationSettings settings, unsigned int seed);
        ~TileMap();
    
        Tile* getTile(int x, int y);
        int getWidth();
        int getHeight();
    
    private:
        Tile** tileMap;
        
        GenerationSettings settings;
    
        unsigned int randomInitialSeed();
        PerlinNoiseGenerator** getGeneratorList(std::mt19937* rand);
        void updateTileAttributes(int x, int y, std::string attr, PerlinNoiseGenerator** gens, double attrScale);
    
        void generateMap(unsigned int seed);
        void generateTileAttributes(std::mt19937* rand);
        void generateMountains(std::mt19937* rand);
        
};

#endif TileMap_hpp
