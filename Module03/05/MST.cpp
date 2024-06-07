#include <cassert>
#include <climits>
#include <iostream>
#include <set>
#include <vector>

struct Edge {
    int to;
    int weight;

    Edge(int _to, int _weight) : to(_to), weight(_weight) {}
};

class ListGraph {
   public:
    ListGraph(int vertexCount);

    void AddEdge(int from, int to, int weight);
    int VerticesCount() const { return adjLists.size(); }

    std::vector<Edge> GetNextVertices(int vertex) const;

   private:
    std::vector<std::vector<Edge>> adjLists;
};

ListGraph::ListGraph(int vertexCount) {
    assert(vertexCount > 0);
    adjLists.resize(vertexCount);
}

void ListGraph::AddEdge(int from, int to, int weight) {
    assert(from >= 0 && from < adjLists.size());
    assert(to >= 0 && to < adjLists.size());

    Edge edge_to(to, weight);
    Edge edge_from(from, weight);

    adjLists[from].push_back(edge_to);
    adjLists[to].push_back(edge_from);
}

std::vector<Edge> ListGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjLists.size());
    return adjLists[vertex];
}

class TSP {
   public:
    int CalcMST(const ListGraph& graph);

   private:
    std::vector<std::vector<Edge>> adjLists;
};

int TSP::CalcMST(const ListGraph& graph) {
    int calcDist = 0;
    int vertexCount = graph.VerticesCount();
    std::vector<int> distances(vertexCount, INT_MAX);
    distances[0] = 0;

    std::set<std::pair<int, int>> priorityQueue;
    priorityQueue.insert({0, 0});
    std::vector<bool> inMST(vertexCount, false);

    while (!priorityQueue.empty()) {
        int distance = priorityQueue.begin()->first;
        int vertex = priorityQueue.begin()->second;
        priorityQueue.erase(priorityQueue.begin());

        if (inMST[vertex]) {
            continue;
        }

        inMST[vertex] = true;
        calcDist += distance;

        for (const Edge& edge : graph.GetNextVertices(vertex)) {
            int nextVertex = edge.to;
            int weight = edge.weight;

            if (!inMST[nextVertex] && weight < distances[nextVertex]) {
                priorityQueue.erase({distances[nextVertex], nextVertex});
                distances[nextVertex] = weight;
                priorityQueue.insert({weight, nextVertex});
            }
        }
    }
    return calcDist;
}

int main() {
    int verticesCount = 0;
    int edgesCount = 0;
    std::cin >> verticesCount >> edgesCount;
    ListGraph graph(verticesCount);

    int from = 0;
    int to = 0;
    int weight = 0;
    for (int i = 0; i < edgesCount; i++) {
        std::cin >> from >> to >> weight;
        graph.AddEdge(from - 1, to - 1, weight);
    }

    TSP tsp;
    std::cout << tsp.CalcMST(graph) << std::endl;
    return 0;
}