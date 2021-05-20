//
//  DisplayManager.cpp
//  Map Generator
//

#include "DisplayManager.hpp"

#include "exe_path.hpp"
#include "RoundedRectangleShape.hpp"

#include <iostream>
#include <string>

DisplayManager::DisplayManager(DisplaySettings settings, TileMap* tm, std::string rDir)
    : window(sf::VideoMode(settings.screenWidth, settings.screenHeight), "Map Generator") {

        mapRenderTexture.create(settings.screenWidth, settings.screenHeight);
        menuRenderTexture.create(settings.screenWidth, settings.screenHeight);

        view.setCenter(sf::Vector2f(settings.screenWidth/2, settings.screenHeight/2));
        view.setSize(sf::Vector2f(settings.screenWidth, settings.screenHeight));

        window.setView(view);

        if (settings.mapStartEnabled) {
            activeMap = true;
            activeMapUI = true;
        }
        else
            activeMenu = true;

        displaySettings = settings;
        
        tileMap = tm;
        
    resourceDir = rDir;
    if (resourceDir == "") {
        resourceDir = ExePath::mergePaths(ExePath::getExecutableDir(), "resources");
    }
    
    //std::cout << "Using resource directory: " << resourceDir << "\n";
        
    xOffset = settings.initialXOffset;
    yOffset = settings.initialYOffset;
    tileSize = fmax(settings.screenWidth, settings.screenHeight)/settings.initialTilesShown;
    
    loadFont();
    loadIcon();

}

void DisplayManager::setTileMap(TileMap* tm) {
    tileMap = tm;
    setWhetherViewingTile(false);
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

void DisplayManager::resize(int width, int height) {
    displaySettings.screenWidth = width;
    displaySettings.screenHeight = height;
    changeTileSize(0); // Make sure tilesize is within bounds

    view.setCenter(sf::Vector2f(displaySettings.screenWidth / 2, displaySettings.screenHeight / 2));
    view.setSize(sf::Vector2f(displaySettings.screenWidth, displaySettings.screenHeight));

    window.setView(view);

    mapRenderTexture.create(displaySettings.screenWidth, displaySettings.screenHeight);
    menuRenderTexture.create(displaySettings.screenWidth, displaySettings.screenHeight);
}

void DisplayManager::display() {
    window.clear();
    draw();
    window.display();
}

void DisplayManager::draw() {
    if (activeMap) {
        // clear map rendertexture
        mapRenderTexture.clear(sf::Color(63,63,55));

        // map drawing
        drawTiles();
        if (activeMapUI) {
            if (viewingTile) { drawTileStats(); }
            drawCoords();
            drawControls();
            drawDebug();
            drawColorScheme();
        }

        // update map rendertexture
        mapRenderTexture.display();

        // pass rendertexture to window
        const sf::Texture& texture = mapRenderTexture.getTexture();
        sf::Sprite renderTextureSprite(texture);
        window.draw(renderTextureSprite);
    }
    if (activeMenu) {
        menuRenderTexture.clear();

        menuRenderTexture.display();

        const sf::Texture& texture3 = menuRenderTexture.getTexture();
        sf::Sprite renderTextureSprite(texture3);
        window.draw(renderTextureSprite);
    }
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
            
            if (viewingTile == true && viewTileCoords.x == x && viewTileCoords.y == y && activeMapUI) {
                drawTile(sf::Color(255, 100, 100), screenPos);
            }
            else
                drawTile(t, screenPos);
            
        }
    }
}

void DisplayManager::drawTile(Tile* t, sf::Vector2f screenPos) {
    
    /* rect.setPosition(screenPos);
    rect.setSize(sf::Vector2f(tileSize, tileSize));
    
    rect.setFillColor(t->getColor(getDisplayMode()));
    mapRenderTexture.draw(rect); */

    drawTile(t->getColor(getDisplayMode()), screenPos);
}

void DisplayManager::drawTile(sf::Color highlight, sf::Vector2f screenPos) {
    rect.setPosition(screenPos);
    rect.setSize(sf::Vector2f(tileSize, tileSize));
    rect.setFillColor(highlight);
    mapRenderTexture.draw(rect);
}

void DisplayManager::drawTileStats() {
    Tile* viewTile = tileMap->getTile(viewTileCoords.x, viewTileCoords.y);

    std::string featureText;
    if (viewTile->hasFeature("sea_cliff")) {
        featureText = "Cliffs";
    } else if (viewTile->isOcean()) {
        featureText = "Ocean";
    } else if (viewTile->hasFeature("mountain")) {
        featureText = "Mountain";
    } else if (viewTile->hasFeature("foothill")) {
        featureText = "Foothills";
    } else if (viewTile->hasFeature("beach")) {
        featureText = "Beach";
    } else if (viewTile->hasFeature("forest")) {
        featureText = "Forest";
    } else {
        featureText = "Plains";
    }
    
    int offset = 5;
    int fontSize = 30; // Pixels

    int xSize = 13.5 * fontSize;
    int ySize = 4 * fontSize + 6 * offset;

    // window EDGE case fix, haha
    if (viewTileDisplayCoords.x > displaySettings.screenWidth - xSize)
        viewTileDisplayCoords.x -= xSize + 50;
    if (viewTileDisplayCoords.y > displaySettings.screenHeight - ySize)
        viewTileDisplayCoords.y -= ySize + 50;
    
    sf::Vector2f size(xSize, ySize);
    sf::RoundedRectangleShape rect(size, 5, 5); // A class I found off of GitHub (make sure to add the files to your IDE in order to see them)
    rect.setFillColor(sf::Color(100, 100, 100));
    rect.setPosition(viewTileDisplayCoords);
    
    mapRenderTexture.draw(rect);

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(fontSize);
    text.setFillColor(sf::Color(displaySettings.baseR, displaySettings.baseG, displaySettings.baseB));
    text.setOutlineThickness(2);
    text.setOutlineColor(sf::Color(displaySettings.outR, displaySettings.outG, displaySettings.outB));
    
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    text.setPosition(viewTileDisplayCoords.x + offset, viewTileDisplayCoords.y + offset);
    text.setString("Tile (" + std::to_string(viewTileCoords.x) + "," + std::to_string(viewTileCoords.y) + ")" + ((featureText != "") ? " - " + featureText : ""));
    mapRenderTexture.draw(text);
    text.setStyle(sf::Text::Regular);
        
    text.move(0, fontSize + offset);
    text.setString("Elevation: " + std::to_string((int) (100*viewTile->getAttribute("elevation"))));
    mapRenderTexture.draw(text);
    
    text.move(0, fontSize + offset);
    text.setString("Temperature: " + std::to_string((int) (100*viewTile->getAttribute("temperature"))));
    mapRenderTexture.draw(text);
    
    text.move(0, fontSize + offset);
    text.setString("Humidity: " + std::to_string((int) (100*viewTile->getAttribute("humidity"))));
    mapRenderTexture.draw(text);
    
}

void DisplayManager::drawCoords() {
    sf::Text coordText;
    coordText.setFont(font);
    
    sf::Vector2<double> coords = getCameraCenter();
    coordText.setString("(" + std::to_string((int)(coords.x)) + ", " + std::to_string((int)(coords.y)) + ")");

    
    coordText.setCharacterSize(70); // Pixels, not normal font size
    coordText.setFillColor(sf::Color(displaySettings.baseR, displaySettings.baseG, displaySettings.baseB)); // Color
    
    coordText.setOutlineThickness(2);
    coordText.setOutlineColor(sf::Color(displaySettings.outR, displaySettings.outG, displaySettings.outB));
    
    coordText.setStyle(sf::Text::Bold);

    
    coordText.setPosition(10, 10);
    mapRenderTexture.draw(coordText);
}

void DisplayManager::drawControls() {
    sf::Text controlText;
    controlText.setFont(font);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
        controlText.setString("WASD/arrows/click-and-drag to move. Shift to go faster.\nSpace to regenerate terrain. LeftCtrl + Space to enter seed in console.\nC/V to change display mode.\nClick on tile to view, ESC to stop viewing.\nF1 to toggle UI.");
    else
        controlText.setString("H for controls.");


    controlText.setCharacterSize(30); // Pixels, not normal font size
    controlText.setFillColor(sf::Color(displaySettings.baseR, displaySettings.baseG, displaySettings.baseB)); // Color

    controlText.setOutlineThickness(2);
    controlText.setOutlineColor(sf::Color(displaySettings.outR, displaySettings.outG, displaySettings.outB));

    controlText.setStyle(sf::Text::Bold);


    controlText.setPosition(10, 110);
    mapRenderTexture.draw(controlText);
}

void DisplayManager::drawDebug() {
    sf::Text debugText;
    debugText.setFont(font);

    debugText.setString(std::to_string(fps));


    debugText.setCharacterSize(40); // Pixels, not normal font size
    debugText.setFillColor(sf::Color(0, 255, 0)); // Color

    debugText.setOutlineThickness(2);
    debugText.setOutlineColor(sf::Color(0, 0, 0));

    debugText.setStyle(sf::Text::Bold);


    debugText.setPosition(displaySettings.screenWidth - debugText.getGlobalBounds().width - 10, 10);
    mapRenderTexture.draw(debugText);

    debugText.setString(std::to_string(tileMap->getSeed()));

    debugText.setFillColor(sf::Color(255, 255, 255));

    debugText.setCharacterSize(15);

    debugText.setPosition(displaySettings.screenWidth - debugText.getGlobalBounds().width - 10, 70);
    mapRenderTexture.draw(debugText);

    debugText.setString(std::to_string(tileMap->getSettings()->width) + " x " + std::to_string(tileMap->getSettings()->height));

    debugText.setPosition(displaySettings.screenWidth - debugText.getGlobalBounds().width - 10, 100);
    mapRenderTexture.draw(debugText);
}

void DisplayManager::drawColorScheme() {
    sf::Text colorSchemeText;
    colorSchemeText.setFont(font);

    switch(displaySettings.displayMode) {
        case 0:
            colorSchemeText.setString("Elevation + Features");
            break;
        case 1:
            colorSchemeText.setString("Elevation");
            break;
        case 2:
            colorSchemeText.setString("Temperature");
            break;
        case 3:
            colorSchemeText.setString("Humidity");
            break;
        default:
            colorSchemeText.setString("Invalid display setting!");
            break;
    }


    colorSchemeText.setCharacterSize(40); // Pixels, not normal font size
    colorSchemeText.setFillColor(sf::Color(displaySettings.baseR, displaySettings.baseG, displaySettings.baseB)); // Color

    colorSchemeText.setOutlineThickness(2);
    colorSchemeText.setOutlineColor(sf::Color(displaySettings.outR, displaySettings.outG, displaySettings.outB));

    colorSchemeText.setStyle(sf::Text::Bold);


    colorSchemeText.setPosition(10, displaySettings.screenHeight - colorSchemeText.getGlobalBounds().height - 20);
    mapRenderTexture.draw(colorSchemeText);
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

double DisplayManager::getMaxTileSize() {
    return fmax(displaySettings.screenWidth,displaySettings.screenHeight)/displaySettings.minTilesShown;
}

double DisplayManager::getMinTileSize() {
    return fmax(displaySettings.screenWidth,displaySettings.screenHeight)/displaySettings.maxTilesShown;
}

void DisplayManager::changeTileSize(double delta) {
    sf::Vector2<double> center = getCameraCenter();
    
    tileSize = fmax(fmin(tileSize + delta, getMaxTileSize()), getMinTileSize());
    
    xOffset = center.x - displaySettings.screenWidth/(2*tileSize);
    yOffset = center.y - displaySettings.screenHeight/(2*tileSize);
    
    setWhetherViewingTile(false);
}

void DisplayManager::moveCamera(double x, double y) {
    xOffset += x;
    yOffset += y;
    
    viewTileDisplayCoords.x -= x * ((double)(displaySettings.screenWidth) / getWindowWidth()) * tileSize;
    viewTileDisplayCoords.y -= y * ((double)(displaySettings.screenHeight) / getWindowHeight()) * tileSize;
}

void DisplayManager::setViewTile(sf::Vector2i tileCoords, sf::Vector2f screenCoords) {
    viewTileCoords = tileCoords;
    viewTileDisplayCoords = screenCoords;

    setWhetherViewingTile(true);
}

void DisplayManager::setWhetherViewingTile(bool view) {
    viewingTile = view;
}

sf::Vector2i DisplayManager::getTileCoordsFromScreenCoords(int screenX, int screenY) {
    return sf::Vector2i((int) (xOffset + (double)screenX/tileSize), (int) (yOffset + (double)screenY/tileSize));
}

void DisplayManager::onClick(int clickX, int clickY) {
    sf::Vector2i tileCoords = getTileCoordsFromScreenCoords(clickX, clickY);
    
    if ((!viewingTile || tileCoords != viewTileCoords) && tileCoords.x >= 0 && tileCoords.x < tileMap->getWidth() && tileCoords.y >= 0 && tileCoords.y < tileMap->getHeight()) {
        setViewTile(tileCoords, sf::Vector2f(clickX + displaySettings.screenWidth/20, clickY + displaySettings.screenHeight/20));
    } else {
        setWhetherViewingTile(false);
    }
}

int DisplayManager::getDisplayMode() {
    return displaySettings.displayMode;
}
void DisplayManager::setDisplayMode(int mode) {
    displaySettings.displayMode = mode;
}
