#include "Graph.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
//using namespace std;
using json = nlohmann::json;

// void Graph::addPath(const int from, const int to, double distance_min) {
//     adjMatrix[from][to] = distance_min;
//     adjMatrix[to][from] = distance_min;
// }

// helper to trim whitespace from both ends
static void trim(std::string &s) {
    // left trim
    s.erase(s.begin(),
            std::find_if(s.begin(), s.end(),
                         [](unsigned char ch){ return !std::isspace(ch); }));
    // right trim
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](unsigned char ch){ return !std::isspace(ch); })
                .base(),
            s.end());
}

const std::vector<Ride> Graph::getRides() const
{
    return rides;
}

void Graph::loadData(const std::string& filename) {

    std::cout << "Loading data..." << std::endl;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open " + filename);
    }

    std::string line;

    // skip header
    getline(file, line);

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> f;
        //split on commas
        while (getline(ss, token, ',')) {
            f.push_back(token);
        }
        if (f.size() < 7) continue;
        //sanity check

        std::string rideType = f[3];
        if (rideType != "yes") continue;

        std::string rideName = f[2];
        // 1) trim the wait-time field
        std::string waitStr = f[4];
        trim(waitStr);

        // 2) parse safely
        int wait = 0;
        try {
            if (!waitStr.empty())
                wait = std::stoi(waitStr);
        } catch (std::exception &e) {
            std::cerr << "Warning: could not parse wait time '"
                      << waitStr << "'; defaulting to 0\n";
        }
        std::string lat = f[5];
        std::string lng = f[6];
        rides.push_back({ rideName, lat + "," + lng, wait });
    }

    std::cout << "Loaded " << rides.size() << " available rides" << std::endl;

    file.close();
}