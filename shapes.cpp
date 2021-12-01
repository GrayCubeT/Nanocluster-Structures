#include "shapes.h"



HexCluster::HexCluster(const double r, const sf::Vector2f& p, bool stop, unsigned long long int stime) : 
        Cluster(12, p, stop) {
    lifetime = stime;
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


bool HexCluster::intersects(Cluster* other) {
    HexCluster* o;
    sf::Vector2f dist = other->pos - pos;
    if (o = dynamic_cast<HexCluster*>(other)) {
        double maxdist = radius + o->radius;
        maxdist *= maxdist;
        return (dist.x * dist.x + dist.y * dist.y < maxdist);
    }
    else {
        double maxdist = radius;
        maxdist *= maxdist;
        return (dist.x * dist.x + dist.y * dist.y < maxdist);
    }

}

bool Cluster::intersects(Cluster* other) {
    return (pos == other->pos);
}
