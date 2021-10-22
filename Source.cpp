#include <stdio.h>
#include <vector>
#include <iomanip>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "shapes.h"
#include "GlobalFunctions.h"
#include "ClusterStructure.h"
#include "ViewHandler.h"
#include "EventHandler.h"
#include "Gui.h"
#include "Constants.h"

using namespace sf;

sf::Font GlobalFont;

int game(sf::RenderWindow& win);

enum State {
    GAME,
    MENU,
    AUTHORS
};

int main() {
    RenderWindow win(VideoMode(1400, 700), "Stat Phys");
    win.setFramerateLimit(FPS);
    sf::View view = win.getDefaultView();
    int framecount = 0;
    bool lmb = false;
    bool lock = false;
    State state = MENU;

    sf::Vector2f clientsize = sf::Vector2f(win.getSize());

    sf::RectangleShape vmk(sf::Vector2f(clientsize.x, 200));
    sf::Texture circus;
    vmk.setPosition(0, 0);
    if (!circus.loadFromFile("images/circus.jpg")) {
        std::cout << "Failed to load image\n";
        return 0;
    }
    vmk.setTexture(&circus);

    sf::RectangleShape start(sf::Vector2f(180, 50));
    sf::Texture stext;
    start.setPosition(clientsize.x / 2 - 90, clientsize.y / 2 - 25);
    if (!stext.loadFromFile("images/placeholder.jpg")) {
        std::cout << "Failed to load image\n";
        return 0;
    }
    start.setTexture(&stext);

    sf::RectangleShape authors(sf::Vector2f(180, 50));
    sf::Texture at;
    authors.setPosition(clientsize.x / 2 - 90, clientsize.y / 2 + 25 + 20);
    if (!at.loadFromFile("images/placeholder.jpg")) {
        std::cout << "Failed to load image\n";
        return 0;
    }
    authors.setTexture(&at);

    sf::RectangleShape exit(sf::Vector2f(180, 50));
    sf::Texture et;
    exit.setPosition(clientsize.x / 2 - 90, clientsize.y / 2 + 25 + 50 + 40);
    if (!et.loadFromFile("images/placeholder.jpg")) {
        std::cout << "Failed to load image\n";
        return 0;
    }
    exit.setTexture(&et);

    sf::RectangleShape max(sf::Vector2f(400, 300));
    sf::Texture maxt;
    max.setPosition(clientsize.x / 2 - 400 - 20, clientsize.y / 2 - 200);
    if (!maxt.loadFromFile("images/max.jpg")) {
        std::cout << "Failed to load image\n";
        return 0;
    }
    max.setTexture(&maxt);

    sf::RectangleShape danya(sf::Vector2f(400, 300));
    sf::Texture danyat;
    danya.setPosition(clientsize.x / 2 + 20, clientsize.y / 2 - 200);
    if (!danyat.loadFromFile("images/danya.jpg")) {
        std::cout << "Failed to load image\n";
        return 0;
    }
    danya.setTexture(&danyat);

    while (win.isOpen()) {
        Event event;
        while (win.pollEvent(event)) {
            // close window event
            if (event.type == Event::Closed) {
                win.close();
            }
            if (event.type == Event::Resized) {
                view.setSize(event.size.width, event.size.height);
                win.setView(view);
            }
        }
        sf::Vector2f mpos(win.mapPixelToCoords(sf::Mouse::getPosition(win)));
        lmb = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        if (lmb) {
            if (lmb && !lock) {
                if (start.getGlobalBounds().contains(mpos)) {
                    state = GAME;
                    game(win);
                    win.setView(view);
                    state = MENU;
                }
                if (authors.getGlobalBounds().contains(mpos)) {
                    state = AUTHORS;
                }
                if (exit.getGlobalBounds().contains(mpos)) {
                    if (state == AUTHORS) {
                        state = MENU;
                    }
                    else {
                        win.close();
                    }
                }
            }
            lock = true;
        }
        else {
            lock = false;
        }


        win.clear();

        if (state == MENU) {
            win.draw(start);
            win.draw(authors);
        }
        if (state == AUTHORS) {
            win.draw(max);
            win.draw(danya);
        }
        win.draw(exit);
        win.draw(vmk);

        win.display();
        framecount++;
    }
    return 0;
}

int game(sf::RenderWindow& win) {
    // local properties for the task
    srand(time(0));
    double temperature = START_TEMPERATURE;
    double cluSize = CLUSTER_SIZE;
    int moveDelay = MOVE_DELAY;
    int spawnDelay = SPAWN_DELAY;
    int spawnAmt = SPAWN_AMOUNT;
    int fps = FPS;
    int prevfps;
    bool randomSpawn = RANDOM_ENABLED;
    bool paused = true;
    bool noDraw = false;

   

    // this has control over speed
	
    
    // views and handling
    auto __ = win.getSize();
    View view = sf::View(sf::Vector2f(__.x / 2, __.y / 2), sf::Vector2f(__));
    viewHandler vhandler(&win);
    
    // height is divided by 2 for viewport reasons
    vhandler.view.setSize(sf::Vector2f(DEFAULTVIEWSIZE * 0.5 / vhandler.screenratio, DEFAULTVIEWSIZE));
    vhandler.view.setCenter(sf::Vector2f());
    vhandler.view.setViewport(sf::FloatRect(0.5, 0, 0.5, 1));

    View fixedView = sf::View(sf::Vector2f(__.x / 2, __.y / 2), sf::Vector2f(__));
    EventHandler eventHandler(&win, &vhandler, &fixedView);

    int btnPressed = 0;

    // timers are not 0 cus startup kind of
    unsigned int frameCount = 10;
    unsigned int moveTimer = 10;
    unsigned int spawnTimer = 10;
    unsigned int buttonTimer = 20;
    bool btnLock = false;

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
        if (win.hasFocus() && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (!btnLock) {
                gui.check(sf::Mouse::getPosition(win));
                if (gui.click) {
                    buttonTimer = frameCount + fps / 2;
                    gui.click = false;
                }
                if (gui.stopRequest) {
                    clusters.regenerate(temperature, cluSize);
                    paused = true;
                    randomSpawn = true;
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
                if (gui.sizeChange) {
                    clusters.clusterSize = cluSize;
                    gui.sizeChange = false;
                }
                if (gui.tempChange) {
                    clusters.settemp(temperature);
                    gui.tempChange = false;
                }

            }
            btnLock = true;
        }
        else {
            btnLock = false;
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
        
        if (clusters.clusterNum > 0) {
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