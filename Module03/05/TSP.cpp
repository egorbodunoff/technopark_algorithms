#include "include/TSP.h"

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

int TSP::Enumeration(const ListGraph& graph) {
    int verticesCount = graph.VerticesCount();
    std::vector<int> vertices(verticesCount);
    for (int i = 0; i < verticesCount; ++i) {
        vertices[i] = i;
    }

    int minWeight = std::numeric_limits<int>::max();
    bool hasNextPermutation = std::next_permutation(vertices.begin() + 1, vertices.end());

    while (hasNextPermutation) {
        int pathWeight = calculatePathWeight(graph, vertices);
        minWeight = std::min(minWeight, pathWeight);
        hasNextPermutation = std::next_permutation(vertices.begin() + 1, vertices.end());
    }

    return minWeight;
}

int TSP::calculatePathWeight(const ListGraph& graph, const std::vector<int>& path) {
    int weight = 0;
    for (int i = 0; i < path.size() - 1; ++i) {
        int from = path[i];
        int to = path[i + 1];
        int edgeWeight = graph.GetWeight(from, to);
        if (edgeWeight == -1) {
            return std::numeric_limits<int>::max();
        }
        weight += edgeWeight;
    }
    return weight;
}

