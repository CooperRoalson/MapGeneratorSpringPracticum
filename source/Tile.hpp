//
//  Tile.hpp
//  Map Generator
//

#ifndef Tile_hpp
#define Tile_hpp

#include "Dictionary.hpp"

#include <SFML/Graphics.hpp>

class Tile {

    private:
        Dictionary<std::string, double> attributes;
        LinkedList<std::string> features;
        sf::Color colorCache;
        bool needToRenderColor = true;
        
        /*double val;
        Tile::color col;
        sf::Color col2;*/
    
    public:
    
        Tile();
        Tile(double elevation, double temp, double humidity);
        
        void renderColor();
        sf::Color getColor();
        
        int getBiome();
    
        double getAttribute(std::string attr);
        void setAttribute(std::string attr, double val);
        
        void addFeature(std::string feat);
        void removeFeature(std::string feat);
        bool hasFeature(std::string feat);
    
};

#endif /* Tile_hpp */
