#include "Graph.h"
#include <queue>
#include <vector>
#include <iostream>
#include <limits>

class Prim {
private:
    const int VISITED = 1;
    const int UNVISITED = 0;
    const int INF = std::numeric_limits<int>::max();
    
    // Helper struct for the priority queue
    struct DVPair {
        int distance;  // distance to "to" ride
        int vertex;    // to - vertex(ride)
        
        DVPair(int k, int v) : distance(k), vertex(v) {}
        
        // For priority queue comparison - msut comapre walking distances as a means to get pplaces- basically the weights of the graph
        bool operator>(const DVPair& other) const {
            return distance > other.distance;
        }
    };
    
public:
    // makes the Minimum Spanning Tree (MST) starting from the given vertex
    std::vector<std::vector<int>> buildMST(Graph& graph, int start) {
        int k = graph.getNodeCount();
        std::vector<int> distance(k, INF);
        std::vector<int> parent(k, -1);
        
        return runPrimAlgorithm(graph, start, distance, parent);
    }
    
    // Returns both the MST edges and fills the distance and parent vectors
    std::vector<std::vector<int>> buildMST(Graph& graph, int start, 
                                          std::vector<int>& distance, 
                                          std::vector<int>& parent) {
        return runPrimAlgorithm(graph, start, distance, parent);
    }
    
    // Makes a real path using DFS 
    std::vector<int> getMSTTraversalOrder(const std::vector<std::vector<int>>& mstEdges, int nodeCount, int start) {
        // Convert edge list to adjacency list
        std::vector<std::vector<int>> adjList(nodeCount);
        for (const auto& edge : mstEdges) {
            int u = edge[0];
            int v = edge[1];
            adjList[u].push_back(v);
            adjList[v].push_back(u);  // Undirected graph - this si okay
        }
        
        std::vector<bool> visited(nodeCount, false);
        std::vector<int> order; //for DFS
        
        dfsOrder(start, visited, adjList, order);
        return order;
    }
    
public:
    //actual Prim's aglorithm 
    std::vector<std::vector<int>> runPrimAlgorithm(Graph& graph, int start, 
                                                 std::vector<int>& distance, 
                                                 std::vector<int>& parent) {
        int k = graph.getNodeCount();
        std::priority_queue<DVPair, std::vector<DVPair>, std::greater<DVPair>> distanceHeap;
        int currVertex;
        std::vector<std::vector<int>> MSTedges;
        
        // Initialize distances to infinity - basically like dijkstras 
        for (int i = 0; i < k; i++) {
            distance[i] = INF;
        }
        distance[start] = 0;  // Start vertex has distance 0 to itself
        
        distanceHeap.push(DVPair(0, start));
        
        for (int i = 0; i < k; i++) {  // For each vertex
            if (distanceHeap.empty()) {
                std::cout << "Heap empty" << std::endl;
                return MSTedges;
            }
            
            // Get minimum distance vertex
            DVPair minDist = distanceHeap.top();
            distanceHeap.pop();
            currVertex = minDist.vertex;
            
            std::cout << "Curr Min distance: " << minDist.distance << " Vertex: " << currVertex << std::endl;
            
            // Skip vertices that have already been visited
            while (graph.getValue(currVertex) == VISITED) {
                if (distanceHeap.empty()) {
                    return MSTedges;
                }
                
                minDist = distanceHeap.top();
                distanceHeap.pop();
                currVertex = minDist.vertex;
            }
            
            graph.setValue(currVertex, VISITED);  // Mark as visited
            
            if (distance[currVertex] == INF) {
                return MSTedges;
            }
            
            // Add edge to MST (except for start vertex)
            if (currVertex != start) {
                MSTedges.push_back({parent[currVertex], currVertex});
            }
            
            // for all neighbors
            std::vector<int> neighbors = graph.neighbors(currVertex);
            for (int j = 0; j < neighbors.size(); j++) {
                int neighbor = neighbors[j];
                
                // If neighbor is not visited and has a smaller weight
                if (graph.getValue(neighbor) != VISITED && 
                    distance[neighbor] > graph.weight(currVertex, neighbor)) {
                    distance[neighbor] = graph.weight(currVertex, neighbor);
                    parent[neighbor] = currVertex;
                    distanceHeap.push(DVPair(distance[neighbor], neighbor));
                }
            }
        }
        
        //returns edges, but must call the getMSTTraversalOrder() on these edges so that you can get real path
        return MSTedges;
    }
    
    void dfsOrder(int u, std::vector<bool>& visited, const std::vector<std::vector<int>>& mst, std::vector<int>& order) {
        visited[u] = true;
        // Add current node to path order
        order.push_back(u);
        
        // Visit all unvisited neighbors
        for (int v : mst[u]) {
            if (!visited[v]) {
                dfsOrder(v, visited, mst, order);
            }
        }
    }
};
