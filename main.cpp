#include "graph.h"
#include <fstream>

int main() {
    setlocale(LC_ALL, "rus");

    std::ifstream file("input.txt");
    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    RoadMap roadmap;
    if (!(file >> roadmap)) {
        std::cerr << "Error reading data from file." << std::endl;
        return 1;
    }
    file.close();

    printRoadMap(roadmap);
    std::cout << "\n";


    std::vector<std::vector<int>> graph(roadmap.vertex);
    for (const auto& edge : roadmap.edges) {
        if (edge.size() == 2) {
            int a = edge[0];
            int b = edge[1];
            graph[a].push_back(b);
            graph[b].push_back(a);
        }
    }

    BFSResult bfsResult = bfs(graph, roadmap.road);

    std::cout << "Shortest distances from vertex " << roadmap.road << ":\n";
    for (int i = 0; i < bfsResult.dist.size(); i++) {
        std::cout << "Vertex " << i << ": ";
        if (bfsResult.dist[i] == INT_MAX) {
            std::cout << "unreachable\n";
        }
        else {
            std::cout << bfsResult.dist[i] << "\n";
        }
    }


    std::ofstream fout("output.txt");
    if (fout) {
        fout << bfsResult;
    }

    //just for fun :)
    std::cout << "\n";
    int destination = 0;
    std::cout << "Input a vertex from where we will build a shortcut to the specified vertex: "; std::cin >> destination;
    if (destination >= 0 && destination < roadmap.vertex && bfsResult.dist[destination] != INT_MAX) {
        std::vector<int> path = getPath(bfsResult.parent, destination);
        std::cout << "\nShortest path from vertex " << roadmap.road
            << " to vertex " << destination << ":\n";
        for (int v : path) {
            std::cout << v << " ";
        }
        std::cout << "\n";
    }
    else {
        std::cout << "\nVertex " << destination
            << " is unreachable from vertex " << roadmap.road << ".\n";
    }

    return 0;
}
