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
    adjList[name] = {};
}

void Graph::addPath(const string& from, const string& to, double distance) {
    adjList[from].emplace_back(to, distance);
    adjList[to].emplace_back(from, distance);
}

void Graph::setWaitTime(const string& ride, int minutes) {
    waitTimes[ride] = minutes;
}

const vector<string> Graph::getRides() const
{
    return rides;
}

void Graph::loadData() {
    
    // Connect rides with walking paths (minutes needed to walk from one to another)
    addPath("Harry Potter", "Hippogriff", 150);
    addPath("Harry Potter", "Gringotts", 200);
    addPath("Gringotts", "Transformers",200);
    addPath("Jurassic Park", "Transformers", 300);
    addPath("Transformers", "The Mummy", 100);
    
    // Load wait times
    loadWaitTimes("Rides.csv");
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

vector<string> Graph::Dijkstra(const string& start, const string& end) {
    
    using QueueNode = pair<double, pair<string, vector<string>>>;
    
    priority_queue<QueueNode, vector<QueueNode>, greater<>> pq;
    map<string, double> dist;
    
    for (const auto& ride : rides) {
        dist[ride] = numeric_limits<double>::max();
    }
    
    pq.push({static_cast<const double&>(waitTimes[start]), {start, {start}}});
    dist[start] = waitTimes[start];
    
    while (!pq.empty()) {
        auto [currentDist, node] = pq.top();
        auto [current, path] = node;
        pq.pop();
        
        if (current == end) return path;
        
        for (auto& [neighbor, distance] : adjList[current]) {
            double newDist = currentDist + distance + waitTimes[neighbor];
            if (newDist < dist[neighbor]) {
                dist[neighbor] = newDist;
                vector<string> newPath = path;
                newPath.push_back(neighbor);
                pq.push({newDist, {neighbor, newPath}});
            }
        }
    }
    
    return {};
}