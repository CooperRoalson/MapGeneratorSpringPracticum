//
//  TileMap.cpp
//  Map Generator
//

#include "TileMap.hpp"

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
    
    generateMap(seed);
    
}

void TileMap::generateMap(unsigned int seed) {
    std::mt19937 rand(seed);
    
    generateTileAttributes(&rand);
    //generateMountains(&rand);
}

void TileMap::generateTileAttributes(std::mt19937* rand) {
    // Create generators
    PerlinNoiseGenerator** elevationGenerators = getGeneratorList(rand);
    PerlinNoiseGenerator** temperatureGenerators = getGeneratorList(rand);
    PerlinNoiseGenerator** humidityGenerators = getGeneratorList(rand);
    
    
    double attributeScale = (1 - 1/settings.octaveScale)/(1 - pow(settings.octaveScale,-1*settings.perlinOctaves));
    
    
    // Assign to tiles
    for (int x = 0; x < settings.width; x++) {
        for (int y = 0; y < settings.height; y++) {
            updateTileAttributes(x, y, "elevation", elevationGenerators, attributeScale);
            updateTileAttributes(x, y, "temperature", temperatureGenerators, attributeScale);
            updateTileAttributes(x, y, "humidity", humidityGenerators, attributeScale);
        }
    }
    
    for (int i = 0; i < settings.perlinOctaves; i++) {
        delete elevationGenerators[i];
        delete temperatureGenerators[i];
        delete humidityGenerators[i];
    }
    delete[] elevationGenerators;
    delete[] temperatureGenerators;
    delete[] humidityGenerators;
    
}

PerlinNoiseGenerator** TileMap::getGeneratorList(std::mt19937* rand) {
    PerlinNoiseGenerator** generators = new PerlinNoiseGenerator*[settings.perlinOctaves];
    
    double size = 1.0/settings.largestOctave;
    for (int i = 0; i < settings.perlinOctaves; i++) {
        generators[i] = new PerlinNoiseGenerator(rand, ceil(settings.width*size), ceil(settings.height*size));
        size *= settings.octaveScale;
    }
    
    return generators;
    
}

void TileMap::updateTileAttributes(int x, int y, std::string attr, PerlinNoiseGenerator** gens, double attrScale) {
    Tile* t = getTile(x, y);
    
    int octaveScale = 1;
    for (int i = 0; i < settings.perlinOctaves; i++) {
        t->setAttribute(attr,
            t->getAttribute(attr) +
                gens[i]->noise(
                    (double)(x * octaveScale)/settings.largestOctave,
                    (double)(y * octaveScale)/settings.largestOctave
                )/octaveScale
        );
        
        octaveScale *= settings.octaveScale;
    }
    
    
    t->setAttribute(attr, t->getAttribute(attr) * attrScale); // Range [0,1]
}

void TileMap::generateMountains(std::mt19937* rand) {
    PerlinNoiseGenerator perlin(rand, ceil(settings.width/settings.mountainPerlinScale), ceil(settings.height/settings.mountainPerlinScale));
    
    std::uniform_real_distribution<double> mtnDist(0,1);
    std::uniform_real_distribution<double> mtnHeight(settings.mountainMinHeight,settings.mountainMaxHeight);
    
    Tile* t;
    for (int x = 0; x < settings.width; x++) {
        for (int y = 0; y < settings.height; y++) {
            if(perlin.noise((double)x/settings.mountainPerlinScale, (double)y/settings.mountainPerlinScale) >= settings.mountainThreshold && mtnDist(rand) < settings.mountainChance) {
            
                t = getTile(x, y);
                t->addFeature("mountain");
                t->setAttribute("elevation", mtnHeight(rand));
                
            }
        }
    }
    
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
