#include "include/SetGraph.h"

#include <cassert>

SetGraph::SetGraph(int vertexCount) {
    assert(vertexCount > 0);

    adjLists.resize(vertexCount);
    prevAdjLists.resize(vertexCount);
}

SetGraph::SetGraph(const IGraph &graph) {
    adjLists.resize(graph.VerticesCount());
    prevAdjLists.resize(graph.VerticesCount());

    for (int i = 0; i < VerticesCount(); i++) {
        std::vector<int> vertices = graph.GetNextVertices(i);

        for (int j = vertices.size() - 1; j >= 0; --j) {
            AddEdge(i, vertices[j]);
        }
    }
}

void SetGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < adjLists.size());
    assert(to >= 0 && to < adjLists.size());

    adjLists[from].insert(to);
    prevAdjLists[to].insert(from);
}

int SetGraph::VerticesCount() const { return adjLists.size(); }

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    std::vector<int> v;
    for (int i : adjLists[vertex]) v.push_back(i);

    return v;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    std::vector<int> v;
    for (int i : prevAdjLists[vertex]) v.push_back(i);

    return v;
}