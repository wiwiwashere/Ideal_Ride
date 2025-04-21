#include "Graph.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
#include <cstdlib>
#include "MST.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
//using namespace std;
using json = nlohmann::json;

//callback for libcurl to write response into a string
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    auto* s = static_cast<std::string*>(userp);
    s->append(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}

int main() {

    const std::string api_key = "AIzaSyAvzsmQlNkwI8h9bSLmb3lqO0ygcY0XtJE";

    Graph universal(0);

    std::cout << "Welcome to Universal Studios Ride Planner\n";
    std::cout << "============================\n\n";

    //load rides.csv
    universal.loadData("Rides.csv");
    std::vector<Ride> ride = universal.getRides();
    auto k = ride.size();
    if (k == 0) {
        std::cerr << "No rides found in CSV.\n";
        return 1;
    }

    std::cout << "Here's a list of rides at Universal Studios\n";

    for (size_t i = 0; i < ride.size(); i++)
    {
        std::cout << (i+1) << ": " << ride[i].name << std::endl;
    }

    std::cout << "Please give your selection of rides separated by space!\n" ;

    std::string line;
    std::getline(std::cin, line);//read in whole line

    std::istringstream iss(line);
    std::vector<int> selections;
    std::string x;
    while (iss >> x) {//extract until EOF
        selections.push_back(stoi(x));
    }

    std::cout << "You selected: \n";
    for (auto ri : selections)
    {
        std::cout << ride[ri-1].name << std::endl;
    }

    //build selectedRides vector
    std::vector<Ride> sel;
    for (int idx: selections)
    {
        if (idx >= 1 && idx <= static_cast<int>(ride.size()))
        {
            sel.push_back(ride[idx-1]);
        }
    }
    int s = sel.size();
    if (s < 2) {
        std::cerr << "Need at least two rides to build a matrix.\n";
        return 1;
    }

    //perform HTTP GET with libcurl
    CURL* curl = curl_easy_init();
    std::string origins, destinations;
    std::string response;
    for (int i = 0; i < s; i++) {
        //eg "28.4721,-81.4625"
        std::string coord = sel[i].coord;
        //URL‑escape it:
        char* esc = curl_easy_escape(curl, coord.c_str(), 0);
        origins += esc;
        destinations += esc;
        curl_free(esc);
        if (i+1 < s) {
            origins += "|";
            destinations += "|";
        }
    }

    //build the full URL
    std::ostringstream url;
    url << "https://maps.googleapis.com/maps/api/distancematrix/json"
        << "?mode=walking"
        << "&origins="      << origins
        << "&destinations=" << destinations
        << "&key="          << api_key;

    //perform HTTP GET ---
    std::string resp;
    curl_easy_setopt(curl, CURLOPT_URL, url.str().c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resp);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "curl error: " << curl_easy_strerror(res) << "\n";
        return 1;
    }
    curl_easy_cleanup(curl);

    //parse JSON
    auto root = json::parse(resp);
    if (root["status"] != "OK") {
        std::cerr << "API error: " << root["status"] << "\n";
        return 1;
    }

    Graph selectionGraphforMST(s); //makes a new graph with the size of the number of selections

    //
    //extract the s*s matrix of walking times (in seconds)
    //when you are makign the vertex here, i dont' rlly understadn how you do it, but just graph has a function called add edge. 
    //just adge edge with (from, to, weight)
    //this will buidlt he graph here. 

    std::vector<std::vector<int>> matrix(s, std::vector<int>(s, 0));
    for (int i = 0; i < s; i++) {
        auto& elements = root["rows"][i]["elements"];
        for (int j = 0; j < s; j++) {
            if (elements[j]["status"] == "OK")
            {
                int seconddd = elements[j]["duration"]["value"];
                matrix[i][j] = seconddd/60.0;
                selectionGraphforMST.addEdge(i, j, seconddd/60.0); // WINNIE PLEASE CHECK LET ME KNOW IF THIS IS WHERE YOU MAKE THE GRAPH PLSSSSSS-----------------------
            }
            else
            {
                matrix[i][j] = INT_MAX;  //unreachable
            }
        }
    }



    //print MST matrix
    // std::cout << "Distance matrix (minutes):\n";
    // for (int i = 0; i < s; i++) {
    //     for (int j = 0; j < s; j++) {
    //         std::cout << matrix[i][j] << (j+1<s ? "," : "");
    //     }
    //     std::cout << "\n";
    // }



    // std::vector<int> D(numNodes); // Distance array -- num of nodes basicallt will eaul num of rides that user engtered 
    // std::vector<int> parent(numNodes); // Predecessor array
    
    // int startVertex = 0;
    
    // std::cout << "Running Prim's algorithm starting from vertex " << startVertex << std::endl;
    // std::vector<std::vector<int>> mstEdges =  runPrimAlgorithm(g, startVertex, D, parent); //g here has to be the selection grpah i made above
    
    // //print ride to ride distances
    // std::cout << "\nMinimum Spanning Tree:" << std::endl;
    // int lastVertex; 
    // for (int i = 0; i < numNodes; i++) {
    //     if (i != startVertex && D[i] != INFINITY) {
    //         std::cout << "Edge: " << parent[i] << " -> " << i << " Weight: " << D[i] << std::endl;
    //     }
    //     lastVertex = i;
    // }

    // std::cout << "\nTotal trip distance: ";
    // int totalWeight = 0;
    // for (int i = 0; i < numNodes; i++) {
    //     if (i != startVertex && D[i] != INFINITY) {
    //         totalWeight += D[i];
    //     }
    // }
    
    // std::cout << totalWeight << std::endl;
    // std::vector<int> correctPathOrder; 
    // //DFS
    //call getMSTTraversalOrder() with correctPathOrder; 
    //traverse the vector u get back from this



    std::cout << "\n\nTotal walking distance + wait times optimized!\n";


    return 0;
}
