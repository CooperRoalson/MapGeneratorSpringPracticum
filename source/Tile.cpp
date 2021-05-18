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

sf::Color Tile::getColor() {
    double val = attributes.get("elevation"); // Range [0,1]
    //val = (3. - val * 2.) * val*val; // Alternate color mapping (smoothstep)
    
    val *= 255;
    
    return sf::Color(val, val, val);
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
