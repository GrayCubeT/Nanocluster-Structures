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
    return t;
}
sf::Vector2f BrounRNG::rng() {
    return (sf::Vector2f(generator(seedling), generator(seedling)));
}

viewHandler::viewHandler(sf::RenderWindow &win) : mpos(sf::Vector2f()), prevmpos(sf::Vector2f()), 
        lmb(false), rmb(false), rmbmoveflag(true), shiftmult(1) 
{
    clientsize = sf::Vector2f(win.getSize());
    screenratio = clientsize.y / clientsize.x;
}

void viewHandler::handle(sf::RenderWindow& win, sf::View& view) {
    

    //mouse handling
    lmb = (sf::Mouse::isButtonPressed(sf::Mouse::Left));
    rmb = (sf::Mouse::isButtonPressed(sf::Mouse::Right));

    //shift
    if (keypress(sf::Keyboard::LShift)) {
        shiftmult = 10;
    }
    else {
        shiftmult = 1;
    }


    // view movement
    if (keypress(sf::Keyboard::A)) {
        view.move(-5 * shiftmult, 0);
    }
    if (keypress(sf::Keyboard::D)) {
        view.move(5 * shiftmult, 0);
    }
    if (keypress(sf::Keyboard::W)) {
        view.move(0, -5 * shiftmult);
    }
    if (keypress(sf::Keyboard::S)) {
        view.move(0, 5 * shiftmult);
    }
    if (rmb) {
        if (rmbmoveflag) {
            prevmpos = mpos;
            rmbmoveflag = false;
        }
        else {
            view.move(-mpos.x + prevmpos.x, -mpos.y + prevmpos.y);
        }
    }
    else {
        rmbmoveflag = true;
    }


    //zoom
    if (keypress(sf::Keyboard::Z)) {
        view.setSize(view.getSize() + sf::Vector2f(10 * shiftmult, 10 * screenratio * shiftmult));
    }
    if (keypress(sf::Keyboard::X)) {
            view.setSize(view.getSize() + sf::Vector2f(-10 * shiftmult, -10 * screenratio * shiftmult));
        }

}

void viewHandler::mouseWheelScroll(sf::RenderWindow& win, sf::View& view, float delta) {
    view.setSize(view.getSize() + sf::Vector2f(-100 * shiftmult * delta, -100 * shiftmult * screenratio * delta));
}
