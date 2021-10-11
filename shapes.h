#pragma once
#include <SFML\Graphics\Shape.hpp>
#include <math.h>
#include <vector>

class Hexagon : public sf::Shape {
public:
    Hexagon(const double radius);

    void setRadius(const double radius = 0);
    const double getRadius() const;

    virtual std::size_t getPointCount() const;
    virtual sf::Vector2f getPoint(size_t index) const override;
    double h_radius;
};

class Cluster {
public:
    bool stopped;
    double lifetime;
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
    HexCluster(const double r = 1, const sf::Vector2f & p = sf::Vector2f(), bool stop = false);
    virtual bool intersects(Cluster* other) override;
};