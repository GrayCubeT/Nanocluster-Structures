#include <stdio.h>
#include <vector>
#include <iomanip>
#include <iostream>
#include "SFML/Graphics.hpp"
#include "shapes.h"
#include "GlobalFunctions.h"
#include "ClusterStructure.h"

using namespace sf;

enum {
    CIRCUSSIZE = 1000,
    START_TEMPERATURE = 20,
    MOVE_DELAY = 100,
    SPAWN_DELAY = 100,
    BUTTON_DELAY = 500
};

int main() {
    // local properties for the task
    srand(time(0));
    double temperature = START_TEMPERATURE;
    
    RenderWindow win(VideoMode(700, 700), "oops", Style::Fullscreen);
	win.setFramerateLimit(60);
    
    // window properties
    bool paused = false;

    // views and handling
    View fixedview = win.getDefaultView();
    View view = win.getDefaultView();
    viewHandler vhandler(win);
    int btnPressed = 0;
    sf::Vector2f mpos;

    // timers
    double buttonCooldown = 0;
    double spawnTimer = 0;
    double moveTimer = 0;

    // ClusterStructure that manages everything
    ClusterStructure clusters = ClusterStructure(CIRCUSSIZE, temperature);

    clusters.addCluster(HexCluster(20, sf::Vector2f(), true));

    view.move(sf::Vector2f(-vhandler.clientsize.x / 2, -vhandler.clientsize.y / 2));

    while (win.isOpen()) {
        mpos = win.mapPixelToCoords(sf::Mouse::getPosition());
        vhandler.mpos = mpos;

        // event handling
        Event event;
        while (win.pollEvent(event)) {
            // close window event
            if (event.type == Event::Closed) {
                win.close();
            }
            // view handling (happens only when an event is done)
            else if (event.type == Event::MouseWheelScrolled) {
                vhandler.mouseWheelScroll(win, view, event.mouseWheelScroll.delta);
            }
            // TODO? resize
        }

        // view handler
        vhandler.handle(win, view);

        // create new clusters
        // F
        if (keypress(Keyboard::F) && buttonCooldown <= 0) {
            clusters.addCluster(HexCluster(30, vhandler.mpos));
            buttonCooldown = BUTTON_DELAY;
        }
        // random
        if (spawnTimer <= 0) {
            clusters.addRandom();
            spawnTimer = SPAWN_DELAY;
        }

        // physics
        if (moveTimer <= 0) {
            moveTimer = MOVE_DELAY;
            clusters.step();
        }
        
		win.clear();
        win.setView(fixedview);
        // draw text + GUI here

        win.setView(view);
        
        if (clusters.clusterNum > 0) {
            win.draw(clusters);
        }

		win.display();

        buttonCooldown -= 1000 / 60;
        moveTimer -= 1000 / 60;
        spawnTimer -= 1000 / 60;
	}
    
	return 0;
}