#include "Graph.h"
#include <queue>
#include <limits>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
using namespace std;

struct QueueNode {
    int cost;
    string id;
    vector<std::string> path;
    
    bool operator>(const QueueNode& other) const {
        return cost > other.cost;
    }
};

void Graph::addRide(const string& name) {
    rides.push_back(name);
    adjMatrix.push_back(vector<int>());
}

void Graph::addPath(const int from, const int to, double distance_min) {
    adjMatrix[from][to] = distance_min;
    adjMatrix[to][from] = distance_min;
}

void Graph::setWaitTime(const string& ride, int minutes) {
    waitTimes[ride] = minutes;
}

const vector<string> Graph::getRides() const
{
    return rides;
}

void Graph::loadWaitTimes(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Could not open CSV file");
    }

    string line;

    //1) Skip header
    getline(file, line);

    //2) Read each line
    while (getline(file, line)) {
        stringstream ss(line);
        string number, park, ride_name, avg_wait, type, source;

        // Read comma‐separated fields
        getline(ss, number, ',');
        getline(ss, park, ',');
        getline(ss, ride_name, ',');
        getline(ss, avg_wait, ',');

        int weight = stoi(avg_wait);

        // Add vertices and wait
        addRide(ride_name);
        setWaitTime(ride_name, weight);
    }

    file.close();
}