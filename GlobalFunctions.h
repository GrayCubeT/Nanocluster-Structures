#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include <math.h>
#include <sstream>
#include <iomanip>

bool keypress(sf::Keyboard::Key key);

template <typename T>
std::string numtostr(T number, int precision = 0);

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

template<typename T>
inline std::string numtostr(T number, int precision) {
    std::stringstream ss;
    ss.precision(precision);
    ss << std::fixed << number;
    std::string ans = ss.str();
    return ans;
}

