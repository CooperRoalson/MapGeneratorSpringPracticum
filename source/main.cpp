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
    std::string path = USE_WORKING_DIR ? ExePath::getWorkingDir() + "/resources/" : ALT_RESOURCE_FOLDER;
    
    
    TileMap::GenerationSettings gs = {100, 100, // Map size
                                      10, // Noise size
                                      3, // Levels of noise detail
                                      2 // Detail size reduction per level
                                     };
    
    TileMap tileMap(gs);
    
    DisplayManager::DisplaySettings ds = {800, 600, // Screen width and height
                                          36.6, 40, // Starting camera  x and y
                                          30, // Starting tile size
                                          10, 150 // Min and max tile sizes
                                         };
    
    DisplayManager dm(ds, &tileMap, path);
    
    double cameraSpeed = 1;
    
    
    bool left = false, right = false, up = false, down = false;
    bool drag = false;
    int recentDragX, recentDragY;
    
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
            
            else if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    dm.changeSize(event.mouseWheelScroll.delta);
                }
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::W) {
                    up = true;
                } else if (event.key.code == sf::Keyboard::S) {
                    down = true;
                } else if (event.key.code == sf::Keyboard::A) {
                    left = true;
                } else if (event.key.code == sf::Keyboard::D) {
                    right = true;
                }
            }
            else if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::W) {
                    up = false;
                } else if (event.key.code == sf::Keyboard::S) {
                    down = false;
                } else if (event.key.code == sf::Keyboard::A) {
                    left = false;
                } else if (event.key.code == sf::Keyboard::D) {
                    right = false;
                }
            }
            
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
            else if (event.type == sf::Event::MouseMoved) {
                if (drag) {
                    int newX = event.mouseMove.x;
                    int newY = event.mouseMove.y;
                    
                    dm.moveCamera((recentDragX-newX)/dm.getTileSize(), (recentDragY-newY)/dm.getTileSize());
                    
                    recentDragX = newX;
                    recentDragY = newY;
                }
            }
            
        }
        
        
        if (up && !down) {dm.moveCamera(0, -cameraSpeed/dm.getTileSize());}
        else if (down && !up) {dm.moveCamera(0, cameraSpeed/dm.getTileSize());}
        if (left && !right) {dm.moveCamera(-cameraSpeed/dm.getTileSize(), 0);}
        else if (right && !left) {dm.moveCamera(cameraSpeed/dm.getTileSize(), 0);}
        
        dm.display();
    }

    return 0;
    
}
