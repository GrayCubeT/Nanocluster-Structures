#include "GlobalFunctions.h"


bool keypress(sf::Keyboard::Key key) {
    return sf::Keyboard::isKeyPressed(key);
}

BrounRNG::BrounRNG(double t) : seedling(){
    // this creates an rng generator
    
    temperature = t;
    generator = std::normal_distribution<double>(0.0, t);
}
double BrounRNG::settmp(double t) {
    double _ = temperature;
    temperature = t;
    generator = std::normal_distribution<double>(0.0, t);
    return _;
}
sf::Vector2f BrounRNG::rng() {
    return (sf::Vector2f(generator(seedling), generator(seedling)));
}
