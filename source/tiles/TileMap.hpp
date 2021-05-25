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
            
          // Tile generation
            int width;
            int height;
            float largestOctave; // How big one perlin cell is in the biggest octave
            int perlinOctaves;
            float octaveScale;
            bool mountainType;
            
          
          // Mountain generation
            float mountainMinHeight;
            float mountainMaxHeight;
            float snowMultiplier;

            //New Settings
            float mountainRangeChancePerTile;
            int mountainRangeLengthLow;
            int mountainRangeLengthHigh;
            float mountainRangeDistanceLow;
            float mountainRangeDistanceHigh;
            float mountainRangeRandomOffset;
            float mountainRangeMaxAngleChange; 
            float mountainRangeShrink;

            //Old Settings
            float mountainPerlinScale; // How big one perlin cell is for mountains
            float mountainThreshold; // What's the perlin threshold for generating mountains? (smaller value = rarer)
            float mountainChance; // What's the chance of a mountain in a tile within that threshold (smaller value = rarer)

            
          // Mountain smoothing
            float mountainSmoothThreshold;
            int mountainSmoothPasses;
            float mountainDistributionLow; // How much to smooth (min and max)
            float mountainDistributionHigh;

          // Ocean settings
            float seaLevel;
            bool canMountainsFormInOcean;
            float beachThreashold;
            float oceanHumidityMultiplier;
            float seafloorThreshold;
            
          // Humidity smoothing
            float humiditySmoothThreshold;
            int humiditySmoothPasses;
            float humidityDistributionLow; // How much to smooth (min and max)
            float humidityDistributionHigh;

          // Forest generation
            float forestPerlinScale; // How big one perlin cell is for forests
            float forestChance; // What's the chance of a forest in a tile within that threshold (smaller value = rarer)
            float forestPerlinWeight; // How much does the perlin noise matter?
            float forestHumidityWeight; // How much does it value humidity?
        };
    
    public:
        TileMap(GenerationSettings settings);
        TileMap(GenerationSettings settings, unsigned int seed);
        ~TileMap();

        unsigned int  getSeed() { return seed; };
    
        const Tile* getTile(int x, int y) const;
        Tile* getTile(int x, int y);
        int getWidth() const;
        int getHeight() const;
        GenerationSettings* getSettings();

        void rerenderTiles(int displayMode);
    
    private:
        Tile** tileMap;
        
        GenerationSettings settings;

        unsigned int seed;
    
        unsigned int randomInitialSeed();
        PerlinNoiseGenerator** getGeneratorList(std::mt19937* rand);
        void updateTileAttributes(int x, int y, std::string attr, PerlinNoiseGenerator** gens, double attrScale);
    
        void generateMap(unsigned int seed);
        void generateTileAttributes(std::mt19937* rand);
        void generateMountains(std::mt19937* rand);
        void generateMountainRange(std::mt19937* rand, int xStart, int yStart);
        bool inBounds(int x, int y);
        void generateMountainsOld(std::mt19937* rand);
        void smoothMountains(std::mt19937* rand);
        void makeSeaCliffs(std::mt19937* rand);
        void designateBeaches();
        void makeSeaWet();
        void smoothHumidity(std::mt19937* rand);
        void generateForests(std::mt19937* rand);
    
        void getTileSurroundingMaxAndMin(double* buffer, int x, int y, bool includeSeaTiles, float& max, float& min);
        
};

#endif /*TileMap_hpp*/
