#pragma once
#include <SFML/Graphics.hpp>
#include "ViewHandler.h"

using namespace sf;

class EventHandler {
    sf::RenderWindow* win;
    viewHandler* vhandler;
    sf::View* fixedView;

public:
    EventHandler(sf::RenderWindow* _win, viewHandler* _vhandler, sf::View* _view);

    void handle();
};