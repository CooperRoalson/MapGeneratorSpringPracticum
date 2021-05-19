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
                                      150, 150, // Map size
                                      40., // Noise size
                                      3, // Levels of noise detail
                                      4., // Detail size reduction per level
        
                                    // Mountain generation
                                      30, // Mountain range scatter (distance between ranges)
                                      0.35, // Mountain range spread (size)
                                      0.05, // Mountain range density
                                      1.5, // Mountain min height
                                      3.0, // Mountain max height
        
                                    // Mountain smoothing
                                      1.4, // Mountain smooth threshold
                                      15,   // Number of smoothing passes
                                      0.4, // How much to smooth (min and max)
                                      0.6, // ^ Lower values = more smoothing

                                    // Oceans
                                      0.5,  // Sea level
                                      false, // Can mountains generate in oceans
                                      0.03, // Beach threashold
                                      0.05, // Sea floor layer size diff
                                      3 // humidity dispersion smoothing passes
        
                                     };
    
    TileMap* tileMap = new TileMap(gs);
    
    DisplayManager::DisplaySettings ds = {1366, 768, // Screen width and height
                                          36.6, 40, // Starting camera x and y
                                          15, // Starting tile size
                                          3, 150, // Min and max tile sizes
                                          200, 200, 200, // Base text color
                                          20, 20, 20, // Outline text color
                                          0, // Default display mode
                                         };
    
    DisplayManager dm(ds, tileMap, path);
    
    double cameraSpeed = 3;
    double FPSUpdateFreq = 0.2; // How often to update the FPS display (in seconds)
    double maxClickLength = 0.2; // How long a click can be to qualify (in seconds)
    
    bool left = false, right = false, up = false, down = false;
    bool drag = false;
    int recentDragX, recentDragY;
    
    sf::Clock clickTime;
    int clickX, clickY;
    
    sf::Clock clock;
    double currentTime, lastTime = 0;
    int frameCounter = 0;

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
                    
                    clickX = event.mouseButton.x;
                    clickY = event.mouseButton.y;
                    clickTime.restart();
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    drag = false;
                    
                    if (event.mouseButton.x - clickX == 0 && event.mouseButton.y - clickY == 0 && clickTime.getElapsedTime().asSeconds() <= maxClickLength) {
                        dm.onClick(clickX * ((double)(ds.screenWidth)/dm.getWindowWidth()), clickY * ((double)(ds.screenHeight)/dm.getWindowHeight()));
                    }
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
            

            else if (event.type == sf::Event::KeyPressed) {
                
                // regenerates the tilemap when space is pressed
                if (event.key.code == sf::Keyboard::Space) {
                    delete tileMap;
                    tileMap = new TileMap(gs);
                    dm.setTileMap(tileMap);
                }
                
                // changes color mode
                else if (event.key.code == sf::Keyboard::C) {
                    dm.setDisplayMode((dm.getDisplayMode() + 3) % 4);
                    tileMap->rerenderTiles(dm.getDisplayMode());
                }
                else if (event.key.code == sf::Keyboard::V) {
                    dm.setDisplayMode((dm.getDisplayMode() + 1) % 4);
                    tileMap->rerenderTiles(dm.getDisplayMode());
                }
                
                // exits tile view
                else if (event.key.code == sf::Keyboard::Escape) {
                    dm.setWhetherViewingTile(false);
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
        currentTime = clock.getElapsedTime().asSeconds();
        frameCounter ++;
        if (currentTime - lastTime >= FPSUpdateFreq) {
            double fps = frameCounter / (currentTime - lastTime);
            dm.setFPS((int)(fps));
            lastTime = currentTime;
            frameCounter = 0;
        }

        dm.display();
    }
    
    delete tileMap;

    return 0;
    
}
