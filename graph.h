#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>

#include <sstream>

#include <vector>

#include <string>

#include <queue>

#include <climits>

#include <algorithm>



struct RoadMap {
    int vertex; // number of vertices

    int edge;   // number of edges

    std::vector<std::vector<int>> edges; // list of edges

    int road;   // starting vertex

    friend std::istream& operator>>(std::istream& in, RoadMap& data);
};

void printRoadMap(const RoadMap& roadmap);

struct BFSResult {
    std::vector<int> dist;

    std::vector<int> parent;

    friend std::ostream& operator<<(std::ostream& out, const BFSResult& data);

};

BFSResult bfs(const std::vector<std::vector<int>>& graph, int start);

std::vector<int> getPath(const std::vector<int>& parent, int finish);

#endif // GRAPH_H