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
    
    
    GenerationSettings gs = {100, 100, // Map size
                            10, // Noise size
                            1, // Levels of noise detail
                            2};
    TileMap tileMap(gs);
    
    DisplayManager dm(800, 600, // Window size
                      10, // Tile size
                      &tileMap, path);
    
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
            
        }
        
        dm.display();
    }

    return 0;
    
}
