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
        
        /*double val;
        Tile::color col;
        sf::Color col2;*/
    
    public:
    
        Tile();
        Tile(double elevation, double temp, double humidity);
        
        sf::Color getColor();
        
        int getBiome();
    
        double getAttribute(std::string attr);
        void setAttribute(std::string attr, double val);
    
};

#endif /* Tile_hpp */
