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
    
    struct DisplaySettings {
        int screenWidth, screenHeight;
        double initialXOffset, initialYOffset, initialTileSize;
        double minTileSize, maxTileSize;
    };
    
public:
    DisplayManager(DisplaySettings settings, TileMap* tileMap, std::string resource_dir);
    
    void setTileMap(TileMap* tileMap);
    
    void display();
    
    void close();
    bool isOpen();
    bool pollEvent(sf::Event &event);
    unsigned int getWindowWidth();
    unsigned int getWindowHeight();
    
    
    void changeTileSize(double delta);
    void moveCamera(double x, double y);
    
    sf::Vector2<double> getCameraCenter();
    
    double getTileSize() {return tileSize;}
        
private:
    DisplayManager::DisplaySettings displaySettings;
    double xOffset, yOffset, tileSize;
    
    sf::RenderWindow window;
    sf::Font font;
    
    TileMap* tileMap;
    
    std::string resourceDir;
    
    void draw();
    void drawTiles();
    void drawTile(Tile* t, sf::Vector2f screenPos);
    void drawCoords();
    
    void loadFont();
    void loadIcon();
    
    std::string getResourcePath(std::string resource);
    
};

#endif /* DisplayManager_hpp */
