//
//  Tile.hpp
//  Map Generator
//

#ifndef Tile_hpp
#define Tile_hpp

#include "Dictionary.hpp"

#include <SFML/Graphics.hpp>

class TileMap; // "Forward declarations" to break circular dependency

class Tile {

    private:
        Dictionary<std::string, double> attributes;
        LinkedList<std::string> features;
        sf::Color colorCache;
        bool needToRenderColor = true;
    
        TileMap* tileMap;
    
        static double clamp(double x, double a, double b);
    
        double colorCurve(double input, double steepness);
            
        /*double val;
        Tile::color col;
        sf::Color col2;*/
    
    public:
    
        Tile(TileMap* parentMap);
        
        void renderColor(int displayMode);
        sf::Color getColor(int displayMode);
        
        bool isOcean();
    
        double getAttribute(std::string attr) const;
        void setAttribute(std::string attr, double val);
        
        void addFeature(std::string feat);
        void removeFeature(std::string feat);
        bool hasFeature(std::string feat) const;
        bool noFeatures();
    
};

#endif /* Tile_hpp */
