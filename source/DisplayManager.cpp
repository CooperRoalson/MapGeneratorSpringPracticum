//
//  DisplayManager.cpp
//  Map Generator
//

#include "DisplayManager.hpp"

#include "exe_path.hpp"

#include <iostream>
#include <string>

DisplayManager::DisplayManager(int width, int height, double initialTileSize, TileMap* _tileMap, std::string _resource_dir)
    : window(sf::VideoMode(width, height), "Map Generator") {
    
        tileMap = _tileMap;
        
    resourceDir = _resource_dir;
    if (resourceDir == "") {
        resourceDir = ExePath::getExecutableDir() + "/" + "resources/";
    }
    
    std::cout << "Using resource directory: " << resourceDir << "\n";
        
        xOffset = 0;
        yOffset = 0;
        tileSize = initialTileSize;
        
}

void DisplayManager::display() {
    window.clear();
    draw();
    window.display();
}

void DisplayManager::draw() {
    /*sf::RectangleShape rect;
    rect.setPosition(50,50);
    rect.setSize(sf::Vector2f(40,40));
    rect.setFillColor(sf::Color(100,100,100));
    rect.setOutlineColor(sf::Color(255,255,255));
    window.draw(rect);*/
    
    /*int x = 0; int y = 0;
    Tile* t = tileMap->getTile(x, y);
                
    sf::RectangleShape rect;
    rect.setPosition(x*tileSize, y*tileSize);
    rect.setSize(sf::Vector2f(tileSize, tileSize));
    Tile::color col = t->getColor();
    rect.setFillColor(sf::Color(col.r, col.g, col.b));
    
    window.draw(rect);*/
    
    for (int y = 0; y < tileMap->getHeight(); y++) {
        for (int x = 0; x < tileMap->getWidth(); x++) {
            
            Tile* t = tileMap->getTile(x, y);
                        
            sf::RectangleShape rect;
            rect.setPosition(x*tileSize, y*tileSize);
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

