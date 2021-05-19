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
    
    /*
    if (hasFeature("mountain")) {
        colorCache = sf::Color(100, 100, 150);
        return;
    } else if (hasFeature("foothill")) {
        colorCache = sf::Color(100, 100, 100);
        /*double elev = attributes.get("elevation"); // Range [0,mtnMax]
        elev /= gs->mountainMaxHeight * gs->mountainDistributionHigh; // Range [0,1]
        double standard = 1 / gs->mountainMaxHeight; // A normal tile's range would be [0,standard] at this point (used for smooth color blend)
        
        colorCache.r = (int)(elev*75) + 100;
        colorCache.g = ((int)((standard-elev)) + 255); // Plugging in standard should give 255 for blend
        colorCache.b = (int)(elev*75) + 75;
        return;
    }
    */
    double elev = attributes.get("elevation"); // Range [0,2]
    elev *= 78;
    double temp = attributes.get("temperature"); // Range [0,1]
    temp *= 255;
    double hum = attributes.get("humidity"); // Range [0,1]
    hum *= 255;

    int colorMode = tileMap->getColorMode();

    if (colorMode == 0) {
        colorCache.r = (int)(100);
        colorCache.g = (int)(elev + 100);
        colorCache.b = (int)(100);
    }
    else if (colorMode == 1) {
        if (elev < 78) {
            colorCache.r = (int)(100);
            colorCache.g = (int)(elev + 100);
            colorCache.b = (int)(100);
        }
        else {
            colorCache.r = (int)(255 - elev);
            colorCache.g = (int)(255 - elev);
            colorCache.b = (int)(255 - elev);
        }
    }
    else if (colorMode == 2) {
        colorCache.r = (int)(temp);
        colorCache.g = (int)(0);
        colorCache.b = (int)(155 - (temp/255)*155);
    }
    else if (colorMode == 3) {
        colorCache.r = (int)(0);
        colorCache.g = (int)(0);
        colorCache.b = (int)(hum);
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
