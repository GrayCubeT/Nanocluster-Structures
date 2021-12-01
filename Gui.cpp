#include "Gui.h"

//sf::Font GlobalFont;

// there is legit no way this works
// it works btw
Gui::Gui(sf::Vector2f* _clientsize, bool* _paused, bool* _spawnRandom, bool* _noDraw,
    bool* _specialMode,
    double* _cluSize, double* _temp, int* _spawnSpeed, int* _spawnAmt,
    int* _fps) :
    clientsize(_clientsize), paused(_paused), spawnRandom(_spawnRandom),
    statsRequest(false), stopRequest(false), click(false), failed(true), exitRequest(false),
    fpsChange(false), tempChange(false), sizeChange(false), simulation(false), simRequest(false),
    specialModeRequest(false), specialMode(_specialMode),
    clusterCounter("", GlobalFont),
    cluSize(
        _cluSize, MAX_CLUSTER_SIZE, MIN_CLUSTER_SIZE,
        sf::FloatRect(
            _clientsize->x / 14 * 0.2, _clientsize->y / 7 * 1.8,
            _clientsize->x * (CONTROL_PART * 0.01) * (138.0 / 140.0) - 100, _clientsize->y / 7 * 0.62),
        "images/size.png"),
    temp(
        _temp, MAX_TEMP, MIN_TEMP,
        sf::FloatRect(
            _clientsize->x / 14 * 0.2, _clientsize->y / 7 * 2.6,
            _clientsize->x * (CONTROL_PART * 0.01) * (138.0 / 140.0) - 100, _clientsize->y / 7 * 0.62),
        "images/diffusion.png"),
    spawnSpeed(
        _spawnSpeed, MAX_SPAWN_SPEED, MIN_SPAWN_SPEED,
        sf::FloatRect(
            _clientsize->x / 14 * 0.2, _clientsize->y / 7 * 3.4,
            _clientsize->x * (CONTROL_PART * 0.01) * (138.0 / 140.0) - 100, _clientsize->y / 7 * 0.62),
        "images/flux.png"),
    spawnAmt(
        _spawnAmt, MAX_SPAWN_AMT, MIN_SPAWN_AMT,
        sf::FloatRect(
            _clientsize->x / 14 * 0.2, _clientsize->y / 7 * 4.2,
            _clientsize->x * (CONTROL_PART * 0.01) * (138.0 / 140.0) - 100, _clientsize->y / 7 * 0.62),
        "images/quantity.png"),
    calcSpeed(
        _fps, MAX_FPS, MIN_FPS,
        sf::FloatRect(
            _clientsize->x / 14 * 0.2, _clientsize->y / 7 * 5.0,
            _clientsize->x * (CONTROL_PART * 0.01) * (138.0 / 140.0) - 100, _clientsize->y / 7 * 0.62),
        "images/speed.png")

{
    if (!stopTexture.loadFromFile("images/clearBtn.png")) {
        failed = true;
        return;
    }
    if (!startTexture.loadFromFile("images/startBtn.png")) {
        failed = true;
        return;
    }
    if (!simulationTexture.loadFromFile("images/speedGenerate.png")) {
        failed = true;
        return;
    }
    if (!unpauseTexture.loadFromFile("images/pauseOff.png")) {
        failed = true;
        return;
    }
    if (!pauseTexture.loadFromFile("images/pauseOn.png")) {
        failed = true;
        return;
    }
    if (!spawnOnTexture.loadFromFile("images/spawnOn.png")) {
        failed = true;
        return;
    }
    if (!spawnOffTexture.loadFromFile("images/spawnOff.png")) {
        failed = true;
        return;
    } 
    if (!getStatsTexture.loadFromFile("images/getStatsBtn.png")) {
        failed = true;
        return;
    }
    if (!exitTexture.loadFromFile("images/exitBtn.png")) {
        failed = true;
        return;
    }
    if (!specialModeOnTexture.loadFromFile("images/specialOn.png")) {
        failed = true;
        return;
    }
    if (!specialModeOffTexture.loadFromFile("images/specialOff.png")) {
        failed = true;
        return;
    }
    if (!ccTexture.loadFromFile("images/counter.png")) {
        failed = true;
        return;
    }
    
    // equivalent of 100 in regular mode
    double x = clientsize->x / 14 * (CONTROL_PART * 0.02);
    double y = clientsize->y / 7;

    startBtn = sf::RectangleShape(sf::Vector2f(x * 1.8, y * 0.5));
    startBtn.setFillColor(sf::Color::White);
    startBtn.setTexture(&startTexture);
    startBtn.setPosition(x * 0.2, y * 0.2);

    pauseBtn = sf::RectangleShape(sf::Vector2f(x * 1.8, y * 0.5));
    pauseBtn.setFillColor(sf::Color::White);
    pauseBtn.setTexture(&pauseTexture);
    pauseBtn.setPosition(x * 2.7, y * 1.1);

    stopBtn = sf::RectangleShape(sf::Vector2f(x * 1.8, y * 0.5));
    stopBtn.setFillColor(sf::Color::White);
    stopBtn.setTexture(&stopTexture);
    stopBtn.setPosition(x * 2.7, y * 0.2);

    toggleSpawnBtn = sf::RectangleShape(sf::Vector2f(x * 1.8, y * 0.5));
    toggleSpawnBtn.setFillColor(sf::Color::White);
    toggleSpawnBtn.setTexture(&spawnOnTexture);
    toggleSpawnBtn.setPosition(x * 0.2, y * 1.1);

    getStatsBtn = sf::RectangleShape(sf::Vector2f(x * 1.8, y * 0.5));
    getStatsBtn.setFillColor(sf::Color::White);
    getStatsBtn.setTexture(&getStatsTexture);
    getStatsBtn.setPosition(x * 0.2, y * 6.2);

    exitBtn = sf::RectangleShape(sf::Vector2f(x * 1.8, y * 0.5));
    exitBtn.setFillColor(sf::Color::White);
    exitBtn.setTexture(&exitTexture);
    exitBtn.setPosition(x * 5.2, y * 6.2);

    simulateBtn = sf::RectangleShape(sf::Vector2f(x * 1.8, y * 0.5));
    simulateBtn.setFillColor(sf::Color::White);
    simulateBtn.setTexture(&simulationTexture);
    simulateBtn.setPosition(x * 2.7, y * 6.2);

    specialModeBtn = sf::RectangleShape(sf::Vector2f(x * 1.8, y * 0.5));
    specialModeBtn.setFillColor(sf::Color::White);
    specialModeBtn.setTexture(&specialModeOffTexture);
    specialModeBtn.setPosition(x * 5.2, y * 1.1);

    ccText = sf::RectangleShape(sf::Vector2f(x * 3.6, y * 0.5));
    ccText.setFillColor(sf::Color::White);
    ccText.setTexture(&ccTexture);
    ccText.setPosition(x * 5.2, y * 0.2);

    clusterCounter.setPosition(x * 8.9, y * 0.2);
    clusterCounter.setCharacterSize(30);
    clusterCounter.setFillColor(TEXT_COLOR);
    clusterCounter.setOutlineColor(OUTLINE_COLOR);
    clusterCounter.setOutlineThickness(OUTLINE_THICKNESS);

    failed = false;
}

// draw all the gui
void Gui::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (!simulation) {
        target.draw(cluSize);
        target.draw(temp);
        target.draw(spawnSpeed);
        target.draw(spawnAmt);
        target.draw(getStatsBtn);   
        target.draw(toggleSpawnBtn);
        target.draw(startBtn);
        target.draw(stopBtn);
        target.draw(specialModeBtn);
    }
    target.draw(simulateBtn);
    target.draw(calcSpeed);
    target.draw(pauseBtn);
    target.draw(exitBtn);
    target.draw(ccText);
    target.draw(clusterCounter);
}

// this function is called when lmb was clicked on mpos
// checks for all the gui that are in the gui section
void Gui::check(sf::Vector2i mpos) {
    auto p = sf::Vector2f(mpos);
    click = true;
    bool simclick = true;
    if (!simulation) {
        if (getStatsBtn.getGlobalBounds().contains(p)) {
            statsRequest = true;
        }
        else if (startBtn.getGlobalBounds().contains(p)) {
            *paused = false;
            pauseBtn.setTexture(&unpauseTexture);
        }
        else if (toggleSpawnBtn.getGlobalBounds().contains(p)) {
            *spawnRandom = !*spawnRandom;
            if (*spawnRandom) {
                toggleSpawnBtn.setTexture(&spawnOnTexture);
            }
            else {
                toggleSpawnBtn.setTexture(&spawnOffTexture);
            }
        }
        else if (specialModeBtn.getGlobalBounds().contains(p)) {
            specialModeRequest = true;
            *specialMode = !*specialMode;
            if (*specialMode) {
                specialModeBtn.setTexture(&specialModeOnTexture);
            }
            else {
                specialModeBtn.setTexture(&specialModeOffTexture);
            }
        }
        else {
            simclick = false;
        }
    }
    if (stopBtn.getGlobalBounds().contains(p)) {
        stopRequest = true;
        *paused = true;
        *spawnRandom = true;
        toggleSpawnBtn.setTexture(&spawnOnTexture);
        pauseBtn.setTexture(&pauseTexture);
    }
    else if (pauseBtn.getGlobalBounds().contains(p)) {
        *paused = !*paused;
        if (*paused) {
            pauseBtn.setTexture(&pauseTexture);
        }
        else {
            pauseBtn.setTexture(&unpauseTexture);
        }
    }
    else if (exitBtn.getGlobalBounds().contains(p)) {
        exitRequest = true;
    }
    else if (simulateBtn.getGlobalBounds().contains(p)) {
        simRequest = true;
    }
    else {
        click = false;
    }
    click = click || simclick;

    if (!simulation) {
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
    }
    if (calcSpeed.contains(p)) {
        calcSpeed.onPress(p);
        fpsChange = true;
    }
    
}

void Gui::update() {
    if (*paused) {
        pauseBtn.setTexture(&pauseTexture);
    }
    else {
        pauseBtn.setTexture(&unpauseTexture);
    }
    if (*spawnRandom) {
        toggleSpawnBtn.setTexture(&spawnOnTexture);
    }
    else {
        toggleSpawnBtn.setTexture(&spawnOffTexture);
    }
    if (*specialMode) {
        specialModeBtn.setTexture(&specialModeOnTexture);
    }
    else {
        specialModeBtn.setTexture(&specialModeOffTexture);
    }
}

void Gui::setCounter(int n) {
    if (n == -1) {
        clusterCounter.setString("");
    }
    else {
        clusterCounter.setString(numtostr(n));

    }
}




