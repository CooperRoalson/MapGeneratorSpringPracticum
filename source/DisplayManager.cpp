//
//  DisplayManager.cpp
//  Map Generator
//

#include "DisplayManager.hpp"

#include "exe_path.hpp"

#include <iostream>

DisplayManager::DisplayManager(int width, int height, std::string _resource_dir)
    : window(sf::VideoMode(width, height), "Map Generator") {
    
    resourceDir = _resource_dir;
    if (resourceDir == "") {
        resourceDir = ExePath::getExecutableDir() + "/" + "resources/";
    }
    
    std::cout << resourceDir << "\n";
}

void DisplayManager::display() {
    window.clear();
    draw();
    window.display();
}

void DisplayManager::draw() {
    return; // Nothing for now
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

