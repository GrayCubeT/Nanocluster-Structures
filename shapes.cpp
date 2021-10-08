#include "shapes.h"

Hexagon::Hexagon(const double radius = 0) :
    h_radius(radius) {
    update();
}

void Hexagon::setRadius(const double radius) {
    h_radius = radius;
    update();
}

const double Hexagon::getRadius() const {
    return h_radius;
}

std::size_t Hexagon::getPointCount() const {
    return 6;
}

sf::Vector2f Hexagon::getPoint(size_t index) const {
    static const float pi = 3.141592654f;

    double angle = (double) index * 2 * pi / getPointCount() - pi / 2;
    double x = cos(angle) * h_radius;
    double y = sin(angle) * h_radius;

    return sf::Vector2f(h_radius + x, h_radius + y);
}

HexCluster::HexCluster(const double r, const sf::Vector2f& p, bool stop) : Cluster(12, p, stop) {
    radius = r;
    double angle, x, y;
    static const double pi = 3.141592654;
    // 4 triangles
    for (int i = 1; i <= 4; i++) {

        verticesPos.push_back(sf::Vector2f(0, -radius));

        angle = (double)i * 2 * pi / 6 - pi / 2;
        x = cos(angle) * radius;
        y = sin(angle) * radius;
        verticesPos.push_back(sf::Vector2f(x, y));

        angle = (double)(i + 1.0) * 2 * pi / 6 - pi / 2;
        x = cos(angle) * radius;
        y = sin(angle) * radius;
        verticesPos.push_back(sf::Vector2f(x, y));

    }
}

Cluster::Cluster(int num, const sf::Vector2f& p, bool stop) {
    pos = p;
    vertexnum = num;
    verticesPos.reserve(num);
    stopped = stop;
    lifetime = 0;
}

