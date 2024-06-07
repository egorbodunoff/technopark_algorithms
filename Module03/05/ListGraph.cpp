#include "include/ListGraph.h"

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

int ListGraph::GetWeight(int from, int to) const { return adjLists[from][to].weight; }