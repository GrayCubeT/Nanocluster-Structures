#define SFML_DEFINE_DISCRETE_GPU_PREFERENCE
#include <stdio.h>
#include <vector>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "shapes.h"
#include "GlobalFunctions.h"
#include "ClusterStructure.h"
#include "ViewHandler.h"
#include "EventHandler.h"
#include "Gui.h"
#include "Constants.h"

using namespace sf;

sf::Clock gclock;
sf::Font GlobalFont;
const char theoryFilename[] = "theory.pdf";


int game(sf::RenderWindow& win, std::string filename);

enum State {
    GAME,
    MENU,
    AUTHORS
};

bool appInFocus(sf::RenderWindow& app)
{

    HWND handle = app.getSystemHandle();
    bool one = handle == GetFocus();
    bool two = handle == GetForegroundWindow();

    
    return one && two;
}

void menu(sf::RenderWindow& win) {
    sf::View view = win.getDefaultView();
    std::string savename = "";
    win.setFramerateLimit(FPS);
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
        return;
    }
    vmk.setTexture(&circus);

    sf::RectangleShape start(sf::Vector2f(180, 50));
    sf::Texture stext;
    start.setPosition(clientsize.x / 2 - 90, clientsize.y / 2 - 25);
    if (!stext.loadFromFile("images/startBtn.png")) {
        std::cout << "Failed to load image\n";
        return;
    }
    start.setTexture(&stext);

    

    sf::RectangleShape load1(sf::Vector2f(243, 50));
    sf::Texture loadtext1;
    load1.setPosition(clientsize.x / 2 + 90 + 40, clientsize.y / 2 - 25 + 70);
    if (!loadtext1.loadFromFile("images/save1.png")) {
        std::cout << "Failed to load image\n";
        return;
    }
    load1.setTexture(&loadtext1);

    sf::RectangleShape load2(sf::Vector2f(243, 50));
    sf::Texture loadtext2;
    load2.setPosition(clientsize.x / 2 + 90 + 40, clientsize.y / 2 + 45 + 70);
    if (!loadtext2.loadFromFile("images/save2.png")) {
        std::cout << "Failed to load image\n";
        return;
    }
    load2.setTexture(&loadtext2);

    
    sf::RectangleShape loadOutline(sf::Vector2f(283, 210));
    loadOutline.setPosition(clientsize.x / 2 + 90 + 20, clientsize.y / 2 - 25);
    sf::Texture loadOT;
    if (!loadOT.loadFromFile("images/saves.png")) {
        std::cout << "Failed to load image\n";
        return;
    }
    loadOutline.setTexture(&loadOT);

    sf::Text loadFile("filename", GlobalFont, 50);
    loadFile.setPosition(clientsize.x / 2 - 90 + 200, clientsize.y / 2 + 35);
    loadFile.setFillColor(sf::Color::Green);

    sf::RectangleShape authors(sf::Vector2f(180, 50));
    sf::Texture at;
    authors.setPosition(clientsize.x / 2 - 90, clientsize.y / 2 + 45 + 70);
    if (!at.loadFromFile("images/authors.png")) {
        std::cout << "Failed to load image\n";
        return;
    }
    authors.setTexture(&at);

    
    sf::RectangleShape theory(sf::Vector2f(180, 50));
    sf::Texture theoryText;
    theory.setPosition(clientsize.x / 2 - 90, clientsize.y / 2 + 45 + 140);
    if (!theoryText.loadFromFile("images/theoryBtn.png")) {
        std::cout << "Failed to load image\n";
        return;
    }
    theory.setTexture(&theoryText);
    


    sf::RectangleShape exit(sf::Vector2f(180, 50));
    sf::Texture et;
    exit.setPosition(clientsize.x / 2 - 90, clientsize.y / 2 + 45 + 140 + 70);
    if (!et.loadFromFile("images/exitBtn.png")) {
        std::cout << "Failed to load image\n";
        return;
    }
    exit.setTexture(&et);

    sf::RectangleShape max(sf::Vector2f(400, 400));
    sf::Texture maxt;
    max.setPosition(clientsize.x / 2 - 400 - 20, clientsize.y / 2 - 200);
    if (!maxt.loadFromFile("images/max.jpg")) {
        std::cout << "Failed to load image\n";
        return;
    }
    max.setTexture(&maxt);

    sf::RectangleShape danya(sf::Vector2f(400, 400));
    sf::Texture danyat;
    danya.setPosition(clientsize.x / 2 + 20, clientsize.y / 2 - 200);
    if (!danyat.loadFromFile("images/danya.jpg")) {
        std::cout << "Failed to load image\n";
        return;
    }
    danya.setTexture(&danyat);


    while (win.isOpen()) {
        Event event;
        while (win.pollEvent(event)) {
            // close window event
            if (event.type == Event::Closed) {
                win.close();
            }
            else if (event.type == Event::Resized) {
                view.setSize(event.size.width, event.size.height);
                win.setView(view);
            }
            else if (event.type == Event::KeyPressed) {
                if (appInFocus(win)) {

                    
                    if (event.key.code <= 25 && event.key.code >= 0) {
                        savename += ('a' + event.key.code);
                        loadFile.setString(savename);
                    }
                    else if (event.key.code <= 35) {
                        savename += ('0' + event.key.code - 26);
                        loadFile.setString(savename);
                    }
                    else if (event.key.code == 59 && savename.size() > 0) {
                        savename = savename.substr(0, savename.size() - 1);
                        loadFile.setString(savename);
                    }
                    else if (event.key.code == 58 && savename.size() > 0) {
                        state = GAME;
                        game(win, savename);
                        win.setView(view);
                        state = MENU;
                    }
                    else if (event.key.code == 50) {
                        savename += '.';
                        loadFile.setString(savename);
                    }
                    std::cout << savename << "\n";
                }
            }
        }
        
        sf::Vector2f mpos(win.mapPixelToCoords(sf::Mouse::getPosition(win)));
        lmb = sf::Mouse::isButtonPressed(sf::Mouse::Left) && appInFocus(win);
        if (win.hasFocus() && lmb) {
            if (lmb && !lock) {
                if (start.getGlobalBounds().contains(mpos)) {
                    state = GAME;
                    game(win, "");
                    win.setView(view);
                    state = MENU;
                }
                if (load1.getGlobalBounds().contains(mpos)) {
                    state = GAME;
                    game(win, "save1.sav");
                    win.setView(view);
                    state = MENU;
                }
                if (load2.getGlobalBounds().contains(mpos)) {
                    state = GAME;
                    game(win, "save2.sav");
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
                if (theory.getGlobalBounds().contains(mpos)) {
                    ShellExecute(NULL, "open", TEXT("theory.pdf"), NULL, NULL, SW_SHOW);
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
            win.draw(loadOutline);
            win.draw(load1);
            win.draw(load2);
            win.draw(theory);
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
}

void simulation(sf::RenderWindow& win, 
        ClusterStructure& clusters, 
        viewHandler* vhandler, 
        Gui* gui, 
        EventHandler* eventHandler, 
        int* fps, bool* paused) {
    gui->setCounter(0);
    auto ans = clusters.timesort();
    if (ans.size() == 0) {
        return;
    }

    gui->update();
    bool btnLock = true;
    unsigned int frameCount = 0, buttonTimer = 100;

    *paused = true;
    win.setFramerateLimit(150);

    std::vector<sf::Vertex> vertices;
    vertices.resize(ans.size() * 12);
    for (unsigned int i = 0; i < ans.size() * 12; i++) {
        vertices[i].position = ans[i / 12]->pos + ans[i / 12]->verticesPos[i % 12];
    }

    unsigned int smax = ans.size();
    unsigned int scur = 0;
    
    sf::RectangleShape boundingBox(sf::Vector2f(DEFAULTVIEWSIZE * 0.89, DEFAULTVIEWSIZE * 0.89));
    boundingBox.setPosition(-DEFAULTVIEWSIZE * 0.43, -DEFAULTVIEWSIZE * 0.43);
    boundingBox.setOutlineThickness(50);
    boundingBox.setOutlineColor(sf::Color::Green);
    boundingBox.setFillColor(sf::Color::Transparent);

    auto __ = win.getSize();
    View fixedView = sf::View(sf::Vector2f(__.x / 2, __.y / 2), sf::Vector2f(__));

    while (win.isOpen()) {

        // event handling
        vhandler->handle();
        eventHandler->handle();

        

        // simulation
        if (!*paused) {
            if (frameCount % (253 - *fps) == 0) {
                scur += 1;
                gui->setCounter(scur);
            }
        }


        // gui collect
        if (appInFocus(win) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (!btnLock) {
                btnLock = true;
                gui->check(sf::Mouse::getPosition(win));
                if (!gui->click) {
                    btnLock = false;
                }
                if (gui->stopRequest) {
                    gui->stopRequest = false;
                    *paused = true;
                    return;
                }
                if (gui->exitRequest) {
                    gui->exitRequest = false;
                    *paused = true;
                    return;
                }
                if (gui->fpsChange) {
                    gui->fpsChange = false;
                }
                if (gui->simRequest) {
                    gui->simRequest = false;
                    *paused = true;
                    return;
                }

            }
        }
        else {
            btnLock = false;
        }



        // drawing
        win.clear();

        // gui
        win.setView(fixedView);
        win.draw(*gui);

        vhandler->setView();
        win.draw(boundingBox);
        win.draw(&vertices[0], std::min(scur * 12, smax * 12), sf::PrimitiveType::Triangles);
        
        win.display();

        // timer update
        frameCount++;
    }
}

int game(sf::RenderWindow& win, std::string filename) {
    srand(time(0));
    // local properties for the task
    bool load = (filename != "none" && filename != "");
    if (!load) {
        filename = "save.sav";
    }
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
    bool specialMode = false;
    long long int lastTicks = 0;
    
    
    // views and handling
    auto __ = win.getSize();
    viewHandler* vhandler = new viewHandler(&win);
    
    // width is divided by 2 for viewport reasons
    vhandler->view.setSize(sf::Vector2f(DEFAULTVIEWSIZE * (1.0 - CONTROL_PART / 100.0) / vhandler->screenratio, DEFAULTVIEWSIZE));
    vhandler->view.setCenter(sf::Vector2f());
    vhandler->view.setViewport(sf::FloatRect(CONTROL_PART / 100.0, 0, 1.0 - CONTROL_PART / 100.0, 1));

    View fixedView = sf::View(sf::Vector2f(__.x / 2, __.y / 2), sf::Vector2f(__));
    EventHandler* eventHandler = new EventHandler(&win, vhandler, &fixedView);

    int btnPressed = 0;

    // timers are not 0 cus startup kind of
    unsigned int frameCount = 0;
    unsigned int moveTimer = frameCount + 10;
    unsigned int spawnTimer = frameCount + 10;
    unsigned int buttonTimer = frameCount + 20;
    bool btnLock = false;

    // ClusterStructure that manages everything
    
    ClusterStructure clusters(CIRCUSSIZE, temperature, cluSize, moveDelay);
    if (load) {
        std::ifstream in(filename, std::ios::binary | std::ios::in);
        clusters.load(in);
        temperature = clusters.temperature;
        cluSize = clusters.clusterSize;
        in.close();
    }
    sf::RectangleShape boundingBox(sf::Vector2f(DEFAULTVIEWSIZE * 0.89, DEFAULTVIEWSIZE * 0.89));
    boundingBox.setPosition(-DEFAULTVIEWSIZE * 0.43, -DEFAULTVIEWSIZE * 0.43);
    boundingBox.setOutlineThickness(50);
    boundingBox.setOutlineColor(sf::Color::Green);
    boundingBox.setFillColor(sf::Color::Transparent);

    // starting center cluster
    // clusters.addCluster(HexCluster(cluSize, sf::Vector2f(), true));

    // gui stuff
    
    Gui* gui = new Gui(&(vhandler->clientsize), &paused, &randomSpawn, &noDraw, &specialMode, &cluSize, &temperature, &spawnDelay, &spawnAmt, &fps);
    

    //Slider<int> slider(&value, 100, 0, sf::FloatRect(100, 100, 200, 40));
    Text mposDebugText = Text("", GlobalFont);
    mposDebugText.setFillColor(Color::Magenta);
    mposDebugText.setOutlineThickness(1);
    mposDebugText.setOutlineColor(Color::White);

    gui->setCounter(clusters.clusterNum);


    while (win.isOpen()) {

        // event handling
        vhandler->handle();
        eventHandler->handle();

        // all the buttons that cant be pressed permanent
        if (appInFocus(win) && buttonTimer <= frameCount) {
            // manual cluster spawn with rmb
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                clusters.addCluster(HexCluster(cluSize, vhandler->mpos, false));
                gui->setCounter(clusters.clusterNum);
                buttonTimer = frameCount + fps / 10;
            }
        }

        // any simulation and automatic stuff is paused
        if (!paused) {
            // random spawn
            if (spawnTimer <= frameCount && randomSpawn) {
                for (int i = 0; i < spawnAmt; ++i) {
                    clusters.addRandom();
                }
                gui->setCounter(clusters.clusterNum);
                spawnTimer = frameCount + spawnDelay;
            }

            // this runs in steps, so running this every frame is aclually better when it's correctly set up
            clusters.collisionCheck();
            // physics, uses clusterstructure's step()
            if (moveTimer <= frameCount) {
                moveTimer = frameCount + MOVE_DELAY;
                clusters.step();
                gui->setCounter(clusters.clusterNum);
            }

        }


        // gui collect
        if (appInFocus(win) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (!btnLock) {
                btnLock = true;
                gui->check(sf::Mouse::getPosition(win));
                if (!gui->click) {
                    btnLock = false;
                }
                if (gui->simRequest) {
                    gui->simulation = true;
                    gui->simRequest = false;
                    simulation(win, clusters, vhandler, gui, eventHandler, &fps, &paused);
                    gui->update();
                    gui->simulation = false;
                }
                if (gui->stopRequest) {
                    clusters.regenerate(temperature, cluSize);
                    gui->stopRequest = false;
                }
                if (gui->exitRequest) {
                    gui->exitRequest = false;
                    delete vhandler;
                    delete eventHandler;
                    delete gui;
                    return 0;
                }
                if (gui->fpsChange) {
                    if (fps > 200) {
                        win.setFramerateLimit(0);
                    }
                    else {
                        win.setFramerateLimit(fps);
                    }
                    gui->fpsChange = false;
                }
                if (gui->statsRequest) {
                    gui->statsRequest = false;
                    
                    std::ofstream s(filename, std::ios::binary | std::ios::out);
                    clusters.save(s);
                    s.close();
                }
                if (gui->sizeChange) {
                    clusters.clusterSize = cluSize;
                    gui->sizeChange = false;
                }
                if (gui->tempChange) {
                    clusters.settemp(temperature);
                    gui->tempChange = false;
                }
                if (gui->specialModeRequest) {
                    clusters.addCluster(HexCluster(cluSize, sf::Vector2f(), true));
                    clusters.swapCollisionTech();
                    gui->specialModeRequest = false;
                }
                
            }
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
                /*auto _ = clusters.__secret_debug_function__(vhandler.mpos);
                mposDebugText.setString(numtostr(_.x) + " " + numtostr(_.y));
                win.draw(mposDebugText);
                auto p = sf::Mouse::getPosition(win);
                std::cout << p.x << " " << p.y << "\n";
                */
                if (frameCount % 100 == 0) {

                    long long int calcTicks = gclock.getElapsedTime().asMilliseconds() - lastTicks;
                    lastTicks = calcTicks;
                    std::cout << "FPS: " << 100.0 / calcTicks * 1000.0 << " TPF:" << calcTicks / 100.0 << "\n";
                    

                }
            }
        }
        
        win.draw(*gui);

        //win.draw(slider);
        //if (slider.contains(sf::Vector2f(sf::Mouse::getPosition(win)))) {
        //   slider.onPress(sf::Vector2f(sf::Mouse::getPosition(win)));
        //    std::cout << value << "\n";
        //}


        
        vhandler->setView();
        win.draw(boundingBox);
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
    delete vhandler;
    delete eventHandler;
    delete gui;
	return 0;
}


int main() {
    if (!GlobalFont.loadFromFile("calibri.ttf")) {
        std::cout << "font not loaded, exiting";
        return 0;
    }
    unsigned int frameCount = 0;
    RenderWindow win(VideoMode(1400, 700), "Stat Phys");
    win.setVerticalSyncEnabled(false);
    menu(win);
    return 0;
}
