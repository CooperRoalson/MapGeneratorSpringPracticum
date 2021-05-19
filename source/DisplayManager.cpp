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
    drawControls();
    drawDebug();
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
    
    rect.setPosition(screenPos);
    rect.setSize(sf::Vector2f(tileSize, tileSize));
    
    /*
    sf::Color color;
    if (t->hasFeature("mountain")) {
        color = sf::Color(100, 100, 150);
    } else if (t->hasFeature("foothill")) {
        color = sf::color(75, 75, 100);
    } else {
        color = t->getColor();
    }*/
    
    rect.setFillColor(t->getColor());
    window.draw(rect);
}

void DisplayManager::drawCoords() {
    sf::Text coordText;
    coordText.setFont(font);
    
    sf::Vector2<double> coords = getCameraCenter();
    coordText.setString("(" + std::to_string((int)(coords.x)) + ", " + std::to_string((int)(coords.y)) + ")");

    
    coordText.setCharacterSize(40); // Pixels, not normal font size
    coordText.setFillColor(sf::Color(200, 200, 200)); // Color
    
    coordText.setOutlineThickness(2);
    coordText.setOutlineColor(sf::Color(25, 25, 25));
    
    coordText.setStyle(sf::Text::Bold);

    
    coordText.setPosition(10, 10);
    window.draw(coordText);
}

void DisplayManager::drawControls() {
    sf::Text controlText;
    controlText.setFont(font);

    sf::Vector2<double> coords = getCameraCenter();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
        controlText.setString("WASD/arrows/click and drag to move. Space to regenerate terrain.");
    else
        controlText.setString("H for controls.");


    controlText.setCharacterSize(20); // Pixels, not normal font size
    controlText.setFillColor(sf::Color(200, 200, 200)); // Color

    controlText.setOutlineThickness(2);
    controlText.setOutlineColor(sf::Color(25, 25, 25));

    controlText.setStyle(sf::Text::Bold);


    controlText.setPosition(10, 70);
    window.draw(controlText);
}

void DisplayManager::drawDebug() {
    sf::Text debugText;
    debugText.setFont(font);

    sf::Vector2<double> coords = getCameraCenter();
    debugText.setString(std::to_string(fps));


    debugText.setCharacterSize(20); // Pixels, not normal font size
    debugText.setFillColor(sf::Color(0, 200, 0)); // Color

    debugText.setOutlineThickness(2);
    debugText.setOutlineColor(sf::Color(0, 0, 0));

    debugText.setStyle(sf::Text::Bold);


    debugText.setPosition(displaySettings.screenWidth-50, 10);
    window.draw(debugText);
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
