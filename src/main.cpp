#include "Graph.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
#include <cstdlib>
#include "prim.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;


int main() {

    const std::string api_key = "AIzaSyAvzsmQlNkwI8h9bSLmb3lqO0ygcY0XtJE";

    Graph universal;

    std::cout << "Welcome to Ideal Ride Planner\n";
    std::cout << "============================\n\n";

    //load rides.csv
    universal.loadData("Rides.csv");
    std::map<std::string, std::vector<Ride>> themeParks = universal.getThemeParks();

    std::cout << "\nHere's a list of theme parks that you could choose from:" << std::endl;

    for (auto park : themeParks)
    {
        std::cout << park.first << std::endl;
    }

    std::cout << "\nPlease enter the full name of the theme park you want to choose\n";
    std::string park_name;
    std::cin >> park_name;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::vector<Ride> ride = themeParks[park_name];

    for (size_t i = 0; i < ride.size(); i++)
    {
        std::cout << (i+1) << ": " << ride[i].name << std::endl;
    }

    std::cout << "\nPlease give your selection of rides separated by space! (use their orders)\n";
    std::cout << "Please list the first ride that you want to go on first!" << std::endl;
    std::cout << "Eg, '12 1 8 9', then the ride with order 12 will be the ride that I want to start with :)\n\n";
    std::cout << "Please don't list a number that's not in the options, and if you list an order more than once then we're going to take that as you want to go on the corresponding ride more than once" << std::endl;

    std::string line;
    std::getline(std::cin, line);//read in whole line

    std::istringstream iss(line);
    std::vector<int> selections;
    std::string x;
    while (iss >> x) {//extract until EOF
        selections.push_back(stoi(x));
    }

    std::cout << "\nYou selected: \n";
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

    universal.init(sel); //change the rides from all the rides to only selected
    universal.buildGraphFromAPI(api_key);

    std::cout << "\nBuilding graph from API...\n\n";

    //print MST matrix
    std::vector< std::vector<int> > adjMatrix = universal.getAdjMatrix();
    // std::cout << "Distance matrix (minutes):\n";
    // for (int i = 0; i < s; i++) {
    //     for (int j = 0; j < s; j++) {
    //         std::cout << adjMatrix[i][j] << (j+1<s ? "," : "");
    //     }
    //     std::cout << "\n";
    // }

    int startVertex = 0;

    Prim prim;
    std::cout << "Running Prim's algorithm starting from vertice " << startVertex << std::endl;
    //auto mstEdges = prim.buildMST(universal, 0);
    //auto visitOrder = prim.getMSTTraversalOrder(mstEdges, universal.getNodeCount(), 0);

    std::vector<int> D(adjMatrix.size()); // Distance array -- num of nodes basicallt will eaul num of rides that user engtered
    std::vector<int> parent(adjMatrix.size()); // Predecessor array
    std::vector<std::vector<int>> mstEdges =  prim.runPrimAlgorithm(universal, startVertex, D, parent); //g here has to be the selection grpah i made above

    //print ride to ride distances
    // std::cout << "\nMinimum Spanning Tree:" << std::endl;
    // int lastVertex;
    // for (int i = 0; i < adjMatrix.size(); i++) {
    //     if (i != startVertex && D[i] != INFINITY) {
    //         std::cout << "Edge: " << parent[i] << " -> " << i << " Weight: " << D[i] << std::endl;
    //     }
    //     lastVertex = i;
    // }

    std::cout << "\nTotal trip distance given by Prim's: ";
    int totalWeight = 0;
    for (int i = 0; i < adjMatrix.size(); i++) {
        if (i != startVertex && D[i] != INFINITY) {
            totalWeight += D[i];
        }
    }

    if (mstEdges.size() != 0)
    {
        std::cout << totalWeight << " mins (not including wait time)" << std::endl;
    }
    else {std::cout << "Not available\n\n";}

    std::vector<int> correctPathOrder = prim.getMSTTraversalOrder(mstEdges, universal.getNodeCount(), 0);

    std::cout << "Prim's traversal order: " << std::endl;
    if (mstEdges.size() != 0)
    {
        for (int idx: correctPathOrder)
        {
            std::cout << " - " << sel[idx].name << std::endl;
        }
    }else {std::cout << "Not available\n\n";}


    std::cout << "\n\nTotal walking distance + wait times optimized!\n";


    return 0;
}