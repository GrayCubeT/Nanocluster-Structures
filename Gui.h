#pragma once
#include <SFML/Graphics.hpp>
#include "GlobalFunctions.h"
#include <iostream>
#include "Constants.h"

const auto BORDER_COLOR = sf::Color::White;
const int BORDER_THICKNESS = 3;
const auto SLIDER_COLOR = sf::Color::Green;
const auto TEXT_COLOR = sf::Color::Magenta;
const auto OUTLINE_COLOR = sf::Color::White;
const int OUTLINE_THICKNESS = 0;

extern sf::Font GlobalFont;

template <typename T>
class Slider : public sf::Drawable {
private:
    sf::Text t;
    sf::RectangleShape fill;
    sf::RectangleShape border;
    sf::FloatRect rect;
    T maxVal;
    T minVal;
    T* val;

public:
    Slider(T* val, T maxval, T minval, sf::FloatRect _rect);
    bool contains(sf::Vector2f point);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void onPress(sf::Vector2f pos);
};

class Gui : public sf::Drawable {
private:
    // button textures
    sf::Texture stopTexture;
    sf::Texture startTexture;
    sf::Texture pauseTexture;
    sf::Texture toggleSpawnTexture;
    sf::Texture toggleNoDrawTexture;
    sf::Texture getStatsTexture;
    sf::Texture exitTexture;

    // actual buttons (trolling achieved)
    sf::RectangleShape exitBtn;
    sf::RectangleShape stopBtn;
    sf::RectangleShape startBtn;
    sf::RectangleShape pauseBtn;
    sf::RectangleShape toggleSpawnBtn;
    sf::RectangleShape toggleNoDrawBtn;
    sf::RectangleShape getStatsBtn;

    // sliders
    Slider<double> cluSize;
    Slider<double> temp;
    Slider<int> spawnSpeed;
    Slider<int> spawnAmt;
    Slider<int> calcSpeed;

    // changing values that dont need any special stuff
    sf::Vector2f* clientsize;
    bool* paused;
    bool* spawnRandom;
    bool* noDraw;


public:
    // values that must be checked every time
    bool statsRequest;
    bool exitRequest;
    bool stopRequest;
    bool click;
    bool failed;
    bool fpsChange;

    Gui(sf::Vector2f* _clientsize, bool* _paused, bool* _spawnRandom, bool* _noDraw, 
        double* _cluSize, double* _temp, int* _spawnSpeed, int* _spawnAmt,
        int* fps);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void check(sf::Vector2i mpos);
};

template<typename T>
inline Slider<T>::Slider(T* val, T maxval, T minval, sf::FloatRect _rect) :
    val(val), maxVal(maxval), minVal(minval), rect(_rect) {

    double part = (double(*val) - minval) / (double(maxval) - minval);

    fill = sf::RectangleShape(sf::Vector2f(_rect.width * part, _rect.height));
    fill.setPosition(_rect.left, _rect.top);
    fill.setFillColor(SLIDER_COLOR);



    border = sf::RectangleShape(sf::Vector2f(_rect.width, _rect.height));
    border.setPosition(_rect.left, _rect.top);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(BORDER_THICKNESS);
    border.setOutlineColor(BORDER_COLOR);

    t = sf::Text(numtostr(*val, 1), GlobalFont);
    t.setOutlineColor(OUTLINE_COLOR);
    t.setOutlineThickness(OUTLINE_THICKNESS);
    t.setFillColor(TEXT_COLOR);
    t.setCharacterSize(_rect.height - 10);
    t.setPosition(_rect.left + _rect.width + t.getLocalBounds().width / 2 + 3, _rect.top);
}

template<typename T>
inline bool Slider<T>::contains(sf::Vector2f point) {
    return border.getGlobalBounds().contains(point);
}

template<typename T>
inline void Slider<T>::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(fill);
    target.draw(border);
    target.draw(t);
}

template<typename T>
inline void Slider<T>::onPress(sf::Vector2f pos) {
    auto part = (pos.x - rect.left) / (rect.width);
    auto value = minVal + (maxVal - minVal) * part;
    if (value > maxVal) {
        value = maxVal;
    }
    if (value < minVal) {
        value = minVal;
    }
    *val = value;

    auto size = sf::Vector2f(rect.width, rect.height);
    fill.setSize(sf::Vector2f(size.x * part, size.y));

    t.setString(numtostr(*val, 1));
    t.setPosition(rect.left + rect.width + t.getLocalBounds().width / 2 + 3, rect.top);

}
