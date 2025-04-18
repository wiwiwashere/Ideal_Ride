#include "Graph.h"
#include <sstream>
#include <iostream>

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

    cout << "Please give your selection of rides separated by space, please restraint to maximum 5 selections!\n" ;

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
    return 0;
}