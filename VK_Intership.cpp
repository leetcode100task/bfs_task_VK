#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <queue>
#include <climits>

struct RoadMap {
    int vertex; // количество вершин
    int edge;   // количество рёбер
    std::vector<std::vector<int>> edges; // список рёбер
    int road;   // стартовая вершина


    friend std::istream& operator>>(std::istream& in, RoadMap& data) {
        std::string line;
        // Чтение количества вершин
        if (std::getline(in, line)) {
            std::stringstream ss(line);
            if (!(ss >> data.vertex) || data.vertex <= 0) {
                std::cerr << "Ошибка: неверное число вершин."
                          << data.vertex 
                          << std::endl;
                in.setstate(std::ios::failbit);
                return in;
            }
        }
        else {
            in.setstate(std::ios::failbit);
            return in;
        }

        // Чтение количества рёбер
        if (std::getline(in, line)) {
            std::stringstream ss(line);
            if (!(ss >> data.edge) || data.edge < 0) {
                std::cerr << "Ошибка: неверное число рёбер."
                          << data.edge 
                          << std::endl;
                in.setstate(std::ios::failbit);
                return in;
            }
        }
        else {
            in.setstate(std::ios::failbit);
            return in;
        }

        // Чтение списка рёбер
        for (int i = 0; i < data.edge; ++i) {
            if (std::getline(in, line)) {
                std::stringstream ss(line);
                int a, b;
                if (!(ss >> a >> b)) {
                    std::cerr << "Ошибка: не удалось считать пару вершин "
                              << a 
                              << b 
                              << " для ребра." 
                              << std::endl;
                    in.setstate(std::ios::failbit);
                    return in;
                }
                if (a < 0 || a >= data.vertex ) {
                    std::cerr << "Ошибка: номер вершины: " << a 
                              << " вне допустимого диапазона." << std::endl;
                    in.setstate(std::ios::failbit);
                    return in;
                }
                if (b < 0 || b >= data.vertex) {
                    std::cerr << "Ошибка: номер вершины: "<< b 
                              << " вне допустимого диапазона." << std::endl;
                    in.setstate(std::ios::failbit);
                    return in;
                }
                data.edges.push_back({ a, b });
            }
            else {
                std::cerr << "Ошибка: недостаточно строк для чтения рёбер." << std::endl;
                in.setstate(std::ios::failbit);
                return in;
            }
        }

        // Чтение стартовой вершины
        if (std::getline(in, line)) {
            std::stringstream ss(line);
            if (!(ss >> data.road)) {
                std::cerr << "Ошибка: не удалось считать номер стартовой вершины." << std::endl;
                in.setstate(std::ios::failbit);
                return in;
            }
            if (data.edge < data.road) {
                std::cerr << "Ошибка: ошибка выхода за пределы диапазона: " 
                          << data.road 
                          << " вне допустимого диапазона." 
                          << std::endl;
                in.setstate(std::ios::failbit);
                return in;
            }
            if (data.road < 0 || data.road >= data.vertex) {
                std::cerr << "Ошибка: стартовая вершина: " 
                    << data.road 
                    << " вне допустимого диапазона." << std::endl;
                in.setstate(std::ios::failbit);
                return in;
            }
        }
        else {
            std::cerr << "Ошибка: отсутствует строка со стартовой вершиной." << std::endl;
            in.setstate(std::ios::failbit);
            return in;
        }

        return in;
    }
};

void printRoadMap(const RoadMap& roadmap) {
    std::cout << "Количество вершин: " << roadmap.vertex << "\n";
    std::cout << "Количество рёбер: " << roadmap.edge << "\n";
    std::cout << "Список рёбер:\n";
    for (const auto& edge : roadmap.edges) {
        if (edge.size() == 2) {
            std::cout << edge[0] << " " << edge[1] << "\n";
        }
    }
    std::cout << "Стартовая вершина: " << roadmap.road << "\n";
}
struct BFSResult {
    std::vector<int> dist;
    std::vector<int> parent;
    friend std::ostream& operator<<(std::ostream& out, 
                                const BFSResult& data) {
        for (size_t i = 0; i < data.dist.size(); ++i) {
            out << "Вершина " << i << ": ";
            if (data.dist[i] == INT_MAX) {
                out << "недостижима\n";
            }
            else {
                out << data.dist[i] << "\n";
            }
        }
        return out;
    }
};

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

int main() {
    setlocale(LC_ALL, "rus");
    std::ifstream file("input.txt");
    if (!file) {
        std::cerr << "Ошибка открытия файла!" << std::endl;
        return 1;
    }

    RoadMap roadmap;
    if (!(file >> roadmap)) {
        std::cerr << "Ошибка при чтении данных из файла." << std::endl;
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

    std::cout << "Кратчайшие расстояния от вершины " 
              << roadmap.road << ":\n";
    for (int i = 0; i < bfsResult.dist.size(); i++) {
        std::cout << "Вершина " << i << ": ";
        if (bfsResult.dist[i] == INT_MAX) {
            std::cout << "недостижима\n";
        }
        else {
            std::cout << bfsResult.dist[i] << "\n";
        }
    }
    std::ofstream fout("output.txt");
    if (fout) {
        fout << bfsResult;
    }


    //часть кода, была сделана просто так, 
    // показывает кратчайшый маршрут, 
    // от точки интереса до заданой вершины xd
    int destination = 2;

    if (destination >= 0 && 
        destination < roadmap.vertex && 
        bfsResult.dist[destination] != INT_MAX) {

        std::vector<int> path = getPath(bfsResult.parent, destination);
        std::cout << "\nКратчайший путь от вершины " << 
                                        roadmap.road << 
                                      " до вершины " << 
                                         destination << ":\n";
        for (int v : path) {
            std::cout << v << " ";
        }
        std::cout << "\n";
    }
    else {
        std::cout << "\nВершина " << destination 
                                  << " недостижима от вершины " 
                                  << roadmap.road << ".\n";
    }

    return 0;
}