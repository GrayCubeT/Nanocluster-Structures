#pragma once
#include <SFML\Graphics\Shape.hpp>
#include <math.h>
#include <vector>


class Cluster {
public:
    bool stopped;
    unsigned long long int lifetime;
    sf::Vector2f pos;
    int vertexnum;
    // relative pos of vertices -> triangles
    std::vector<sf::Vector2f> verticesPos;

    Cluster(int num, const sf::Vector2f& p = sf::Vector2f(), bool stop = false);
    virtual bool intersects(Cluster* other);
};

// defines a hexagonal cluster 
class HexCluster : public Cluster {
public:
    double radius;
    HexCluster(const double r = 1, const sf::Vector2f & p = sf::Vector2f(), bool stop = false, unsigned long long int stime = 0);
    virtual bool intersects(Cluster* other) override;
};