#pragma once
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <nlohmann/json.hpp>
#include <limits>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <functional>

using json = nlohmann::json;

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
    std::map<std::string, std::vector<Ride>> ThemeParks;
    std::vector<Ride> rides;
    int nodeCount;
    std::vector<std::vector<int>> adjMatrix; // Adjacency matrix
    std::vector<int> visitedNodes; // To mark nodes as visited/unvisited

    const int VISITED = 1;
    const int UNVISITED = 0;
    const int INF = std::numeric_limits<int>::max();
    
public:
    const std::vector<Ride> getRides() const;
    const std::map<std::string, std::vector<Ride>> getThemeParks() const;
    void loadData(const std::string& filename);
    Graph() : nodeCount(0) {}
    void initEmpty(int n);

    void init(const std::vector<Ride>& selectedRides)
    {
        rides = selectedRides;
        initEmpty(selectedRides.size());
    }

    void addEdge(int u, int v, int weight) {
        adjMatrix[u][v] = weight;
        adjMatrix[v][u] = weight; // For undirected graph -- Ride to another ride and back - should be okay
    }

    int getNodeCount() const {
        return this->nodeCount;
    }

    const std::vector<std::vector<int>>& getAdjMatrix() const
    {
        return this->adjMatrix;
    }

    int edgeCount() const;

    std::vector<int> neighbors(int v) const;

    int weight(int u, int v) const {
        return adjMatrix[u][v];
    }

    void setValue(int v, int value) {
        visitedNodes[v] = value;
    }

    int getValue(int v) const {
        return visitedNodes[v];
    }

    //callback for libcurl to write response into a string
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        auto* s = static_cast<std::string*>(userp);
        s->append(static_cast<char*>(contents), size * nmemb);
        return size * nmemb;
    }

    void buildGraphFromAPI(std::string api_key);
};
