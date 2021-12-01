#pragma once
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "GlobalFunctions.h"
#include "Constants.h"


const auto BORDER_COLOR = sf::Color::White;
const int BORDER_THICKNESS = 3;
const auto SLIDER_COLOR = sf::Color::Color(0, 255, 0);
const auto TEXT_COLOR = sf::Color::Color(0, 255, 0);
const auto OUTLINE_COLOR = sf::Color::White;
const int OUTLINE_THICKNESS = 0;

extern sf::Font GlobalFont;

template <typename T>
class Slider : public sf::Drawable {
private:
    sf::Text t;
    sf::RectangleShape fill;
    sf::RectangleShape background;
    sf::Texture bgTexture;
    sf::FloatRect rect;
    T maxVal;
    T minVal;
    T* val;

public:
    Slider(T* val, T maxval, T minval, sf::FloatRect _rect, std::string filename);
    bool contains(sf::Vector2f point);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void onPress(sf::Vector2f pos);
};

class Gui : public sf::Drawable {
private:
    // button textures
    sf::Texture stopTexture;
    sf::Texture startTexture;
    sf::Texture getStatsTexture;
    sf::Texture exitTexture;
    sf::Texture simulationTexture;

    sf::Texture pauseTexture;
    sf::Texture unpauseTexture;

    sf::Texture spawnOnTexture;
    sf::Texture spawnOffTexture;
    sf::Texture specialModeOnTexture;
    sf::Texture specialModeOffTexture;
    sf::Texture ccTexture;

    // actual buttons (trolling achieved)
    sf::RectangleShape exitBtn;
    sf::RectangleShape stopBtn;
    sf::RectangleShape startBtn;
    sf::RectangleShape pauseBtn;
    sf::RectangleShape toggleSpawnBtn;
    sf::RectangleShape getStatsBtn;
    sf::RectangleShape simulateBtn;
    sf::RectangleShape specialModeBtn;

    sf::RectangleShape ccText;
    sf::Text clusterCounter;

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
    bool* specialMode;

public:
    // values that must be checked every time
    bool statsRequest;
    bool exitRequest;
    bool stopRequest;
    bool click;
    bool failed;
    bool fpsChange;
    bool tempChange;
    bool sizeChange;
    bool simRequest;
    bool specialModeRequest;

    bool simulation;

    Gui(sf::Vector2f* _clientsize, bool* _paused, bool* _spawnRandom, bool* _noDraw,
        bool* _specialMode,
        double* _cluSize, double* _temp, int* _spawnSpeed, int* _spawnAmt,
        int* fps);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void check(sf::Vector2i mpos);
    void update();
    void setCounter(int n);
};

template<typename T>
inline Slider<T>::Slider(T* val, T maxval, T minval, sf::FloatRect _rect, std::string filename) :
        val(val), maxVal(maxval), minVal(minval), 
        rect(_rect.left, _rect.top + 39, _rect.width, 20) {

    double part = (double(*val) - minval) / (double(maxval) - minval);

    fill = sf::RectangleShape(sf::Vector2f(_rect.width * part, _rect.height / 3.1));
    fill.setPosition(_rect.left + 3, _rect.top + 39);
    fill.setFillColor(SLIDER_COLOR);

    background = sf::RectangleShape(sf::Vector2f(_rect.width, _rect.height));
    background.setPosition(_rect.left, _rect.top);

    if (!bgTexture.loadFromFile(filename)) {
        throw std::runtime_error("Slider texture not found");
    }
    background.setTexture(&bgTexture);

    t = sf::Text(numtostr(*val, 1), GlobalFont);
    t.setOutlineColor(OUTLINE_COLOR);
    t.setOutlineThickness(OUTLINE_THICKNESS);
    t.setFillColor(TEXT_COLOR);
    t.setCharacterSize(_rect.height / 2);
    t.setPosition(_rect.left + _rect.width + 10, _rect.top + _rect.height / 2);

}

template<typename T>
inline bool Slider<T>::contains(sf::Vector2f point) {
    return rect.contains(point);
}

template<typename T>
inline void Slider<T>::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(background);
    target.draw(fill);
    target.draw(t);
}

template<typename T>
inline void Slider<T>::onPress(sf::Vector2f pos) {
    auto part = (pos.x - rect.left - 3) / (rect.width - 6);
    auto value = minVal + (maxVal - minVal) * part;
    if (value > maxVal) {
        value = maxVal;
    }
    if (value < minVal) {
        value = minVal;
    }
    *val = value;

    auto size = sf::Vector2f(rect.width - 6, rect.height);
    fill.setSize(sf::Vector2f(size.x * part, size.y));

    t.setString(numtostr(*val, 1));

}
