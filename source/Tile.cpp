//
//  Tile.cpp
//  Map Generator
//

#include "Tile.hpp"

#include <iostream>
#include <math.h>

Tile::Tile() : Tile(0., 0., 0.) {}

Tile::Tile(double elevation, double temp, double humidity) {
    attributes.add("elevation", elevation);
    attributes.add("temperature", temp);
    attributes.add("humidity", humidity);
}

void Tile::renderColor() {
    double val = attributes.get("elevation"); // Range [0,1]
    //val = (3. - val * 2.) * val*val; // Alternate color mapping (smoothstep)

    val *= 155;

    //std::cout << val << std::endl;

    colorCache.r = (int)(100);
    colorCache.g = (int)(val + 100);
    colorCache.b = (int)(100);
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
