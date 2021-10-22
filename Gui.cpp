#include "Gui.h"

//sf::Font GlobalFont;


// there is legit no way this works
Gui::Gui(sf::Vector2f* _clientsize, bool* _paused, bool* _spawnRandom, bool* _noDraw,
    double* _cluSize, double* _temp, int* _spawnSpeed, int* _spawnAmt,
    int* _fps) :
    clientsize(_clientsize), paused(_paused), spawnRandom(_spawnRandom),
    statsRequest(false), stopRequest(false), click(false), failed(true), exitRequest(false),
    fpsChange(false), tempChange(false), sizeChange(false),
    cluSize(
        _cluSize, MAX_CLUSTER_SIZE, MIN_CLUSTER_SIZE,
        sf::FloatRect(
            _clientsize->x / 14 * 0.2, _clientsize->y / 7 * 2,
            _clientsize->x / 14 * 6, _clientsize->y / 7 * 0.4)),
    temp(
        _temp, MAX_TEMP, MIN_TEMP,
        sf::FloatRect(
            _clientsize->x / 14 * 0.2, _clientsize->y / 7 * 2.8,
            _clientsize->x / 14 * 6, _clientsize->y / 7 * 0.4)),
    spawnSpeed(
        _spawnSpeed, MAX_SPAWN_SPEED, MIN_SPAWN_SPEED,
        sf::FloatRect(
            _clientsize->x / 14 * 0.2, _clientsize->y / 7 * 3.6,
            _clientsize->x / 14 * 6, _clientsize->y / 7 * 0.4)),
    spawnAmt(
        _spawnAmt, MAX_SPAWN_AMT, MIN_SPAWN_AMT,
        sf::FloatRect(
            _clientsize->x / 14 * 0.2, _clientsize->y / 7 * 4.4,
            _clientsize->x / 14 * 6, _clientsize->y / 7 * 0.4)),
    calcSpeed(
        _fps, MAX_FPS, MIN_FPS,
        sf::FloatRect(
            _clientsize->x / 14 * 0.2, _clientsize->y / 7 * 5.2,
            _clientsize->x / 14 * 6, _clientsize->y / 7 * 0.4))

{
    if (!stopTexture.loadFromFile("images/placeholder.jpg")) {
        failed = true;
        return;
    }
    if (!startTexture.loadFromFile("images/placeholder.jpg")) {
        failed = true;
        return;
    }
    if (!pauseTexture.loadFromFile("images/placeholder.jpg")) {
        failed = true;
        return;
    }
    if (!toggleSpawnTexture.loadFromFile("images/placeholder.jpg")) {
        failed = true;
        return;
    }
    if (!toggleNoDrawTexture.loadFromFile("images/placeholder.jpg")) {
        failed = true;
        return;
    } 
    if (!getStatsTexture.loadFromFile("images/placeholder.jpg")) {
        failed = true;
        return;
    }
    if (!exitTexture.loadFromFile("images/placeholder.jpg")) {
        failed = true;
        return;
    }
    
    // equivalent of 100 units in standart mode
    double x = clientsize->x / 14; 
    double y = clientsize->y / 7;

    startBtn = sf::RectangleShape(sf::Vector2f(x * 1.8, y * 0.5));
    startBtn.setFillColor(sf::Color::White);
    startBtn.setTexture(&startTexture);
    startBtn.setPosition(x * 0.2, y * 0.2);

    pauseBtn = sf::RectangleShape(sf::Vector2f(x * 1.8, y * 0.5));
    pauseBtn.setFillColor(sf::Color::White);
    pauseBtn.setTexture(&pauseTexture);
    pauseBtn.setPosition(x * 2.7, y * 0.2);

    stopBtn = sf::RectangleShape(sf::Vector2f(x * 1.8, y * 0.5));
    stopBtn.setFillColor(sf::Color::White);
    stopBtn.setTexture(&stopTexture);
    stopBtn.setPosition(x * 5.2, y * 0.2);

    toggleSpawnBtn = sf::RectangleShape(sf::Vector2f(x * 1.8, y * 0.5));
    toggleSpawnBtn.setFillColor(sf::Color::White);
    toggleSpawnBtn.setTexture(&toggleSpawnTexture);
    toggleSpawnBtn.setPosition(x * 0.2, y * 1.1);

    getStatsBtn = sf::RectangleShape(sf::Vector2f(x * 1.8, y * 0.5));
    getStatsBtn.setFillColor(sf::Color::White);
    getStatsBtn.setTexture(&getStatsTexture);
    getStatsBtn.setPosition(x * 0.2, y * 6.2);

    exitBtn = sf::RectangleShape(sf::Vector2f(x * 1.8, y * 0.5));
    exitBtn.setFillColor(sf::Color::White);
    exitBtn.setTexture(&exitTexture);
    exitBtn.setPosition(x * 2.7, y * 6.2);

    failed = false;
}

// draw all the gui
void Gui::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(cluSize);
    target.draw(temp);
    target.draw(spawnSpeed);
    target.draw(spawnAmt);
    target.draw(calcSpeed);
    target.draw(getStatsBtn);
    target.draw(toggleSpawnBtn);
    target.draw(startBtn);
    target.draw(stopBtn);
    target.draw(pauseBtn);
    target.draw(exitBtn);
}
// this function is called when lmb was clicked on mpos
// checks for all the gui that are in the gui section
void Gui::check(sf::Vector2i mpos) {
    auto p = sf::Vector2f(mpos);
    click = true;
    if (stopBtn.getGlobalBounds().contains(p)) {
        stopRequest = true;
    }
    else if (pauseBtn.getGlobalBounds().contains(p)) {
        *paused = !*paused;
    }
    else if (getStatsBtn.getGlobalBounds().contains(p)) {
        statsRequest = true;
    }
    else if (startBtn.getGlobalBounds().contains(p)) {
        *paused = false;
    }
    else if (toggleSpawnBtn.getGlobalBounds().contains(p)) {
        *spawnRandom = !*spawnRandom;
    } 
    else if (exitBtn.getGlobalBounds().contains(p)) {
        exitRequest = true;
    }
    else {
        click = false;
    }


    if (cluSize.contains(p)) {
        cluSize.onPress(p);
        sizeChange = true;
    }
    else if (temp.contains(p)) {
        temp.onPress(p);
        tempChange = true;
    }
    else if (spawnSpeed.contains(p)) {
        spawnSpeed.onPress(p);
    }
    else if (spawnAmt.contains(p)) {
        spawnAmt.onPress(p);
    }
    else if (calcSpeed.contains(p)) {
        calcSpeed.onPress(p);
        fpsChange = true;
    }
    
}



