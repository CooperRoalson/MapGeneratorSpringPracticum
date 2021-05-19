//
//  Tile.cpp
//  Map Generator
//

#include "Tile.hpp"
#include "TileMap.hpp"

#include <iostream>
#include <math.h>

Tile::Tile(TileMap* parentMap) {
    tileMap = parentMap;
    
    attributes.add("elevation", 0);
    attributes.add("temperature", 0);
    attributes.add("humidity", 0);
}

void Tile::renderColor() {
    TileMap::GenerationSettings* gs = tileMap->getSettings();
    
    double elev = attributes.get("elevation"); // Range [0,1]
    double mountainElev = clamp(elev/(gs->mountainMaxHeight), 0, 1);
    
    
    
    double temp = attributes.get("temperature"); // Range [0,1]
    temp *= 255;
    double hum = attributes.get("humidity"); // Range [0,1]
    hum *= 255;

    int colorMode = tileMap->getColorMode();

    switch (colorMode) {

        case 0: // Elevation + mountains
            if (elev < gs->seaLevel) {
                colorCache.r = (int)(50);
                colorCache.g = (int)(75);
                colorCache.b = (int)(100 + (elev * 155 / gs->seaLevel));
            }
            else if (hasFeature("mountain")) {
                mountainElev *= 50;
                colorCache = sf::Color(205 + mountainElev, 205 + mountainElev, 205 + mountainElev);
            }
            else if (hasFeature("foothill") || hasFeature("sea_cliff")) {
                if (elev <= 1) {
                    if (hasFeature("sea_cliff"))
                        colorCache.r = (int)(10 * elev + 100);
                    else
                        colorCache.r = (int)(100);
                    colorCache.g = (int)(130*elev + 100);
                    colorCache.b = (int)(100);
                } else {
                    double extraPercent = (elev - 1) / (gs->mountainMaxHeight - 1);
                    if (hasFeature("sea_cliff"))
                        colorCache.r = (int)(clamp((30-30*elev) + 100 + extraPercent * 100, colorCache.r, 255));
                    else
                        colorCache.r = (int)(100 + extraPercent * 100);
                    colorCache.g = (int)(clamp(130 + 100*exp(-10*extraPercent), colorCache.g, 255)); // The clamp is to prevent purples
                    colorCache.b = (int)(100 + extraPercent*100);
                }
            }
            else {
                elev *= 130;
                colorCache.r = (int)(100);
                colorCache.g = (int)(elev + 100);
                colorCache.b = (int)(100);
            }
            break;
        
        case 1: // Elevation
            elev = clamp(elev*155/gs->mountainMaxHeight, 0, 155);
            colorCache.r = (int)(100);
            colorCache.g = (int)(elev + 100);
            colorCache.b = (int)(100);
            break;
        
        case 2: // Temp
            colorCache.r = (int)(temp);
            colorCache.g = (int)(0);
            colorCache.b = (int)(155 - (temp/255)*155);
            break;
        
        case 3: // Humidity
            colorCache.r = (int)(0);
            colorCache.g = (int)(0);
            colorCache.b = (int)(hum);
            break;
        
        default:
            colorCache = sf::Color(0,0,0);
            break;
    }
}

sf::Color Tile::getColor() {
    if (needToRenderColor) {renderColor(); needToRenderColor = false;}
    return colorCache;
}

int Tile::getBiome() {
    return 0; // 0 for now
}

double Tile::getAttribute(std::string attr) {
    return attributes.get(attr);
}

void Tile::setAttribute(std::string attr, double val) {
    attributes.set(attr, val);
}

void Tile::addFeature(std::string feat) {
    features.add(feat);
}

void Tile::removeFeature(std::string feat) {
    features.removeFirst(feat);
}

bool Tile::hasFeature(std::string feat) {
    return features.findFirst(feat) != -1;
}

double Tile::clamp(double x, double a, double b) {
    return fmin(fmax(x, a), b);
}
