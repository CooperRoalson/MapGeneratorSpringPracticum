//
//  TileMap.hpp
//  Map Generator
//

#ifndef TileMap_hpp
#define TileMap_hpp

#include <iostream>

#include "Tile.hpp"


struct GenerationSettings {
    int width;
    int height;
    int largestOctave; // How big  one perlin cell is in the biggest octave
    int perlinOctaves;
    float octaveScale;
};

class TileMap {
    
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
        
};

#endif /* TileMap_hpp */
