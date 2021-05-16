//
//  DisplayManager.cpp
//  Map Generator
//

#include "DisplayManager.hpp"

#include "exe_path.hpp"

#include <iostream>
#include <string>

DisplayManager::DisplayManager(DisplaySettings settings, TileMap* _tileMap, std::string _resource_dir)
    : window(sf::VideoMode(settings.screenWidth, settings.screenHeight), "Map Generator") {
        displaySettings = settings;
        
        tileMap = _tileMap;
        
    resourceDir = _resource_dir;
    if (resourceDir == "") {
        resourceDir = ExePath::getExecutableDir() + "/" + "resources/";
    }
    
    std::cout << "Using resource directory: " << resourceDir << "\n";
        
        xOffset = settings.initialXOffset;
        yOffset = settings.initialYOffset;
        tileSize = settings.initialTileSize;
        
}

void DisplayManager::display() {
    window.clear();
    draw();
    window.display();
}

void DisplayManager::draw() {
    
    int tileDisplayWidth = (int)(displaySettings.screenWidth / tileSize) + 2;
    int tileDisplayHeight = (int)(displaySettings.screenHeight / tileSize) + 2;
        
    int x, y;
    for (int y = (int)yOffset; y < tileDisplayHeight + (int)yOffset; y++) {
        for (int x = (int)xOffset; x < tileDisplayWidth + (int)xOffset; x++) {
            
            if (x < 0 || x >= tileMap->getWidth() || y < 0 || y >= tileMap->getHeight()) {
                continue;
            }
            
            Tile* t = tileMap->getTile(x, y);
                        
            sf::RectangleShape rect;
            rect.setPosition((x-xOffset)*tileSize, (y-yOffset)*tileSize);
            rect.setSize(sf::Vector2f(tileSize, tileSize));
            //sf::Color col = t->getColor();
            //std::cout << std::to_string(col.r) << std::endl;
            rect.setFillColor(t->getColor());
            
            window.draw(rect);
            
        }
    }
}



// Functions relayed from sf::RenderWindow

bool DisplayManager::isOpen() {
    return window.isOpen();
}

void DisplayManager::close() {
    window.close();
}

bool DisplayManager::pollEvent(sf::Event &event) {
    return window.pollEvent(event);
}

unsigned int DisplayManager::getWindowWidth() {
    return window.getSize().x;
}
unsigned int DisplayManager::getWindowHeight() {
    return window.getSize().y;
}


void DisplayManager::changeTileSize(double delta) {
    double centerX = xOffset + displaySettings.screenWidth/(2.*tileSize);
    double centerY = yOffset + displaySettings.screenHeight/(2.*tileSize);
    
    tileSize = std::max(std::min(tileSize + delta, displaySettings.maxTileSize), displaySettings.minTileSize);
    
    xOffset = centerX - displaySettings.screenWidth/(2*tileSize);
    yOffset = centerY - displaySettings.screenHeight/(2*tileSize);
}

void DisplayManager::moveCamera(double x, double y) {
    xOffset += x;
    yOffset += y;
}
