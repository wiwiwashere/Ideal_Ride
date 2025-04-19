#pragma once
#include <vector>
#include <string>
#include <map>
#include <nlohmann/json.hpp>
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
    
public:
    //void addRide(const std::string& name);
    //void addRide(const string& name, double x, double y);
    //void addPath(const int from, const int to, double distance_min);
    //void setWaitTime(const std::string& ride, int minutes);
    //const auto& getAdjacencyList() const;
    const std::vector<Ride> getRides() const;
    void loadData(const std::string& filename);
};