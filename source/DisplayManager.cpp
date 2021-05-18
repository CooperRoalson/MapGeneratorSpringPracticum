//
//  DisplayManager.cpp
//  Map Generator
//

#include "DisplayManager.hpp"

#include "exe_path.hpp"

#include <iostream>
#include <string>

DisplayManager::DisplayManager(DisplaySettings settings, TileMap* tm, std::string rDir)
    : window(sf::VideoMode(settings.screenWidth, settings.screenHeight), "Map Generator") {
        displaySettings = settings;
        
        tileMap = tm;
        
    resourceDir = rDir;
    if (resourceDir == "") {
        resourceDir = ExePath::mergePaths(ExePath::getExecutableDir(), "resources");
    }
    
    std::cout << "Using resource directory: " << resourceDir << "\n";
        
    xOffset = settings.initialXOffset;
    yOffset = settings.initialYOffset;
    tileSize = settings.initialTileSize;
    
    loadFont();
    loadIcon();
        
}

void DisplayManager::setTileMap(TileMap* tm) {
    tileMap = tm;
}

std::string DisplayManager::getResourcePath(std::string resource) {
    return ExePath::mergePaths(resourceDir,resource);
}

void DisplayManager::loadFont() {
    if (!font.loadFromFile(getResourcePath("font.ttf")))
    {
        throw std::runtime_error("Resource not found: " + getResourcePath("font.ttf"));
    }
}

void DisplayManager::loadIcon() {
    sf::Image icon;
    if (!icon.loadFromFile(getResourcePath("icon.png")))
    {
        throw std::runtime_error("Resource not found: " + getResourcePath("icon.png"));
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

void DisplayManager::display() {
    window.clear();
    draw();
    window.display();
}

void DisplayManager::draw() {
    drawTiles();
    drawCoords();
}

void DisplayManager::drawTiles() {

    int tileDisplayWidth = (int)(displaySettings.screenWidth / tileSize) + 2;
    int tileDisplayHeight = (int)(displaySettings.screenHeight / tileSize) + 2;
        
    int x, y;
    for (int y = (int)yOffset; y < tileDisplayHeight + (int)yOffset; y++) {
        for (int x = (int)xOffset; x < tileDisplayWidth + (int)xOffset; x++) {
            
            if (x < 0 || x >= tileMap->getWidth() || y < 0 || y >= tileMap->getHeight()) {
                continue;
            }
            
            Tile* t = tileMap->getTile(x, y);
            sf::Vector2f screenPos((x-xOffset)*tileSize, (y-yOffset)*tileSize);
                        
            drawTile(t, screenPos);
            
        }
    }
}

void DisplayManager::drawTile(Tile* t, sf::Vector2f screenPos) {
    sf::RectangleShape rect;
    rect.setPosition(screenPos);
    rect.setSize(sf::Vector2f(tileSize, tileSize));
    sf::Color color;
    
    if (t->hasFeature("mountain")) {
        color = sf::Color(50, 50, 200);
    } else {
        color = t->getColor();
    }
    
    rect.setFillColor(color);
    window.draw(rect);
}

void DisplayManager::drawCoords() {
    sf::Text coordText;
    coordText.setFont(font);
    
    sf::Vector2<double> coords = getCameraCenter();
    coordText.setString("(" + std::to_string((int)(coords.x)) + ", " + std::to_string((int)(coords.y)) + ")");

    
    coordText.setCharacterSize(40); // Pixels, not normal font size
    coordText.setFillColor(sf::Color(25,25,25)); // Color
    
    coordText.setOutlineThickness(2);
    coordText.setOutlineColor(sf::Color(200,200,200));
    
    coordText.setStyle(sf::Text::Bold);

    
    coordText.setPosition(10, 10);
    window.draw(coordText);
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

sf::Vector2<double> DisplayManager::getCameraCenter() {
    double centerX = xOffset + displaySettings.screenWidth/(2.*tileSize);
    double centerY = yOffset + displaySettings.screenHeight/(2.*tileSize);
    return sf::Vector2<double>(centerX, centerY);
}


void DisplayManager::changeTileSize(double delta) {
    sf::Vector2<double> center = getCameraCenter();
    
    tileSize = std::max(std::min(tileSize + delta, displaySettings.maxTileSize), displaySettings.minTileSize);
    
    xOffset = center.x - displaySettings.screenWidth/(2*tileSize);
    yOffset = center.y - displaySettings.screenHeight/(2*tileSize);
}

void DisplayManager::moveCamera(double x, double y) {
    xOffset += x;
    yOffset += y;
}
