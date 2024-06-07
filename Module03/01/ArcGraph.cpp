#include "include/ArcGraph.h"

#include <cassert>

ArcGraph::ArcGraph(int vertexCount) {
    assert(vertexCount > 0);
    verticesNumber = vertexCount;
}

ArcGraph::ArcGraph(const IGraph &graph) {
    verticesNumber = graph.VerticesCount();

    for (int i = 0; i < graph.VerticesCount(); i++) {
        std::vector<int> vertices = graph.GetNextVertices(i);
        for (int j : vertices) AddEdge(i, j);
    }
}

void ArcGraph::AddEdge(int from, int to) {
    assert(from >= 0 && to >= 0);

    for (const Edge &edge : edgesList)
        if (edge.from == from && edge.to == to) {
            return;
        }
    Edge edge(from, to);
    edgesList.push_back(edge);
}

int ArcGraph::VerticesCount() const { return verticesNumber; }

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < edgesList.size());
    std::vector<int> v;

    for (const Edge &edge : edgesList) {
        if (edge.from == vertex) {
            v.push_back(edge.to);
        }
    }
    return v;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < edgesList.size());
    std::vector<int> v;

    for (const Edge &edge : edgesList) {
        if (edge.to == vertex) {
            v.push_back(edge.from);
        }
    }
    return v;
}