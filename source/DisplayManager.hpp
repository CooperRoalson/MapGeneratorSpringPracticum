//
//  DisplayManager.hpp
//  Map Generator
//

#ifndef DisplayManager_hpp
#define DisplayManager_hpp

#include "DisplayManager.hpp"

#include <SFML/Graphics.hpp>

class DisplayManager {
    
public:
    DisplayManager(int width, int height, std::string resource_dir = "");
    
    void display();
    
    void close();
    bool isOpen();
    bool pollEvent(sf::Event &event);
    
    
private:
    sf::RenderWindow window;
    
    std::string resourceDir;
    
    void draw();
    
};

#endif /* DisplayManager_hpp */
