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
    CIRCUSSIZE = 10000,
    CLUSTER_SIZE = 30,

    START_TEMPERATURE = 30,
    // how much frames it takes for next step
    MOVE_DELAY = 1,
    // how much frames it takes to spawn a new cluster (should be a multiple of move_delay)
    SPAWN_DELAY = 6,
    // how much frames it takes for button presses to register after a button press
    BUTTON_DELAY = 6,
    RANDOM_ENABLED = false
};

int main() {
    // local properties for the task
    srand(time(0));
    double temperature = START_TEMPERATURE;
    
    RenderWindow win(VideoMode(700, 700), "oops", Style::Fullscreen);
	win.setFramerateLimit(120);
    
    // window properties
    bool paused = false;

    // views and handling
    View view = win.getDefaultView();
    viewHandler vhandler(win);
    int btnPressed = 0;
    sf::Vector2f mpos;

    // timers
    unsigned int frameCount = 0;
    unsigned int moveTimer = 0;
    unsigned int spawnTimer = 0;
    unsigned int buttonTimer = 0;

    // ClusterStructure that manages everything
    ClusterStructure clusters = ClusterStructure(CIRCUSSIZE, temperature, CLUSTER_SIZE, MOVE_DELAY);
    
    // starting center cluster
    clusters.addCluster(HexCluster(CLUSTER_SIZE, sf::Vector2f(), true));

    // gui stuff
    Font font;
    
    if (!font.loadFromFile("virgo.ttf")) {
        std::cout << "font not loaded, exiting";
        return 0;
    }

    View fixedview = win.getDefaultView();
    Text mposDebugText = Text("", font);
    mposDebugText.setFillColor(Color::Magenta);
    mposDebugText.setOutlineThickness(1);
    mposDebugText.setOutlineColor(Color::White);

    // actual program
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
        // manual spawn with F
        if (keypress(Keyboard::F) && buttonTimer <= frameCount) {
            clusters.addCluster(HexCluster(CLUSTER_SIZE, vhandler.mpos));
            buttonTimer = frameCount + BUTTON_DELAY;
        }
        // random spawn
        if (spawnTimer <= frameCount && RANDOM_ENABLED) {
            clusters.addRandom();
            spawnTimer = frameCount + SPAWN_DELAY;
        }

        if (keypress(Keyboard::B)) {
            frameCount = frameCount;
        }

        // physics, uses clusterstructure's step()
        if (moveTimer <= frameCount) {
            moveTimer = frameCount + MOVE_DELAY;
            clusters.step();
            
        }
        
        // this runs in steps, so running this every frame is aclually better when it's correctly set up
        clusters.collisionCheck();
        
        // drawing
		win.clear();
        win.setView(fixedview);
        
        // draw text + GUI here
        auto _ = clusters.__secret_debug_function__(mpos);
        mposDebugText.setString(numtostr(_.x) + " " + numtostr(_.y));
        win.draw(mposDebugText);
        
        
        // 
        win.setView(view);
        
        if (clusters.clusterNum > 0) {
            win.draw(clusters);
        }

		win.display();


        // timers update
        frameCount++;
	}
    
	return 0;
}