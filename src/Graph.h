#pragma once
#include <vector>
#include <string>
#include <map>
#include <nlohmann/json.hpp>
#include <limits>
#include <functional>
//using namespace std;

struct Ride {
    std::string name;
    std::string coord; // "lat,lng"
    int wait_time;
};

class Graph {
private:
    //map<string, vector<pair<string, double>>> adjList;
    //map data type: ride_name, list of rides it connects to
    //sparse graph, theme park typically have relatively few connections per ride
    //std::vector<std::vector<int>> adjMatrix;
    std::vector<Ride> rides;
    int nodeCount;
    std::vector<std::vector<int>> adjMatrix; // Adjacency matrix
    std::vector<int> visitedNodes; // To mark nodes as visited/unvisited


public:
    //void addRide(const std::string& name);
    //void addRide(const string& name, double x, double y);
    //void addPath(const int from, const int to, double distance_min);
    //void setWaitTime(const std::string& ride, int minutes);
    //const auto& getAdjacencyList() const;
    const std::vector<Ride> getRides() const;
    void loadData(const std::string& filename);
    Graph(int n) : nodeCount(n) {
        // Initialize adjacency matrix with INFINITY (no edge)
        adjMatrix.resize(n, std::vector<int>(n, INFINITY));
        
        // Set diagonal to 0 (distance to self is 0)
        for (int i = 0; i < n; i++) {
            adjMatrix[i][i] = 0;
        }
        
        visitedNodes.resize(n, UNVISITED);
    }

    void addEdge(int u, int v, int weight) {
        adjMatrix[u][v] = weight;
        adjMatrix[v][u] = weight; // For undirected graph -- Ride to another ride and back - should be okay
    }

    int getNodeCount() const {
        return this->nodeCount;
    }

    int edgeCount() const {
        int count = 0;
        for (int i = 0; i < nodeCount; i++) {
            for (int j = i + 1; j < nodeCount; j++) {
                if (adjMatrix[i][j] != INFINITY) {
                    count++;
                }
            }
        }
        return count;
    }

    std::vector<int> neighbors(int v) const {
        std::vector<int> result;
        for (int i = 0; i < nodeCount; i++) {
            if (adjMatrix[v][i] != INFINITY && v != i) {
                result.push_back(i);
            }
        }
        return result;
    }

    int weight(int u, int v) const {
        return adjMatrix[u][v];
    }

    void setValue(int v, int value) {
        visitedNodes[v] = value;
    }

    int getValue(int v) const {
        return visitedNodes[v];
    }
};
