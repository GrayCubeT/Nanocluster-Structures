#pragma once
#include <SFML/Graphics.hpp>

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
    sf::RenderWindow* win;
    sf::View view;

    viewHandler(sf::RenderWindow* _win);
    void handle();
    void mouseWheelScroll(float delta);
    void setView();
};
