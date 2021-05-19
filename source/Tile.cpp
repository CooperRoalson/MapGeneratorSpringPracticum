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

void Tile::renderColor(int displayMode) {
    TileMap::GenerationSettings* gs = tileMap->getSettings();
    
    double elev = attributes.get("elevation"); // Range [0,1]
    double mountainElev = clamp(elev/(gs->mountainMaxHeight), 0, 1);
    
    
    
    double temp = attributes.get("temperature"); // Range [0,1]
    temp *= 255;
    double hum = attributes.get("humidity"); // Range [0,1]
    hum *= 255;

    switch (displayMode) {

        case 0: // Elevation + mountains
            if (isOcean() && !hasFeature("sea_cliff")) { // Ocean
                colorCache.r = (int)(50);
                colorCache.g = (int)(75);
                colorCache.b = (int)(100 + (elev * 155 / gs->seaLevel));
            } else if (hasFeature("beach")) { // Beach
                elev *= 155;
                colorCache.r = (int)(elev + 100);
                colorCache.g = (int)(elev + 100);
                colorCache.b = 100;
            } else if (elev > gs->mountainMinHeight + (1./3.) * (gs->mountainMaxHeight-gs->mountainMinHeight)) { // Snow
                
                mountainElev *= 50;
                colorCache = sf::Color(205 + mountainElev, 205 + mountainElev, 205 + mountainElev);
            } else if (elev <= 1) { // Normal
                colorCache.r = (int)(100);
                colorCache.g = (int)(150 + 50*colorCurve(elev, 100));
                colorCache.b = (int)(100);
            } else { // Foothills
                double extraPercent = (elev - 1) / (gs->mountainMaxHeight - 1);
                colorCache.r = (int)(100 + extraPercent * 100);
                colorCache.g = (int)(clamp(150 + 50*exp(-10*extraPercent), colorCache.r, 255)); // The clamp is to prevent purples
                colorCache.b = (int)(100 + extraPercent*100);
                if (hasFeature("sea_cliff")) {
                    colorCache.r = (int)(clamp((30-30*elev) + 100 + extraPercent * 100, colorCache.g, 255));
                }
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

sf::Color Tile::getColor(int displayMode) {
    if (needToRenderColor) {renderColor(displayMode); needToRenderColor = false;}
    return colorCache;
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

bool Tile::noFeatures() {
    return features.isEmpty();
}

double Tile::clamp(double x, double a, double b) {
    return fmin(fmax(x, a), b);
}

double Tile::colorCurve(double input, double steepness) {
    double seaLevel = tileMap->getSettings()->seaLevel;
    return log(steepness*(input-seaLevel)+1) / log(steepness+1-steepness*seaLevel);
}

bool Tile::isOcean() {
    return getAttribute("elevation") < tileMap->getSettings()->seaLevel;
}
