#include "ClusterStructure.h"


void ClusterStructure::load(std::ifstream& in) {

    in.seekg(0, std::ios::beg);
    in.read(reinterpret_cast<char*>(&maxClusters), sizeof(int));
    in.read(reinterpret_cast<char*>(&dimension), sizeof(double));
    in.read(reinterpret_cast<char*>(&temperature), sizeof(double));
    in.read(reinterpret_cast<char*>(&clusterSize), sizeof(double));
    

    int cluNum;
    in.read(reinterpret_cast<char*>(&cluNum), sizeof(int));

    this->regenerate(temperature, clusterSize);

    for (int i = 0; i < cluNum; i++) {
        char st;
        unsigned long long int lifetime;
        double rad;
        float x, y;

        in.read(&st, sizeof(char));
        in.read(reinterpret_cast<char*>(&rad), sizeof(rad));
        in.read(reinterpret_cast<char*>(&x), sizeof(float));
        in.read(reinterpret_cast<char*>(&y), sizeof(float));
        in.read(reinterpret_cast<char*>(&lifetime), sizeof(unsigned long long int));
        
        this->addCluster(HexCluster(rad, sf::Vector2f(x, y), (st != 0), lifetime));

    }
}

void ClusterStructure::save(std::ofstream& out) {
    out.write(reinterpret_cast<const char*>(&maxClusters), sizeof(maxClusters));
    out.write(reinterpret_cast<const char*>(&dimension), sizeof(dimension));
    out.write(reinterpret_cast<const char*>(&temperature), sizeof(temperature));
    out.write(reinterpret_cast<const char*>(&clusterSize), sizeof(clusterSize));
    
    out.write(reinterpret_cast<const char*>(&clusterNum), sizeof(clusterNum));

    for (int i = 0; i < clusterNum; i++) {
        char st = (char)clusters[i].stopped;
        out.write(&st, sizeof(st));
        out.write(reinterpret_cast<const char*>(&(clusters[i].radius)), sizeof(clusters[i].radius));
        out.write(reinterpret_cast<const char*>(&(clusters[i].pos.x)), sizeof(clusters[i].pos.x));
        out.write(reinterpret_cast<const char*>(&(clusters[i].pos.y)), sizeof(clusters[i].pos.y));
        out.write(reinterpret_cast<const char*>(&(clusters[i].lifetime)), sizeof(clusters[i].lifetime));
    }

}

ClusterStructure::ClusterStructure(int maxClu, double temp, double cluSize, double moveDelay, double _dimension) :
        moveGenerator(temp), temperature(temp), maxClusters(maxClu), clusterSize(cluSize), 
        dimension(_dimension), mmap(), staticCollision(false), mt_rand(time(0)), 
        realRandom(-_dimension, _dimension) {
    clusterNum = 0;
    vertices.clear();
    vertices.resize(maxClu * 12ll);
    vertices.setPrimitiveType(sf::Triangles);
    clusters.reserve(maxClu);
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
    clusters.push_back(HexCluster(clusterSize, 
            sf::Vector2f(realRandom(mt_rand), realRandom(mt_rand)), false));
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
                        vertices[(clusterNum - 1) * 12ll + j].position = sf::Vector2f(-40000, -40000);
                    }
                    clusterNum--;
                    i--;
                    continue;
                } else {
                    mmap.remove(&clusters[i]);
                    clusters.pop_back();
                    clusterNum--;
                    for (int j = 0; j < 12; ++j) {
                        vertices[i * 12ll + j].position = sf::Vector2f(-40000, -40000);
                    }
                    i--;
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

void ClusterStructure::regenerate(double temp, double cluSize) {
    // recreate the map
    mmap.clear();

    for (int i = 0; i < maxClusters * 12; i++) {
        vertices[i].position = sf::Vector2f(-40000, -40000);
    }
    clusters.clear();
    moveGenerator.settmp(temp);
    temperature = temp;
    clusterSize = cluSize;
    clusterNum = 0;
}

void ClusterStructure::settemp(double temp) {
    temperature = temp;
    moveGenerator.settmp(temp);
}

void ClusterStructure::swapCollisionTech() {
    staticCollision = !staticCollision;
    mmap.staticCollision = !mmap.staticCollision;
}

std::vector<HexCluster*> ClusterStructure::timesort() {
    std::vector<HexCluster*> ans;
    for (int i = 0; i < clusterNum; ++i) {
        if (clusters[i].stopped) {
            ans.push_back(&clusters[i]);
        }
    }
    std::sort(ans.begin(), ans.end(), [](HexCluster* a, HexCluster* b) {return a->lifetime < b->lifetime; });
    return ans;
}

MeshMap::MeshMap(const int& _stepAmt,
    const double& _clusterSize,
    const double& _dimensions)

    : dimension(_dimensions), stepAmt(_stepAmt),
    stepNum(0), cellSize(_clusterSize), staticCollision(false) {
    // dimension is equal to half of the size of the rectangle -> and the cellsize is doubled
    size = std::ceil(dimension / cellSize * 2);
    meshMap = new std::list<Cluster*>[size * size];

}

MeshMap::~MeshMap() {
    delete[] meshMap;
}
sf::Vector2u MeshMap::translateToInternalCoords(sf::Vector2f pos) {
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
    if (staticCollision) {
        if (a->stopped || b->stopped) {
            a->stopped = true;
            b->stopped = true;
            if (a->lifetime == 0) {
                a->lifetime = gclock.getElapsedTime().asMicroseconds();
            }
            if (b->lifetime == 0) {
                b->lifetime = gclock.getElapsedTime().asMicroseconds();
            }
        }
    }
    else {
        a->stopped = true;
        b->stopped = true;
        if (a->lifetime == 0) {
            a->lifetime = gclock.getElapsedTime().asMicroseconds();
        }
        if (b->lifetime == 0) {
        b->lifetime = gclock.getElapsedTime().asMicroseconds();
    }
    }
}


void MeshMap::clear() {
    for (int i = 0; i < size * size; i++) {
        meshMap[i].clear();
    }
}
