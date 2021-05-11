//
//  DisplayManager.hpp
//  Map Generator
//

#ifndef DisplayManager_hpp
#define DisplayManager_hpp

#include "DisplayManager.hpp"
#include "TileMap.hpp"

#include <SFML/Graphics.hpp>

class DisplayManager {
    
public:
    DisplayManager(int width, int height, double initialTileSize, TileMap* tileMap, std::string resource_dir = "");
    
    void display();
    
    void close();
    bool isOpen();
    bool pollEvent(sf::Event &event);
        
private:
    double xOffset, yOffset, tileSize;
    
    sf::RenderWindow window;
    
    TileMap* tileMap;
    
    std::string resourceDir;
    
    void draw();
    
};

#endif /* DisplayManager_hpp */
