#pragma once
#include <vector>
#include <string>
#include <map>
using namespace std;

// struct Ride {
//     string name;
//     double x, y;
// };

class Graph {
private:
    map<string, vector<pair<string, double>>> adjList;
    //map data type: ride_name, list of rides it connects to
    //sparse graph, theme park typically have relatively few connections per ride
    vector<string> rides;
    map<string, int> waitTimes;
    
public:
    void addRide(const string& name);
    //void addRide(const string& name, double x, double y);
    void addPath(const string& from, const string& to, double distance_min);
    void setWaitTime(const string& ride, int minutes);
    //const auto& getAdjacencyList() const;
    const vector<string> getRides() const;
    void loadData();
    void loadWaitTimes(const string& filename);
    vector<string> Dijkstra(
        const string& start,
        const string& end
    );
    friend class APIhelper;
};