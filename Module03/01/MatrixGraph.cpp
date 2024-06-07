#include "include/MatrixGraph.h"

#include <cassert>

MatrixGraph::MatrixGraph(int vertexCount) {
    assert(vertexCount > 0);

    edgesList.resize(vertexCount);
    for (std::vector<bool> &i : edgesList) {
        i.assign(vertexCount, false);
    }
}

MatrixGraph::MatrixGraph(const IGraph &graph) {
    edgesList.resize(graph.VerticesCount());
    for (std::vector<bool> &i : edgesList) i.assign(VerticesCount(), false);

    for (int i = 0; i < VerticesCount(); i++) {
        std::vector<int> vertices = graph.GetNextVertices(i);
        for (int j : vertices) AddEdge(i, j);
    }
}

void MatrixGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < edgesList.size());
    assert(to >= 0 && to < edgesList.size());

    edgesList[from][to] = true;
}

int MatrixGraph::VerticesCount() const { return edgesList.size(); }

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < edgesList.size());
    std::vector<int> v;

    for (int i = 0; i < VerticesCount(); ++i) {
        if (edgesList[vertex][i]) {
            v.push_back(i);
        }
    }
    return v;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < edgesList.size());
    std::vector<int> v;

    for (int i = 0; i < VerticesCount(); i++)
        if (edgesList[i][vertex]) {
            v.push_back(i);
        }
    return v;
}