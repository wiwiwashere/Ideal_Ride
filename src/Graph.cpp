#include "Graph.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
//using namespace std;
using json = nlohmann::json;

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

void Graph::initEmpty(int n) {
    nodeCount = n;
    // Initialize adjacency matrix with INFINITY (no edge)
    adjMatrix.assign(n, std::vector<int>(n, INF));

    // Set diagonal to 0 (distance to self is 0)
    for (int i = 0; i < n; i++) {
        adjMatrix[i][i] = 0;
    }

    visitedNodes.assign(n, UNVISITED);
}

const std::vector<Ride> Graph::getRides() const
{
    return rides;
}

const std::map<std::string, std::vector<Ride>> Graph::getThemeParks() const
{
    return ThemeParks;
}

int Graph::edgeCount() const {
    int count = 0;
    for (int i = 0; i < nodeCount; i++) {
        for (int j = i + 1; j < nodeCount; j++) {
            if (adjMatrix[i][j] != INF) {
                count++;
            }
        }
    }
    return count;
}

std::vector<int> Graph::neighbors(int v) const {
    std::vector<int> result;
    for (int i = 0; i < nodeCount; i++) {
        if (adjMatrix[v][i] != INF && v != i) {
            result.push_back(i);
        }
    }
    return result;
}


void Graph::loadData(const std::string& filename) {

    std::cout << "Loading data..." << std::endl;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open " + filename);
    }

    std::string line;
    int totalRides = 0;

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
        std::string themepark = f[1];
        std::string lat = f[5];
        std::string lng = f[6];
        ThemeParks[themepark].push_back({ rideName, lat + "," + lng, wait });
        totalRides+=1;
    }

    std::cout << "Loaded " << totalRides << " available rides" << std::endl;

    file.close();
}

void Graph::buildGraphFromAPI(std::string api_key)
{
    if (nodeCount < 2) return;

    //std::cout << "perform HTTP GET with libcurl\n";
    //perform HTTP GET with libcurl
    CURL* curl = curl_easy_init();
    std::string origins, destinations;
    std::string response;
    for (int i = 0; i < nodeCount; i++) {
        //eg "28.4721,-81.4625"
        std::string coord = rides[i].coord;
        //URL‑escape it:
        char* esc = curl_easy_escape(curl, coord.c_str(), 0);
        origins += esc;
        destinations += esc;
        curl_free(esc);
        if (i+1 < nodeCount) {
            origins += "|";
            destinations += "|";
        }
    }

    //std::cout << "build the full URL\n";
    //build the full URL
    std::ostringstream url;
    url << "https://maps.googleapis.com/maps/api/distancematrix/json"
        << "?mode=walking"
        << "&origins="      << origins
        << "&destinations=" << destinations
        << "&key="          << api_key;

    //std::cout << "perform HTTP GET\n";
    //perform HTTP GET ---
    std::string resp;
    curl_easy_setopt(curl, CURLOPT_URL, url.str().c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resp);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "curl error: " << curl_easy_strerror(res) << "\n";
    }
    curl_easy_cleanup(curl);

    //std::cout << "parse JSON\n";
    //parse JSON
    auto root = json::parse(resp);
    if (root["status"] != "OK") {
        throw std::runtime_error("Distance API returned status " + root["status"].get<std::string>());
    }

    //std::cout << "fill edges\n";
    //extract the s*s matrix of walking times (in seconds)
    //fill edges
    for (int i = 0; i < nodeCount; ++i)
    {
        //std::cout << i << "filling" << std::endl;
        auto& elements = root["rows"][i]["elements"];
        for (int j = 0; j < nodeCount; ++j) {
            if (elements[j]["status"] == "OK")
            {
                int second = elements[j]["duration"]["value"];
                int min = (second + 59)/60; //round up to minutes
                addEdge(i, j, min);
            }
            //else leave it INF (as it is)
        }
    }
}