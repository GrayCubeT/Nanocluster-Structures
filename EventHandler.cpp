#include "EventHandler.h"

EventHandler::EventHandler(sf::RenderWindow* _win, viewHandler* _vhandler, sf::View* _view) : win(_win), vhandler(_vhandler), fixedView(_view) {
}

void EventHandler::handle() {
    // event handling
    Event event;
    while (win->pollEvent(event)) {
        // close window event
        if (event.type == Event::Closed) {
            win->close();
        }
        // view handling (happens only when an event is done)
        else if (event.type == Event::MouseWheelScrolled) {
            vhandler->mouseWheelScroll(event.mouseWheelScroll.delta);
        }
        else if (event.type == Event::Resized) {
            //std::cout << "was: " << vhandler->view.getSize().x << " " << vhandler->view.getSize().y << "\n";
            sf::Vector2f prevclientsize = vhandler->clientsize;
            vhandler->clientsize = sf::Vector2f(event.size.width, event.size.height);
            vhandler->screenratio = event.size.height / event.size.width;
            sf::Vector2f s = vhandler->view.getSize();
            vhandler->view.setSize(s.x * event.size.width / prevclientsize.x, s.y * event.size.height / prevclientsize.y);

            fixedView->setSize(event.size.width, event.size.height);
            fixedView->setCenter(event.size.width / 2.0, event.size.height / 2.0);
            
        }
        
    }
}
