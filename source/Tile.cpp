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
        colorCache.b = (int)(elev*75) + 75;*/
        return;
    }
                               
    double elev = attributes.get("elevation"); // Range [0,1]
    elev *= 155;

    colorCache.r = 100;
    colorCache.g = (int)elev + 100;
    colorCache.b = 100;
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
