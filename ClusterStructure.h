#pragma once
#include <SFML/Graphics.hpp>
#include "shapes.h"
#include "GlobalFunctions.h"
#include "Constants.h"
#include <random>
#include <list>
#include <fstream>
#include <stdlib.h>
#include <algorithm>
#include <vector>


extern sf::Clock gclock;

// An internal class that represents a square map of squares for faster collision detection
// also supports splitting the map and taking multiple steps to calculate collisions
class MeshMap {
private:
    double dimension;
    double cellSize;
    size_t size;
public:
    int stepAmt;
    int stepNum;
    bool staticCollision;

    //the actual map
    std::list<Cluster*>* meshMap;

    
    MeshMap(const int& _stepAmt = 1, const double& _clusterSize = 300, const double& _dimensions = 10000);
    
    ~MeshMap();
    sf::Vector2u translateToInternalCoords(sf::Vector2f pos);
    std::list<Cluster*>& find(const sf::Vector2f& point);
    bool insert(Cluster* clu, sf::Vector2f offset = sf::Vector2f());
    bool remove(Cluster* clu);
    bool move(Cluster* clu, sf::Vector2f v);

    // check all the neighboiuring spaces for collisions
    // internally calls collision method. only runs a part of the calculation
    void checkCollision();
    void collision(Cluster* a, Cluster* b);


    void clear();
};

// this class represents the entirety of clusters 
// plus has all the nessesary interfaces to be drawn, make steps and check collisions
class ClusterStructure : public sf::Drawable {
private:
    sf::VertexArray vertices;
    int maxClusters;
    std::vector <HexCluster> clusters;
    BrounRNG moveGenerator;
    std::mt19937 mt_rand;
    std::uniform_real_distribution<double> realRandom;

    double dimension;
    MeshMap mmap;
    bool staticCollision;

public:
    int clusterNum;
    double temperature;
    double clusterSize;

    void load(std::ifstream& in);
    void save(std::ofstream& out);

    ClusterStructure(int maxClusters, double temp, double cluSize = 30, double moveDelay = 1, double _dimension = 10000.0);
    bool addCluster(HexCluster);
    bool addRandom();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    bool step();
    int collisionCheck();
    sf::Vector2u __secret_debug_function__(sf::Vector2f pos);
    void regenerate(double temp, double cluSize);
    void settemp(double temp);
    void swapCollisionTech();

    std::vector<HexCluster*> timesort();
    

};