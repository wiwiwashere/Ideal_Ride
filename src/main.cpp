#include "Graph.h"
#include <sstream>
#include <iostream>
#include "MST.h"

int main() {
    Graph universal;
    universal.loadWaitTimes("Rides.csv");
    //universal.loadWaitTimes("Rides.csv");
    
    cout << "Welcome to Universal Studios Ride Planner\n";
    cout << "============================\n\n";

    cout << "Here's a list of rides at Universal Studios\n";
    vector<string> rides = universal.getRides();

    for (int i = 0; i < rides.size(); i++)
    {
        cout << (i+1) << ": " << rides[i] << endl;
    }

    cout << "Please give your selection of rides separated by space!\n" ;

    string line;
    getline(cin, line);//read in whole line

    istringstream iss(line);
    vector<int> selections;
    string x;
    while (iss >> x) {//extract until EOF
        selections.push_back(stoi(x));
    }

    cout << "You selected: \n";
    for (auto ri : selections)
    {
        cout << rides[ri-1] << endl;
    }

    /*
    cout << "Optimal path:\n";
    for (size_t i = 0; i < path.size(); ++i) {
        cout << (i+1) << ". " << path[i];
        if (i != path.size()-1) cout << " -> ";
    }
    */
    
    cout << "\n\nTotal walking distance + wait times optimized!\n";


    //example: user selected rides (we index them 0..k-1)
    vector<string> rides_example = {
        "Harry Potter", "Transformers", "Jurassic Park", "Mummy Ride"
    };
    int k = rides.size();
    int startIdx = 0; // e.g. "Harry Potter"

    //example distance matrix (symmetric, zeros on diagonal)
    Matrix mat = {
        {0, 5, 10, 8},
        {5, 0, 3, 7},
        {10,3, 0, 9},
        {8, 7, 9, 0}
    };

    //compute MSTs
    auto primTree    = primMST(mat, startIdx);
    auto kruskalTree = kruskalMST(mat);

    //get visit orders
    vector<int> orderPrim, orderKruskal;
    dfsOrder(startIdx, -1, primTree,    orderPrim);
    dfsOrder(startIdx, -1, kruskalTree, orderKruskal);

    // Print results
    cout << "Prim's MST visit order:\n";
    for (int idx : orderPrim)
    {
        cout << " - " << rides_example[idx] << "\n";
    }

    cout << "\nKruskal's MST visit order:\n";
    for (int idx : orderKruskal)
    {
        cout << " - " << rides_example[idx] << "\n";
    }

    return 0;




    return 0;
}
