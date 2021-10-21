#include <stdio.h>
#include <vector>
#include <iomanip>
#include <iostream>
#include "SFML/Graphics.hpp"
#include "shapes.h"
#include "GlobalFunctions.h"
#include "ClusterStructure.h"
#include "ViewHandler.h"
#include "EventHandler.h"
#include "Gui.h"
#include "Constants.h"

using namespace sf;

sf::Font GlobalFont;
enum {
    
};

int main() {
    // local properties for the task
    srand(time(0));
    double temperature = START_TEMPERATURE;
    double cluSize = CLUSTER_SIZE;
    int moveDelay = MOVE_DELAY;
    int spawnDelay = SPAWN_DELAY;
    int spawnAmt = SPAWN_AMOUNT;
    int fps = FPS;
    bool randomSpawn = RANDOM_ENABLED;
    bool paused = false;
    bool noDraw = false;

    RenderWindow win(VideoMode(1400, 700), "Stat Phys");

    // this has control over speed
	win.setFramerateLimit(FPS);
    
    // views and handling
    View view = win.getDefaultView();
    viewHandler vhandler(&win);
    
    // height is divided by 2 for viewport reasons
    vhandler.view.setSize(sf::Vector2f(DEFAULTVIEWSIZE * 0.5 / vhandler.screenratio, DEFAULTVIEWSIZE));
    vhandler.view.setCenter(sf::Vector2f());
    vhandler.view.setViewport(sf::FloatRect(0.5, 0, 0.5, 1));

    View fixedView = win.getDefaultView();
    EventHandler eventHandler(&win, &vhandler, &fixedView);

    int btnPressed = 0;

    // timers
    unsigned int frameCount = 0;
    unsigned int moveTimer = 0;
    unsigned int spawnTimer = 0;
    unsigned int buttonTimer = 0;

    // ClusterStructure that manages everything
    ClusterStructure clusters = ClusterStructure(CIRCUSSIZE, temperature, cluSize, moveDelay);
    
    // starting center cluster
    // clusters.addCluster(HexCluster(cluSize, sf::Vector2f(), true));

    // gui stuff
    if (!GlobalFont.loadFromFile("virgo.ttf")) {
        std::cout << "font not loaded, exiting";
        return 0;
    }
    int value = 0;
    Gui gui(&(vhandler.clientsize), &paused, &randomSpawn, &noDraw, &cluSize, &temperature, &spawnDelay, &spawnAmt, &fps);
    

    //Slider<int> slider(&value, 100, 0, sf::FloatRect(100, 100, 200, 40));
    Text mposDebugText = Text("", GlobalFont);
    mposDebugText.setFillColor(Color::Magenta);
    mposDebugText.setOutlineThickness(1);
    mposDebugText.setOutlineColor(Color::White);

    while (win.isOpen()) {

        // event handling
        vhandler.handle();
        eventHandler.handle();

        // all the buttons that cant be pressed permanent
        if (buttonTimer <= frameCount) {
            // manual cluster spawn with F
            if (keypress(Keyboard::F)) {
                clusters.addCluster(HexCluster(cluSize, vhandler.mpos));
                buttonTimer = frameCount + fps / 10;
            }
            // pause
            if (keypress(Keyboard::Escape)) {
                paused = !paused;
                buttonTimer = frameCount + fps / 2;
            }
        }

        // any simulation and automatic stuff is paused
        if (!paused) {
            // random spawn
            if (spawnTimer <= frameCount && randomSpawn) {
                for (int i = 0; i < spawnAmt; ++i) {
                    clusters.addRandom();
                }
                spawnTimer = frameCount + spawnDelay;
            }

            // physics, uses clusterstructure's step()
            if (moveTimer <= frameCount) {
                moveTimer = frameCount + MOVE_DELAY;
                clusters.step();

            }

            // this runs in steps, so running this every frame is aclually better when it's correctly set up
            clusters.collisionCheck();
        }


        // gui collect
        if (buttonTimer <= frameCount && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            gui.check(sf::Mouse::getPosition(win));
            if (gui.click) {
                buttonTimer = frameCount + fps / 2;
                gui.click = false;
            }
            if (gui.stopRequest) {
                clusters.regenerate(temperature, cluSize);
                gui.stopRequest = false;
            }
            if (gui.exitRequest) {
                gui.exitRequest = false;
                return 0;
            }
            if (gui.fpsChange) {
                win.setFramerateLimit(fps);
                gui.fpsChange = false;
            }
            if (gui.statsRequest) {
                gui.statsRequest = false;
            }
        }


        // drawing
        win.clear();

        // gui
        win.setView(fixedView);

        //debug
        {
            // debug
            if (keypress(Keyboard::B)) {
                frameCount = frameCount;
                auto _ = clusters.__secret_debug_function__(vhandler.mpos);
                mposDebugText.setString(numtostr(_.x) + " " + numtostr(_.y));
                win.draw(mposDebugText);
                auto p = sf::Mouse::getPosition(win);
                std::cout << p.x << " " << p.y << "\n";
            }
        }
        
        win.draw(gui);

        //win.draw(slider);
        //if (slider.contains(sf::Vector2f(sf::Mouse::getPosition(win)))) {
        //   slider.onPress(sf::Vector2f(sf::Mouse::getPosition(win)));
        //    std::cout << value << "\n";
        //}


        
        vhandler.setView();

        if (!noDraw && clusters.clusterNum > 0) {
            win.draw(clusters);
        }

		win.display();

        if (frameCount % 200 == 0) {
            win.setFramerateLimit(fps);
        }
        // timers update
        frameCount++;
	}
    
	return 0;
}