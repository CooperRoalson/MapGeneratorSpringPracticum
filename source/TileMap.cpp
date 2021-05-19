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
        tileMap[i] = new Tile(this);
    }
    
    generateMap(seed);
    
}

void TileMap::generateMap(unsigned int seed) {
    std::cout << "Generating new map with seed " << std::to_string(seed) << "\n";
    std::mt19937 rand(seed);
    
    generateTileAttributes(&rand);
    std::cout << "Base tile map generated\n";
    
    generateMountains(&rand);
    std::cout << "Mountains generated\n";
    
    std::cout << "Smoothing mountains\n";
    for (int i = 0; i < settings.mountainSmoothPasses; i++) {
        smoothMountains(&rand);
        std::cout << "> " << i+1 << " smooth passes completed\n";
    }
    std::cout << "Smoothing complete\n";

    if (!settings.canMountainsFormInOcean) {
        std::cout << "Generating sea cliffs\n";
        makeSeaCliffs(&rand);
    }

    std::cout << "Designating beaches\n";
    designateBeaches();

    // Gives ocean tiles humidity
    std::cout << "Wetting ocean\n";
    makeSeaWet();
    
    // Essentially smooths the humidity. Uses the same settings as foothills
    std::cout << "Dispersing humidity\n";
    for (int i = 0; i < settings.humiditySmoothPasses; i++) {
        smoothHumidity(&rand);
        std::cout << "> " << i+1 << " dispersion passes completed\n";
    }
    
    generateForests(&rand);
    std::cout << "Forests generated\n";
}

void TileMap::rerenderTiles(int displayMode) {
    for (int x = 0; x < settings.width; x++) {
        for (int y = 0; y < settings.height; y++) {
            getTile(x, y)->renderColor(displayMode);
        }
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
    std::uniform_real_distribution<double> mtnHeight(settings.mountainMinHeight - 1,settings.mountainMaxHeight - 1);
    
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
                    if (t->getAttribute("elevation") > settings.seaLevel || settings.canMountainsFormInOcean) {
                        t->addFeature("mountain");
                        t->setAttribute("elevation", t->getAttribute("elevation") + mtnHeight(*rand));
                    }
                }
            }
        }
    }
}

void TileMap::getTileSurroundingMaxAndMin(double* buffer, int x, int y, bool includeSeaTiles, float& max, float& min) {
    min = settings.mountainMaxHeight;
    max = 0;
    for (int xmod = -1; xmod <= 1; xmod++) {
        for (int ymod = -1; ymod <= 1; ymod++) {
            int currentx = xmod + x;
            int currenty = ymod + y;
            if (currentx >= 0 && currenty >= 0 && currentx < settings.width && currenty < settings.height) { //If tile in bounds of map
                double tempElevation = buffer[currenty * settings.width + currentx];
                
                if (includeSeaTiles || tempElevation >= settings.seaLevel) {
                    if (tempElevation > max) {
                        max = tempElevation;
                    }
                    if (tempElevation < min) {
                        min = tempElevation;
                    }
                }
            }
        }
    }
}

void TileMap::smoothMountains(std::mt19937* rand) {
    double* heightBuffer = new double[settings.width * settings.height]; //Needed so that modified data does not interfere with currently generating data
    for (int y = 0; y < settings.height; y++) {
        for (int x = 0; x < settings.width; x++) {
            heightBuffer[y * settings.width + x] = getTile(x, y)->getAttribute("elevation");
        }
    }
    std::uniform_real_distribution<double> foothillDistribution(settings.mountainDistributionLow, settings.mountainDistributionHigh);
    for (int x = 0; x < settings.width; x++) {
        for (int y = 0; y < settings.height; y++) {
            Tile* currentTile = getTile(x, y);
            float min, max;

            // Do not perform modifications on tiles with the mountain attribute
            if (!(currentTile->hasFeature("mountain"))){// && !(currentTile->hasFeature("foothill"))) {
                
                // Finds the minimum and maximum heights of the adjacent 8 tiles and determines the lowest and greatest elevation
                getTileSurroundingMaxAndMin(heightBuffer, x, y, settings.canMountainsFormInOcean, max, min);
                
                // If a modification is necessary smooth out the tile
                if (max / min >= settings.mountainSmoothThreshold && (settings.canMountainsFormInOcean || !(currentTile->isOcean()))) {
                    currentTile->addFeature("foothill");
                    currentTile->setAttribute("elevation", foothillDistribution(*rand) * (max - min) + min);
                }
                
            }
        }
    }
    
    delete[] heightBuffer;
}

void TileMap::makeSeaCliffs(std::mt19937* rand) {
    double* heightBuffer = new double[settings.width * settings.height]; //Needed so that modified data does not interfere with currently generating data
    for (int y = 0; y < settings.height; y++) {
        for (int x = 0; x < settings.width; x++) {
            heightBuffer[y * settings.width + x] = getTile(x, y)->getAttribute("elevation");
        }
    }
    std::uniform_real_distribution<double> foothillDistribution(settings.mountainDistributionLow, settings.mountainDistributionHigh);
    for (int x = 0; x < settings.width; x++) {
        for (int y = 0; y < settings.height; y++) {
            Tile* currentTile = getTile(x, y);
            float min, max;
            
            if (currentTile->isOcean()) {
            
                // Finds the minimum and maximum heights of the adjacent 8 tiles and determines the lowest and greatest elevation
                getTileSurroundingMaxAndMin(heightBuffer, x, y, true, max, min);

                // If a it is a valid sea tile, and near mountains
                if (max >= 1) {
                    currentTile->addFeature("sea_cliff");
                    currentTile->setAttribute("elevation", foothillDistribution(*rand) * (max - min) + min);
                }
            }
        }
    }

    delete[] heightBuffer;
}

void TileMap::designateBeaches() {
    for (int x = 0; x < settings.width; x++) {
        for (int y = 0; y < settings.height; y++) {
            Tile* currentTile = getTile(x, y);

            if (currentTile->getAttribute("elevation") >= settings.seaLevel && settings.seaLevel + settings.beachThreashold > currentTile->getAttribute("elevation")) {
                currentTile->addFeature("beach");
            }
        }
    }
}

void TileMap::makeSeaWet() {
    for (int x = 0; x < settings.width; x++) {
        for (int y = 0; y < settings.height; y++) {
            Tile* currentTile = getTile(x, y);

            if (currentTile->getAttribute("elevation") < settings.seaLevel) {
                currentTile->addFeature("sea");
                currentTile->setAttribute("humidity", fmin(1, settings.oceanHumidityMultiplier * currentTile->getAttribute("humidity")));
            }
        }
    }
}

void TileMap::smoothHumidity(std::mt19937* rand) {
    double* humidityBuffer = new double[settings.width * settings.height]; //Needed so that modified data does not interfere with currently generating data
    for (int y = 0; y < settings.height; y++) {
        for (int x = 0; x < settings.width; x++) {
            humidityBuffer[y * settings.width + x] = getTile(x, y)->getAttribute("humidity");
        }
    }
    std::uniform_real_distribution<double> distribution(settings.humidityDistributionLow, settings.humidityDistributionHigh);
    for (int x = 0; x < settings.width; x++) {
        for (int y = 0; y < settings.height; y++) {
            Tile* currentTile = getTile(x, y);
            float min, max;

            if (!(currentTile->isOcean()) && !(currentTile->hasFeature("mountain") || currentTile->hasFeature("foothill"))) {
                
                // Finds the minimum and maximum heights of the adjacent 8 tiles and determines the lowest and greatest elevation
                getTileSurroundingMaxAndMin(humidityBuffer, x, y, currentTile->hasFeature("beach"), max, min);
                
                // If a modification is necessary smooth out the tile
                if (max / min >= settings.humiditySmoothThreshold) {
                    currentTile->setAttribute("humidity", distribution(*rand) * (max - min) + min);
                }
                
            }
        }
    }
    
    delete[] humidityBuffer;
}

void TileMap::generateForests(std::mt19937* rand) {
    PerlinNoiseGenerator perlin(rand, ceil(settings.width/settings.forestPerlinScale), ceil(settings.height/settings.forestPerlinScale));
    
    std::uniform_real_distribution<double> forestDist(0,1);
    
    Tile* t;
    double perlinVal, randVal, chance, humidityChance;
    for (int x = 0; x < settings.width; x++) {
        for (int y = 0; y < settings.height; y++) {
            
            t = getTile(x, y);
            if (!(t->isOcean()) && t->noFeatures()) {

                perlinVal = perlin.noise((double)x/settings.forestPerlinScale, (double)y/settings.forestPerlinScale);

                //if(perlinVal < settings.forestThreshold) {
                                        
                    chance = pow(perlinVal,settings.forestPerlinWeight) * settings.forestChance * pow(t->getAttribute("humidity"), settings.forestHumidityWeight);
                    randVal = forestDist(*rand);
                    if (randVal < chance) {
                        t->addFeature("forest");
                    }
                //}
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

TileMap::GenerationSettings* TileMap::getSettings() {
    return &settings;
}
