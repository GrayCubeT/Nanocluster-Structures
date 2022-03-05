#include "ViewHandler.h"

viewHandler::viewHandler(sf::RenderWindow* _win) : mpos(sf::Vector2f()), prevmpos(sf::Vector2f()),
lmb(false), rmb(false), shiftmult(1), win(_win), zoom(1), lmbMoveLock(true)
{
    view = win->getDefaultView();
    clientsize = sf::Vector2f(win->getSize());
    screenratio = clientsize.y / clientsize.x;
}

void viewHandler::handle() {

    if (!win->hasFocus()) {
        lmbMoveLock = true;
        return;
    }
    
    

    mpos = win->mapPixelToCoords(sf::Mouse::getPosition(*win));
    //mouse handling
    lmb = (sf::Mouse::isButtonPressed(sf::Mouse::Left));
    rmb = (sf::Mouse::isButtonPressed(sf::Mouse::Right));

    // view movement
    if (keypress(sf::Keyboard::A)) {
        view.move(-5 * shiftmult * MOVE_MULTIPLIER, 0);
    }
    if (keypress(sf::Keyboard::D)) {
        view.move(5 * shiftmult * MOVE_MULTIPLIER, 0);
    }
    if (keypress(sf::Keyboard::W)) {
        view.move(0, -5 * shiftmult * MOVE_MULTIPLIER);
    }
    if (keypress(sf::Keyboard::S)) {
        view.move(0, 5 * shiftmult* MOVE_MULTIPLIER);
    }

    if (lmbMoveLock && !lmb) {
        lmbMoveLock = false;
    }

    if (!lmbMoveLock &&
            sf::Mouse::getPosition(*win).x > win->getSize().x / 100.0 * CONTROL_PART &&
            lmb) {
        view.move(-mpos.x + prevmpos.x, -mpos.y + prevmpos.y);
        
    }
    else {
        prevmpos = mpos;
    }

    //zoom
    if (keypress(sf::Keyboard::Z)) {
        zoom *= 1.01;
        view.zoom(1.01);
    }
    if (keypress(sf::Keyboard::X)) {
        zoom *= 0.99;
        view.zoom(0.99);
    }
}

void viewHandler::mouseWheelScroll(float delta) {
    zoom *= (1.0 - delta * 0.1);
    view.zoom(1.0 - delta * 0.1);
}

void viewHandler::setView() {
    win->setView(view);
}
