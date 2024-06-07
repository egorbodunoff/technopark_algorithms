#include <cassert>
#include <iostream>
#include <queue>
#include <vector>
#include <climits>

class ListGraph {
public:
    ListGraph(int vertexCount);

    void AddEdge(int from, int to);
    int VerticesCount() const { return adjLists.size(); }

    std::vector<int> GetNextVertices(int vertex) const;

private:
    std::vector<std::vector<int>> adjLists;
};

ListGraph::ListGraph(int vertexCount) {
    assert(vertexCount > 0);
    adjLists.resize(vertexCount);
}

void ListGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < adjLists.size());
    assert(to >= 0 && from < adjLists.size());

    adjLists[from].push_back(to);
    adjLists[to].push_back(from);
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjLists.size());
    return adjLists[vertex];
}

int countShortestPaths(const ListGraph& graph, int start, int tail) {
    int vertexCount = graph.VerticesCount();
    std::vector<int> dist(vertexCount, INT_MAX);
    std::vector<int> count(vertexCount, 0);
    std::queue<int> bfsQ;

    dist[start] = 0;
    count[start] = 1;
    bfsQ.push(start);
    while (!bfsQ.empty()) {
        int vertex = bfsQ.front();
        bfsQ.pop();

        for (int v : graph.GetNextVertices(vertex)) {
            if (dist[v] == INT_MAX) {
                dist[v] = dist[vertex] + 1;
                bfsQ.push(v);
            }
            if (dist[v] == dist[vertex] + 1) {
                count[v] += count[vertex];
            }
        }
    }

    return count[tail];
}

int main() {
    int v, n = 0;
    std::cin >> v;
    std::cin >> n;
    ListGraph graph(v);

    for (int i = 0; i < n; ++i) {
        int from, to = 0;
        std::cin >> from >> to;
        graph.AddEdge(from, to);
    }
    int from, to = 0;
    std::cin >> from >> to;
    std::cout << countShortestPaths(graph, from, to) << std::endl;
    return 0;
}