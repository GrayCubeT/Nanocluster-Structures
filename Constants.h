#pragma once
#include <SFML/Graphics.hpp>

extern sf::Clock gclock;

enum {
    //minmax of all the slider values
    MAX_CLUSTER_SIZE = 200,
    MIN_CLUSTER_SIZE = 5,
    MAX_CLUSTER_NUM = 10000,
    MAX_SPAWN_AMT = 20,
    MIN_SPAWN_AMT = 1,
    MIN_TEMP = 1,
    MAX_TEMP = 200,
    MIN_SPAWN_SPEED = 2,
    MAX_SPAWN_SPEED = 240,
    MAX_FPS = 250,
    MIN_FPS = 30,

    // used for view generation
    DEFAULTVIEWSIZE = 23000,
    // in percent
    CONTROL_PART = 30,
    // maximum amount of clusters
    CIRCUSSIZE = 10000,
    // default values
    // default size of one cluster
    CLUSTER_SIZE = 90,
    // default temperature
    START_TEMPERATURE = 10,
    // how much frames it takes for next step to be calculated
    MOVE_DELAY = 1,
    // how much frames it takes to spawn a new random cluster (should be a multiple of move_delay)
    SPAWN_DELAY = 1,
    // how much clusters are randomly spawned each time
    SPAWN_AMOUNT = 1,
    // do random clusters spawn?
    RANDOM_ENABLED = true,

    // this is more global speed control (higher = faster)
    FPS = 120
};