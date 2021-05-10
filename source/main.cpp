//
//  main.cpp
//  Map Generator
//

//#include <SFML/Graphics.hpp>

#define USE_WORKING_DIR true
#define ALT_RESOURCE_FOLDER ""

#include "DisplayManager.hpp"

#include <iostream>
#include <filesystem>

#include "exe_path.hpp"

int main(int argc, char const** argv)
{
    /*
     Where is the resource folder?
      USE_WORKING_DIR specifies whether to use the working directory
      ALT_RESOURCE_FOLDER specifies the location. "" uses the exe's location
    */
    std::string path = USE_WORKING_DIR ? ExePath::getWorkingDir() + "/resources/" : ALT_RESOURCE_FOLDER;
    
    std::cout << "Working dir: " << ExePath::getWorkingDir() << "\n";
    DisplayManager dm(800, 600, path);
    
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
    
/*
    std::string resource_dir = ExePath::getExecutableDir() + "/" + "resources/";
 
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile(resource_dir + "icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Load a sprite to display
    sf::Texture texture;
    if (!texture.loadFromFile(resource_dir + "cute_image.jpg")) {
        return EXIT_FAILURE;
    }
    sf::Sprite sprite(texture);

    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile(resource_dir + "sansation.ttf")) {
        return EXIT_FAILURE;
    }
    sf::Text text("Hello SFML", font, 50);
    text.setFillColor(sf::Color::Black);

 
    
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
        }
        
        window.clear();

        draw();

        window.display();
    }

    return EXIT_SUCCESS;
 */
}
