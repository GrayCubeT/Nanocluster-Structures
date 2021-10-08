#pragma once
#include <SFML/Graphics.hpp>
#include "shapes.h"
#include "GlobalFunctions.h"
#include <random>

class ClusterStructure : public sf::Drawable {
private:
    sf::VertexArray vertices;
    int maxClusters;
    std::vector <HexCluster> clusters;
    BrounRNG moveGenerator;

public:
    int clusterNum;
    double temperature;

    ClusterStructure(int maxClusters, double temp);
    bool addCluster(HexCluster);
    bool addRandom();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    bool step();
};