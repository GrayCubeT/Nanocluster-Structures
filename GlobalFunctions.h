#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include <math.h>

bool keypress(sf::Keyboard::Key key);

class BrounRNG {
private:
    double temperature;
    std::normal_distribution<> generator;
    std::default_random_engine seedling;

public:
    BrounRNG(double t);
    double settmp(double temperature);
    sf::Vector2f rng();
};

class viewHandler {
public:
    sf::Vector2f mpos, prevmpos;
    bool lmb, rmb, rmbmoveflag;
    double shiftmult;
    sf::Vector2f clientsize;
    double screenratio;

    viewHandler(sf::RenderWindow& win);
    void handle(sf::RenderWindow& win, sf::View& view);
    void mouseWheelScroll(sf::RenderWindow& win, sf::View& view, float delta);
};