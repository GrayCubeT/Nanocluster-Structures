#include "ClusterStructure.h"


ClusterStructure::ClusterStructure(int maxClu, double temp, double cluSize, double moveDelay, double _dimension) : 
        moveGenerator(temp), temperature(temp), maxClusters(maxClu), clusterSize(cluSize), 
        dimension(_dimension) {
    clusterNum = 0;
    vertices.clear();
    vertices.resize(maxClu * 12ll);
    vertices.setPrimitiveType(sf::Triangles);
    clusters.reserve(maxClu);
    mmap = MeshMap(moveDelay, cluSize * 4, _dimension);
}

bool ClusterStructure::addCluster(HexCluster clu) {
    if (clusterNum >= maxClusters) {
        return false;
    }
    clusters.push_back(clu);
    for (int i = 0; i < 12; ++i) {
        vertices[clusterNum * 12ll + i].position = clu.pos + clu.verticesPos[i];
    }
    mmap.insert(&clusters[clusterNum]);
    clusterNum += 1;
    return true;
}
bool ClusterStructure::addRandom() {
    if (clusterNum >= maxClusters) {
        return false;
    }
    clusters.push_back(HexCluster(clusterSize, sf::Vector2f((rand() % (int)(dimension * 2)) - dimension, (rand() % (int)(dimension * 2)) - dimension)));
    mmap.insert(&clusters[clusterNum]);
    for (int i = 0; i < 12; ++i) {
        vertices[clusterNum * 12ll + i].position = clusters[clusterNum].pos + clusters[clusterNum].verticesPos[i];
    }
    clusterNum += 1;

    return true;
}
void ClusterStructure::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(vertices);
}

// movement function, collision check stub is also here
bool ClusterStructure::step() {
    for (int i = 0; i < clusterNum; i++) {
        // random movement
        if (!clusters[i].stopped) {
            sf::Vector2f mov = moveGenerator.rng();
            // check movement of cluster before actually moving it
            // false => the cluster is out of boundaries -> remove the thing
            if (!mmap.move(&clusters[i], mov)) {
                if (clusterNum - 1 > i) {
                    mmap.remove(&clusters[i]);
                    mmap.remove(&clusters[clusterNum - 1]);
                    clusters[i] = clusters[clusterNum - 1];
                    clusters.pop_back();
                    mmap.insert(&clusters[i]);
                    for (int j = 0; j < 12; ++j) {
                        vertices[i * 12ll + j].position = clusters[i].pos + clusters[i].verticesPos[j];
                    }
                    clusterNum--;
                    i--;
                    continue;
                } else {
                    mmap.remove(&clusters[i]);
                    clusters.pop_back();
                    clusterNum--;
                    continue;
                }
            }
            clusters[i].pos += mov;
            for (int j = 0; j < 12; j++) {
                vertices[i * 12ll + j].position += mov;
            }
        }
        // collision check
        /*
        for (int j = i + 1; j < clusterNum; j++) {
            sf::Vector2f dist = clusters[i].pos - clusters[j].pos;
            double maxdist = clusters[i].radius + clusters[j].radius;
            maxdist *= maxdist;
            if (dist.x * dist.x + dist.y * dist.y < maxdist) {
                clusters[i].stopped = true;
                clusters[j].stopped = true;
            }
        }*/
        
    }
    return false;
}
int ClusterStructure::collisionCheck() {
    mmap.checkCollision();
    return mmap.stepNum;
}
sf::Vector2u ClusterStructure::__secret_debug_function__(sf::Vector2f pos) {
    return mmap.translateToInternalCoords(pos);
}

MeshMap::MeshMap(const int& _stepAmt,
    const double& _clusterSize,
    const double& _dimensions)

    : dimension(_dimensions), stepAmt(_stepAmt),
    stepNum(0), cellSize(_clusterSize) {
    // dimension is equal to half of the size of the rectangle -> and the cellsize is doubled
    size = std::ceil(dimension / cellSize * 2);
    meshMap = new std::list<Cluster*>[size * size];

}
sf::Vector2u MeshMap::translateToInternalCoords(sf::Vector2f pos) {
    // this might work incorrectly somehow?
    unsigned int x = std::floor((pos.x + dimension) / cellSize);
    unsigned int y = std::floor((pos.y + dimension) / cellSize);
    return sf::Vector2u(x, y);
}
std::list<Cluster*>& MeshMap::find(const sf::Vector2f& point) {
    sf::Vector2u pos(translateToInternalCoords(point));
    return meshMap[pos.y * size + pos.x];
}
bool MeshMap::insert(Cluster* clu, sf::Vector2f offset) {
    sf::Vector2u pos(translateToInternalCoords(clu->pos + offset));

    if (pos.x >= size || pos.y >= size || pos.x < 0 || pos.y < 0) {
        return false;
    }
    meshMap[pos.y * size + pos.x].push_front(clu);
    return true;
}
bool MeshMap::remove(Cluster* clu) {
    sf::Vector2u pos(translateToInternalCoords(clu->pos));
    if (pos.x >= size || pos.y >= size || pos.x < 0 || pos.y < 0) {
        // this is very bad, means that one cluster is way out of the boundary
        return false;
    }
    size_t s = meshMap[pos.y * size + pos.x].size();
    // basically remove clusters
    meshMap[pos.y * size + pos.x].remove(clu);
    
    return (s != meshMap[pos.y * size + pos.x].size());
}
bool MeshMap::move(Cluster* clu, sf::Vector2f v) {
    sf::Vector2f p = clu->pos + v;
    sf::Vector2u pos = translateToInternalCoords(p);
    if (pos.x > size || pos.y > size || pos.x < 0 || pos.y < 0) {
        // this means we must move the thing to the other side of the field
        return false;
    }
    // if this step's movement changed the location of the cluster, update the meshmap
    if (pos != translateToInternalCoords(clu->pos)) {
        remove(clu);
        insert(clu, v);
    }
    return true;
}
// check all the neighboiuring spaces for collisions
// internally calls collision method. only runs a part of the calculation
void MeshMap::checkCollision() {
    if (stepNum == stepAmt) {
        stepNum = 0;
    }

    // determine the sizes of checked rectangle
    unsigned int st = stepNum * (size / stepAmt), en = (++stepNum) * (size / stepAmt);
    std::list <Cluster*>* c, *r, *dr, *d;
    for (unsigned int y = st; y < en; ++y) {
        c = &meshMap[y * size];
        r = &meshMap[y * size + 1];
        if (y + 1 < size) {
            dr = &meshMap[(y + 1) * size + 1];
            d = &meshMap[(y + 1) * size];
        }
        else {
            dr = nullptr;
            d = nullptr;
        }
        for (unsigned int x = 0; x < size; ++x) {
            // 4 squares checked cus no need to double check
            // same-square check
            if (c) {
                for (auto a : *c) {
                    for (auto b : *c) {
                        if (a != b && a->intersects(b)) {
                                collision(a, b);
                        }
                    }
                }
            }
            // right-side check
            if (r) {
                for (auto a : *c) {
                    for (auto b : *r) {
                        if (a->intersects(b)) {
                            collision(a, b);
                        }
                    }
                }
            }
            // down-side check
            if (d) {
                for (auto a : *c) {
                    for (auto b : *d) {
                        if (a->intersects(b)) {
                            collision(a, b);
                        }
                    }
                }
            }
            // down-right-side check
            if (dr) {
                for (auto a : *c) {
                    for (auto b : *dr) {
                        if (a->intersects(b)) {
                            collision(a, b);
                        }
                    }
                }
            }
            
            // next check list getter (a bit smarter than should be)
            c = r;
            d = dr;
            if (x + 2 < size) {
                r = &meshMap[y * size + x + 2];
                if (y + 1 < size) {
                    dr = &meshMap[(y + 1) * size + x + 2];
                } else {
                    dr = nullptr;
                }
            } else {
                r = nullptr;
                dr = nullptr;
            }
        }
    }
}
void MeshMap::collision(Cluster* a, Cluster* b) {
    // can be made a bit more abstract
    a->stopped = true;
    b->stopped = true;
}