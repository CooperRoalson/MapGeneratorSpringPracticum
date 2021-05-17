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
            int largestOctave; // How big  one perlin cell is in the biggest octave
            int perlinOctaves;
            float octaveScale;
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
        
};

#endif /* TileMap_hpp */
