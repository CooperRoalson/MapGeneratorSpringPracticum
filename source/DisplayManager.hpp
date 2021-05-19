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
    
    void onClick(int clickX, int clickY);

    void setFPS(int n) { fps = n; };
    
    void setViewTile(sf::Vector2i tileCoords, sf::Vector2f screenCoords);
    void setWhetherViewingTile(bool view);
    
    sf::Vector2i getTileCoordsFromScreenCoords(int screenX, int screenY);
        
private:
    DisplayManager::DisplaySettings displaySettings;
    double xOffset, yOffset, tileSize;
    
    sf::RenderWindow window;
    sf::Font font;

    int fps;
    
    bool viewingTile = false;
    sf::Vector2i viewTileCoords;
    sf::Vector2f viewTileDisplayCoords;
    
    TileMap* tileMap;
    
    std::string resourceDir;
    
    void draw();
    void drawTiles();

    sf::RectangleShape rect;
    void drawTile(Tile* t, sf::Vector2f screenPos);
    void drawTileStats();
    void drawCoords();
    void drawControls();
    void drawDebug();
    void drawColorScheme();
    
    void loadFont();
    void loadIcon();
    
    std::string getResourcePath(std::string resource);
    
};

#endif /* DisplayManager_hpp */
