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

TileMap::TileMap(GenerationSettings gs, unsigned int seed) {
    settings = gs;
    tileMap = new Tile*[settings.width*settings.height];
    
    for (int i = 0; i < settings.width*settings.height; i++) {
        tileMap[i] = new Tile();
    }
    
    generateMap(seed);
    
}

void TileMap::generateMap(unsigned int seed) {
    std::mt19937 rand(seed);
    
    generateTileAttributes(&rand);
    generateMountains(&rand);
    for (int i = 0; i < settings.mountainSmoothPasses; i++) {
        smoothMountains(&rand);
        std::cout << i << " smooth passes completed.\n";
    }
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
    double perlinVal, randVal;
    for (int x = 0; x < settings.width; x++) {
        for (int y = 0; y < settings.height; y++) {

            perlinVal = perlin.noise((double)x/settings.mountainPerlinScale, (double)y/settings.mountainPerlinScale);
            //std::cout << "perlinVal = " << perlinVal << std::endl;

            if(perlinVal < settings.mountainThreshold) {
                //std::cout << "Perlin cleared for mountain" << std::endl;
                randVal = mtnDist(*rand);
                if (randVal < settings.mountainChance) {
                    //std::cout << "Random chance cleared for mountain" << std::endl;
            
                    t = getTile(x, y);
                    t->addFeature("mountain");
                    t->setAttribute("elevation", mtnHeight(*rand));
                }
            }
        }
    }
}

void TileMap::smoothMountains(std::mt19937* rand) {
    double* tileBuffer = new double[settings.width * settings.height]; //Needed so that modified data does not interfere with currently generating data
    for (int y = 0; y < settings.height; y++) {
        for (int x = 0; x < settings.width; x++) {
            tileBuffer[y * settings.width + x] = getTile(x, y)->getAttribute("elevation");
        }
    }
    std::uniform_real_distribution<double> foothillDistribution(settings.mountainDistributionLow, settings.mountainDistributionHigh);
    for (int x = 0; x < settings.width; x++) {
        for (int y = 0; y < settings.height; y++) {
            Tile* currentTile = getTile(x, y);
            float min = settings.mountainMaxHeight;
            float max = 0;

            //Finds the minimum and maximum heights of the adjacent 8 tiles and determines the lowest and greatest elevation
            if (!(currentTile->hasFeature("mountain")) && !(currentTile->hasFeature("foothill"))) {//Do not perform modifications on tiles with the mountain or foothill attributes
                for (int xmod = -1; xmod <= 1; xmod++) {
                    for (int ymod = -1; ymod <= 1; ymod++) {
                        int currentx = xmod + x;
                        int currenty = ymod + y;
                        if (currentx > -1 && currenty > -1 && currentx < settings.width && currenty < settings.height) { //If tile in bounds of map
                            double tempElevation = tileBuffer[currenty * settings.width + currentx];
                            
                            if (tempElevation > max) {
                                max = tempElevation;
                            }
                            if (tempElevation < min) {
                                min = tempElevation;
                            }
                        }
                    }
                }
                
                if (max - min >= settings.mountainSmoothThreshold) { //If a modification is necessary smooth out the tile 
                    currentTile->addFeature("foothill");
                    currentTile->setAttribute("elevation", foothillDistribution(*rand) * (max - min) + min);
                }
                
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
