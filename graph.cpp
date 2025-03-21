#include "graph.h"

std::istream& operator>>(std::istream& in, RoadMap& data) {
    std::string line;

    // Read number of vertices
    if (std::getline(in, line)) {
        std::stringstream ss(line);
        if (!(ss >> data.vertex) || data.vertex <= 0) {
            std::cerr << "Error: invalid number of vertices: "
                << data.vertex << std::endl;
            in.setstate(std::ios::failbit);
            return in;
        }
    }
    else {
        in.setstate(std::ios::failbit);
        return in;
    }

    // Read number of edges
    if (std::getline(in, line)) {
        std::stringstream ss(line);
        if (!(ss >> data.edge) || data.edge < 0) {
            std::cerr << "Error: invalid number of edges: "
                << data.edge << std::endl;
            in.setstate(std::ios::failbit);
            return in;
        }
    }
    else {
        in.setstate(std::ios::failbit);
        return in;
    }

    // Read edge list
    for (int i = 0; i < data.edge; ++i) {
        if (std::getline(in, line)) {
            std::stringstream ss(line);
            int a, b;
            if (!(ss >> a >> b)) {
                std::cerr << "Error: failed to read vertex pair "
                    << a << " " << b << " for an edge." << std::endl;
                in.setstate(std::ios::failbit);
                return in;
            }
            if (a < 0 || a >= data.vertex) {
                std::cerr << "Error: vertex number " << a
                    << " is out of valid range." << std::endl;
                in.setstate(std::ios::failbit);
                return in;
            }
            if (b < 0 || b >= data.vertex) {
                std::cerr << "Error: vertex number " << b
                    << " is out of valid range." << std::endl;
                in.setstate(std::ios::failbit);
                return in;
            }
            data.edges.push_back({ a, b });
        }
        else {
            std::cerr << "Error: not enough lines to read edges." << std::endl;
            in.setstate(std::ios::failbit);
            return in;
        }
    }

    // Read starting vertex
    if (std::getline(in, line)) {
        std::stringstream ss(line);
        if (!(ss >> data.road)) {
            std::cerr << "Error: failed to read the starting vertex number." << std::endl;
            in.setstate(std::ios::failbit);
            return in;
        }
        if (data.edge < data.road) {
            std::cerr << "Error: out-of-range error: "
                << data.road << " is out of valid range." << std::endl;
            in.setstate(std::ios::failbit);
            return in;
        }
        if (data.road < 0 || data.road >= data.vertex) {
            std::cerr << "Error: starting vertex " << data.road
                << " is out of valid range." << std::endl;
            in.setstate(std::ios::failbit);
            return in;
        }
    }
    else {
        std::cerr << "Error: missing line for the starting vertex." << std::endl;
        in.setstate(std::ios::failbit);
        return in;
    }

    return in;
}

std::ostream& operator<<(std::ostream& out, const BFSResult& data) {
    for (size_t i = 0; i < data.dist.size(); ++i) {
        out << "Vertex " << i << ": ";
        if (data.dist[i] == INT_MAX) {
            out << "unreachable\n";
        }
        else {
            out << data.dist[i] << "\n";
        }
    }
    return out;
}

void printRoadMap(const RoadMap& roadmap) {
    std::cout << "Number of vertices: " << roadmap.vertex << "\n";
    std::cout << "Number of edges: " << roadmap.edge << "\n";
    std::cout << "Edge list:\n";
    for (const auto& edge : roadmap.edges) {
        if (edge.size() == 2) {
            std::cout << edge[0] << " " << edge[1] << "\n";
        }
    }
    std::cout << "Starting vertex: " << roadmap.road << "\n";
}

BFSResult bfs(const std::vector<std::vector<int>>& graph, int start) {
    BFSResult result;
    int n = graph.size();
    result.dist.assign(n, INT_MAX);
    result.parent.assign(n, -1);

    std::queue<int> q;
    result.dist[start] = 0;
    q.push(start);

    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (int neighbor : graph[v]) {
            if (result.dist[neighbor] > result.dist[v] + 1) {
                result.dist[neighbor] = result.dist[v] + 1;
                result.parent[neighbor] = v;
                q.push(neighbor);
            }
        }
    }
    return result;
}

std::vector<int> getPath(const std::vector<int>& parent, int finish) {
    std::vector<int> path;
    for (int cur = finish; cur != -1; cur = parent[cur])
        path.push_back(cur);
    std::reverse(path.begin(), path.end());
    return path;
}
