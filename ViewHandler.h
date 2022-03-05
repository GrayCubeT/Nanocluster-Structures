#pragma once
#include <SFML/Graphics.hpp>
#include "GlobalFunctions.h"
#include "Constants.h"

enum {
    MOVE_MULTIPLIER = 10
};

// A more complicated version of a view
// has functions that handle ASWD ZX and lmb mouse dragging 
class viewHandler {
public:
    sf::Vector2f mpos, prevmpos;
    bool lmb, rmb;
    double shiftmult;
    double zoom;
    sf::Vector2f clientsize;
    // y / x clientsize
    double screenratio;
    bool lmbMoveLock;
    sf::RenderWindow* win;
    sf::View view;

    viewHandler(sf::RenderWindow* _win);
    void handle();
    void mouseWheelScroll(float delta);
    void setView();
};
