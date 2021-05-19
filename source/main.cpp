//
//  main.cpp
//  Map Generator
//

//#include <SFML/Graphics.hpp>

#define USE_WORKING_DIR true
#define ALT_RESOURCE_FOLDER ""

#include "DisplayManager.hpp"
#include "exe_path.hpp"
#include "TileMap.hpp"
#include "PerlinNoise.hpp"

#include <iostream>

int main(int argc, char const** argv)
{
    
    /*
     Where is the resource folder?
      USE_WORKING_DIR specifies whether to use the working directory
      ALT_RESOURCE_FOLDER specifies the location. "" uses the exe's location
    */
    std::string path = USE_WORKING_DIR ? ExePath::mergePaths(ExePath::getWorkingDir(), "resources") : ALT_RESOURCE_FOLDER;
    
    
    TileMap::GenerationSettings gs = {
        
                                    // Tile generation
                                      100, 100, // Map size
                                      10., // Noise size
                                      2, // Levels of noise detail
                                      4., // Detail size reduction per level
        
                                    // Mountain generation
                                      15., // Mountain range scatter (distance between ranges)
                                      0.34, // Mountain range spread (size)
                                      0.08, // Mountain range density
                                      1.5, // Mountain min height
                                      2.0, // Mountain max height
        
                                    // Mountain smoothing
                                      0.15, // Mountain smooth threshold
                                      15,   // Number of smoothing passes
                                      0.65, // How much to smooth (min and max)
                                      0.85, // ^ Lower values = more smoothing

                                    // Default Color
                                      0
        
                                     };
    
    TileMap* tileMap = new TileMap(gs);
    
    DisplayManager::DisplaySettings ds = {960, 540, // Screen width and height
                                          36.6, 40, // Starting camera x and y
                                          15, // Starting tile size
                                          3, 150 // Min and max tile sizes
                                         };
    
    DisplayManager dm(ds, tileMap, path);
    
    double cameraSpeed = 3;
    
    
    bool left = false, right = false, up = false, down = false;
    bool drag = false;
    int recentDragX, recentDragY;
    
    sf::Clock clock;
    float lastTime = 0;

    while (dm.isOpen())
    {

        // Process events
        sf::Event event;
        while (dm.pollEvent(event))
        {
            // Close the window
            if (event.type == sf::Event::Closed) {
                dm.close();
            }
            
            // Zoom with mouse
            else if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    dm.changeTileSize(event.mouseWheelScroll.delta);
                    cameraSpeed = ds.initialTileSize / (dm.getTileSize()*dm.getTileSize());
                }
            }
            
            // Drag with mouse
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    recentDragX = event.mouseButton.x;
                    recentDragY = event.mouseButton.y;
                    drag = true;
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    drag = false;
                }
            }
            // Move with mouse. Mouse take priority over WASD.
            else if (event.type == sf::Event::MouseMoved) {
                if (drag) {
                    int newX = event.mouseMove.x;
                    int newY = event.mouseMove.y;
                    
                    dm.moveCamera(((recentDragX-newX)/dm.getTileSize()) * ((double)(ds.screenWidth)/dm.getWindowWidth()), ((recentDragY-newY)/dm.getTileSize()) * ((double)(ds.screenHeight)/dm.getWindowHeight()));
                    
                    recentDragX = newX;
                    recentDragY = newY;
                }
            }
            

            // regenerates the tilemap when space is pressed
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    delete tileMap;
                    tileMap = new TileMap(gs);
                    dm.setTileMap(tileMap);
                }
                else if (event.key.code == sf::Keyboard::C) {
                    tileMap->setColorMode((tileMap->getColorMode() + 1) % 4);
                    tileMap->rerenderTiles();
                    gs.colorMode = tileMap->getColorMode();
                }
                else if (event.key.code == sf::Keyboard::V) {
                    tileMap->setColorMode((tileMap->getColorMode() + 3) % 4);
                    tileMap->rerenderTiles();
                    gs.colorMode = tileMap->getColorMode();
                }
            }
        }

        // frame-locked actions
        // Move via WASD. Mouse take priority over WASD.
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            up = true;
        }
        else {
            up = false;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            down = true;
        }
        else {
            down = false;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            left = true;
        }
        else {
            left = false;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            right = true;
        }
        else {
            right = false;
        }
        if (!drag) {
            if (up && !down) { dm.moveCamera(0, -cameraSpeed); }
            else if (down && !up) { dm.moveCamera(0, cameraSpeed); }
            if (left && !right) { dm.moveCamera(-cameraSpeed, 0); }
            else if (right && !left) { dm.moveCamera(cameraSpeed, 0); }
        }
        
        // FPS calculation
        double currentTime = clock.getElapsedTime().asSeconds();
        double fps = 1.0 / (currentTime - lastTime);
        if ((int)(currentTime * 10) - (int)(lastTime * 10) == 1) {
            dm.setFPS((int)(fps));
        }
        lastTime = currentTime;

        dm.display();
    }
    
    delete tileMap;

    return 0;
    
}
