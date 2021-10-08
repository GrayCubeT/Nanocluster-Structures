#include "ClusterStructure.h"


ClusterStructure::ClusterStructure(int maxClu, double temp) : moveGenerator(temp), temperature(temp), maxClusters(maxClu) {
    clusterNum = 0;
    vertices.clear();
    vertices.resize(maxClu * 12ll);
    vertices.setPrimitiveType(sf::Triangles);
    clusters.reserve(maxClu);
}

bool ClusterStructure::addCluster(HexCluster clu)
{
    if (clusterNum >= maxClusters) {
        return false;
    }
    clusters.push_back(clu);
    for (int i = 0; i < 12; ++i) {
        vertices[clusterNum * 12ll + i].position = clu.pos + clu.verticesPos[i];
    }
    clusterNum += 1;
    return true;
}

bool ClusterStructure::addRandom()
{
    
    if (clusterNum >= maxClusters) {
        return false;
    }
    HexCluster clu = HexCluster(30, sf::Vector2f((rand() % 10001) - 5000, (rand() % 10001) - 5000));
    clusters.push_back(clu);
    for (int i = 0; i < 12; ++i) {
        vertices[clusterNum * 12ll + i].position = clu.pos + clu.verticesPos[i];
    }
    clusterNum += 1;
    return true;
}


void ClusterStructure::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(vertices);
}

bool ClusterStructure::step()
{
    for (int i = 0; i < clusterNum; i++) {
        if (!clusters[i].stopped) {
            sf::Vector2f mov = moveGenerator.rng();
            clusters[i].pos += mov;
            for (int j = 0; j < 12; j++) {
                vertices[i * 12ll + j].position += mov;
            }
        }
        for (int j = i + 1; j < clusterNum; j++) {
            sf::Vector2f dist = clusters[i].pos - clusters[j].pos;
            double maxdist = clusters[i].radius + clusters[j].radius;
            maxdist *= maxdist;
            if (dist.x * dist.x + dist.y * dist.y < maxdist) {
                clusters[i].stopped = true;
                clusters[j].stopped = true;
            }
        }
    }
    return false;
}

